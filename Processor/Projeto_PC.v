module Projeto_PC(

input [31:0] PC, 			 			  //Entrada de 32 bits
input clock,     			 			  //Clock 
input reset,     			 			  //Reset
input load,      			 			  //Load funciona como flag para o jump
input [1:0] LeEntrada,   			  //Flag para esperar entrada
input aceitaFlagEntrada, 			  //Acao de confirmar o input
input carregando, 		 			  //Flag que indica quando um programa esta sendo carregado e pausa o contador do PC
input [31:0] Offset,					  //Entrada do Offset
output reg saidaFlagEntrada, 		  //Saida para acender LEDG e esperar entrada
output reg [31:0] saida, 			  //Saida de 32 bits
output reg [31:0] saidaContadorPC, //Saida de 32 bits
output reg [31:0] saidaTeste //Saida de 32 bits

);

	always@(posedge clock) begin 
	   //Caso o botão reset seja ativado reset na saída
		if(reset)begin
			saidaFlagEntrada <= 1'b0;
			saida<=32'b00000000000000000000000000000000;
		end
		//Caso o reset não seja ativado, se o load tiver ativado faça o jump
		else if(load | carregando)begin
			saida <= PC;
		end
		else if(LeEntrada == 2'b01 && aceitaFlagEntrada == 1'b1) begin
			saidaFlagEntrada <= 1'b1;
			saida <= PC;
		end
		//Caso nenhuma opção acima ocorra faça PC + 1
		else begin
			saidaFlagEntrada <= 1'b0;
			saida <= PC + 1;
		end
		
		//saida <= saida + Offset;
		saidaTeste <= saida + Offset;
		
		saidaContadorPC <= saidaTeste;
	end
 
endmodule 