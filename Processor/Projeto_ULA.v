module Projeto_ULA (selecao, X, Y, Resultado, Zero, shamt);
	
	input [4:0] selecao; //Chave de seleção de 5 bits para selecionar a operação que será realizada
	input [31:0] X, Y;   //Duas entradas da ULA
	input [4:0] shamt;
	output reg [31:0] Resultado; //Saída da ULA
	output Zero; //Saída zero (branch)
	
	always @ (*) begin
		case (selecao[4:0])
			5'b00000: Resultado = X;             //Mantém x
			5'b00001: Resultado = -X;            //Nega x
			5'b00010: Resultado = X + Y;         //Soma x e y  
			5'b00011: Resultado = X - Y;         //Subtrai y de x  
			5'b00100: Resultado = X & Y;         //And de x e y
			5'b00101: Resultado = X | Y;         //Or de x e y 
			5'b00110: Resultado = ~(X | Y);      //NOr de x e y
			5'b00111: Resultado = X / Y;         //Divisão de x por y  
			5'b01000: Resultado = X * Y;         //Multiplica x com y
			5'b01001: Resultado = (X<Y) ? 1:0;   //X menor que y
			5'b01010: Resultado = (X<=Y) ? 1:0;  //X menor ou igual a y
			5'b01011: Resultado = (X>Y) ? 1:0;   //X maior que y
			5'b01100: Resultado = (X>=Y) ? 1:0;  //X maior ou igual que y
			5'b01101: Resultado = (X==Y) ? 1:0;  //X igual a y
			5'b01110: Resultado = (X!=Y) ? 1:0;  //X diferente de Y
			5'b01111: Resultado = X << shamt;    //Deslocamento à esquerda
			5'b10000: Resultado = X >> shamt;    //Deslocamento à direita
			5'b10001: Resultado = Y;             //Mantém y
			default: Resultado = 0;
		endcase
		
	end
	
	assign Zero = (Resultado == 0) ? 1:0;
	
endmodule
