module Projeto_Mux_Entrada #(parameter DATA_WIDTH=32)
(
    input [(DATA_WIDTH-1):0] Entrada1, 
    input [(DATA_WIDTH-1):0] Entrada2, 
    input [(DATA_WIDTH-1):0] Entrada3,
    input [1:0] Select, 
	 input Halt,
    output reg [(DATA_WIDTH-1):0] Saida
);

    always @ (*) begin
        case (Select)
            2'b00: Saida = Entrada1;
            2'b01: Saida = Entrada2;
            2'b10: begin
							if(Halt == 1) begin
								Saida = Entrada3 - 1;
							end
							else begin
								Saida = Entrada3 + 1;
							end
						 end
            default: Saida = {DATA_WIDTH{1'b0}};
        endcase
    end

endmodule
