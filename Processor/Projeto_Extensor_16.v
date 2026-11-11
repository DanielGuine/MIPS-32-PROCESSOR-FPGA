module Projeto_Extensor_16(entrada, saida);

input [14:0] entrada;
output [31:0] saida;

assign saida = {16'b0000000000000000, entrada};

endmodule 