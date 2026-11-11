module Projeto_BR(endereco_escrita, endereco_leitura1, endereco_leitura2, clock, writeRegs, dados_escrita, leituraRD, leituraRS, leituraRT);

	input [4:0] endereco_leitura1;
	input [4:0] endereco_leitura2;
	input [4:0] endereco_escrita;
	input clock, writeRegs; //sinais de escrita e clock
	input [31:0] dados_escrita; //valor que sera escrito na memoria
	output [31:0] leituraRS; //saida que fornece o valor armazenado
	output [31:0] leituraRD; //saida que fornece o valor armazenado
	output [31:0] leituraRT; //saida que fornece o valor armazenado
	integer PrimeiroClock=1; //flag para escrever na memoria apenas no primeiro clock só para teste
	
	reg[31:0] regs [31:0]; //declaracao da memoria em si

	
	always @ (posedge clock) begin
		if(PrimeiroClock==1) begin
			regs[0] = 32'b00000000000000000000000000000000; 
			PrimeiroClock<=2;
		end
		
		if (writeRegs) begin
			regs[endereco_escrita] = dados_escrita;
		end
	end
	
	assign leituraRD = regs[27]; //atribuicao continua 27
	assign leituraRS = regs[endereco_leitura1]; //atribuicao continua
	assign leituraRT = regs[endereco_leitura2]; //atribuicao continua
	
endmodule