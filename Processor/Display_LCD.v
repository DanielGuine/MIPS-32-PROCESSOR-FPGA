module Display_LCD(
  input clk_50MHz,	//	50 MHz clock
  input Set_LCD,
  input [31:0] Rs, // Mensagem de entrada
//	LCD Module 16X2
  output LCD_ON,	// LCD Power ON/OFF
  output LCD_BLON,	// LCD Back Light ON/OFF
  output LCD_RW,	// LCD Read/Write Select, 0 = Write, 1 = Read
  output LCD_EN,	// LCD Enable
  output LCD_RS,	// LCD Command/Data Select, 0 = Command, 1 = Data
  inout [7:0] LCD_DATA	// LCD Data bus 8 bits
);


// reset delay gives some time for peripherals to initialize
wire DLY_RST;
reg [31:0] Msg = 32'b0;
Reset_Delay r0(	.iCLK(clk_50MHz),.oRESET(DLY_RST) );

// turn LCD ON
assign	LCD_ON		=	1'b1;
assign	LCD_BLON	=	1'b1;

always@(posedge clk_50MHz) 
begin
	if(Set_LCD)
	begin 
		Msg <= Rs;	
	end
end


lcd u1(
// Host Side
   .iCLK(clk_50MHz),
   .iRST_N(DLY_RST),
   .Msg(Msg),
// LCD Side
   .LCD_DATA(LCD_DATA),
   .LCD_RW(LCD_RW),
   .LCD_EN(LCD_EN),
   .LCD_RS(LCD_RS)
);

endmodule