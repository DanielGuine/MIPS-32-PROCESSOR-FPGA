module Offset_MD (
	input Clock,
	input [3:0] ID_Programa,
	input Set_Offset_MD,
	output reg [31:0] saida
);

always @(posedge Clock) begin
	if(Set_Offset_MD == 1) begin
		case (ID_Programa)
			4'b0000: saida = 0;
			4'b0001: saida = 100;
			4'b0010: saida = 200;
			4'b0011: saida = 300;
			4'b0100: saida = 400;
			4'b0101: saida = 500;
			4'b0110: saida = 600;
			4'b0111: saida = 700;
			4'b1000: saida = 800;
			4'b1001: saida = 900;
			4'b1010: saida = 1000;
			default: saida = 0;
		endcase
	end
end

endmodule
