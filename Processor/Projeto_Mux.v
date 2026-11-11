module Projeto_Mux #(parameter DATA_WIDTH=32)
(
	input [(DATA_WIDTH-1):0] Entrada1, 
	input [(DATA_WIDTH-1):0] Entrada2, 
	input Select, 
	output reg [(DATA_WIDTH-1):0] Saida
);


	always @ (*) begin
	   
		if(Select == 0) begin
			Saida = Entrada1;
		end
		if(Select == 1) begin
			Saida = Entrada2;
		end
	end

endmodule 