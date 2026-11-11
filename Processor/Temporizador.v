module Temporizador (
	input Clock,
	input Reset,
	input quantum_start,
	input [1:0] flag_entrada,
	input Halt,
	output reg quantum_end,
	output reg ativo
);

parameter LIMITE_QUANTUM = 16;

reg [7:0] contador;

initial begin
	contador <= 0;
	quantum_end <= 0;
	ativo <= 0;
end

always @(posedge Clock) begin
	if (Reset || Halt) begin
		contador <= 0;
		quantum_end <= 0;
		ativo <= 0;
	end
	else begin
		// Prioridade 1: Recebimento do sinal de Start
		// Se receber o comando para iniciar, ativa o estado e reseta contadores
		if (quantum_start == 1) begin
			ativo <= 1;
			contador <= 0;
			quantum_end <= 0;
		end
		
		// Prioridade 2: Processo de Contagem
		// Só conta se estiver 'ativo' e não estiver esperando entrada (flag 01)
		else if (ativo == 1 && flag_entrada != 2'b01) begin
			if (contador < LIMITE_QUANTUM - 1) begin
				contador <= contador + 1;
				quantum_end <= 0;
			end else begin
				// Atingiu o limite do Quantum
				contador <= 0;
				quantum_end <= 1; // Envia sinal de fim de quantum
				ativo <= 0;       // Desativa o timer e espera novo start
			end
		end
		
		// Prioridade 3: Limpeza de sinais
		// Garante que o pulso de quantum_end dure apenas 1 ciclo se não houver reset
		else begin
			quantum_end <= 0;
		end
	end
end

endmodule