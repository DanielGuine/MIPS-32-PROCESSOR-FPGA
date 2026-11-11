
module Projeto_MD
#(parameter DATA_WIDTH=32, parameter ADDR_WIDTH=10)
(
	input [(DATA_WIDTH-1):0] data,
	input [(DATA_WIDTH-1):0] Offset,
	input [(ADDR_WIDTH-1):0] addr,
	input write, read, read_clock, write_clock,
	output reg [(DATA_WIDTH-1):0] saida
);
	
	// Declare the RAM variable
	reg [DATA_WIDTH-1:0] ram[2**ADDR_WIDTH-1:0];
	
	always @ (posedge write_clock)
	begin
		// Write
		if (write) begin
			ram[addr + Offset] <= data;
		end
	end
	// Trocar por um clock de 50mhz -> Divisor de frequência
	always @ (posedge read_clock)
	begin
		// Read 
		if(read) begin
			saida <= ram[addr + Offset];
		end
	end
	
endmodule

