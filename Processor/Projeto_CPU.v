module Projeto_CPU (clock, clock_fast, clock_slow, reset, saidaBR, userEntrada, aceitaFlagEntrada, saidaFlagEntrada, saidaContagemPC, carregandoPrograma, setLCD, testeMD, FlagQuantumAtivo);

//ENTRADAS E SAÍDAS

input clock; //clock da CPU

input clock_fast;  //Clock de Leitura das Memórias

input clock_slow;

input [31:0] userEntrada; //Entrada para Input pelo Kit FPGA

input reset;  //Botão de Reset

input aceitaFlagEntrada; //Açao do botão de entrada

output [31:0] saidaBR; //Saída do conteúdo do Registrador 31

output saidaFlagEntrada; //Acende LEDG para indicar espera de um input

output [31:0] saidaContagemPC; //Saida com o valor atual do PC para os displays de 7 SEG

output carregandoPrograma; //Flag para indicar que um programa esta sendo carregado (Pausar PC)

output setLCD; //Flag para permitir escrita no LCD

output [31:0] testeMD;

output FlagQuantumAtivo;

//CONEXÕES

//Saida ULA
wire [31:0] saidaULA;
wire zero;

//Saída PC
wire [31:0] saidaPC;
wire [31:0] saidaTestePC;

//Saida MD
wire [31:0] saidaMD;

//Saida MI
wire [31:0] saidaMI;

//Saida BR
wire [31:0] saidaBR1;
wire [31:0] saidaBR2;

//Saida Extensor 16 Bits
wire [31:0] saidaExtensor;

//Saida Mux1
wire [31:0] saidaMux1;

//Saida Mux2
wire [31:0] saidaMux2;

//Saida Mux3
wire [31:0] saidaMux3;

//Saida Mux4
wire [31:0] saidaMux4;

//Saida Mux5
wire [31:0] saidaMux5;

//Saida Mux6
wire [31:0] saidaMux6;

//Saida Mux Instrucao
wire [31:0] saidaMuxInstrucao;

//Saida And
wire out_and;

//Saida Controlador HD
wire [15:0] endereco_leitura_HD;
wire [15:0] endereco_escrita_MI;

//Saida HD
wire [31:0] instrucao_escrita_MI;

//Saida Offset MI
wire [31:0] saidaOffsetMI;

//Saida Offset MI
wire [31:0] saidaOffsetMD;

//Saida Temporizador
wire saidaQuantumEnd;

//Saida UC
wire [4:0] selecao_UC_ULA;
wire Escreve_UC_Reg;
wire OrigALU_UC;
wire RegDst_UC;
wire Escreve_UC_MD;
wire Le_UC_Mem;
wire MemPara_UC_Reg;
wire Branch_UC;
wire [1:0] LeEntrada_UC;
wire JumpReg_UC;
wire Load_UC_Prog;
wire Escreve_UC_MI;
wire Set_Offset_MI_UC;
wire Set_Offset_MD_UC;
wire Halt_UC;
wire Set_Quantum_UC;


	Offset_MI Offset_MI(.Clock(clock), .Set_Offset_MI(Set_Offset_MI_UC), .Halt(Halt_UC), .saida(saidaOffsetMI));
	
	Projeto_PC PC(.clock(clock), 
					  .PC(saidaMux6), 
					  .Offset(saidaOffsetMI), 
					  .load(out_and), 
					  .reset(reset), 
					  .LeEntrada(LeEntrada_UC), 
					  .aceitaFlagEntrada(aceitaFlagEntrada), 
					  .carregando(carregandoPrograma), 
					  .saidaFlagEntrada(saidaFlagEntrada), 
					  .saida(saidaPC), 
					  .saidaContadorPC(saidaContagemPC),
					  //.saidaTeste(saidaTestePC)
					  );
	
	AndGate And(.a(Branch_UC), .b(zero), .y(out_and));
	
	Projeto_Mux Mux_4(.Entrada1(saidaPC), .Entrada2(saidaExtensor), .Select(out_and), .Saida(saidaMux4));
	
	Projeto_Mux Mux_6(.Entrada1(saidaMux4), .Entrada2(saidaBR1), .Select(JumpReg_UC), .Saida(saidaMux6));
	
	Controlador_HD Controlador_HD(.Clock(clock_slow), .Reset(reset), .Load_from_HD(Load_UC_Prog), .indice_programa(saidaMux5), .carregando(carregandoPrograma), .endereco_HD(endereco_leitura_HD), .endereco_RAM(endereco_escrita_MI));
	
	Projeto_HD HD(.clk(clock_fast), .addr(endereco_leitura_HD), .q(instrucao_escrita_MI));
	
	Temporizador Temporizador(.Clock(clock), .Reset(reset), .quantum_start(Set_Quantum_UC), .flag_entrada(LeEntrada_UC), .Halt(Halt_UC), .quantum_end(saidaQuantumEnd), .ativo(FlagQuantumAtivo));
	
	Projeto_MI MI(.read_clock(clock_fast), .write_clock(clock_slow), .data(instrucao_escrita_MI), .Offset(saidaOffsetMI), .read_addr(saidaPC), .write_addr(endereco_escrita_MI), .write(carregandoPrograma), .saida(saidaMI), .saidaTeste(testeHD));
	
	Projeto_Mux Mux_Saida_Instrucao(.Entrada1(saidaMI), .Entrada2(32'b10001011001110000000000000000000), .Select(saidaQuantumEnd), .Saida(saidaMuxInstrucao));
	
	Projeto_Mux #(.DATA_WIDTH(5)) Mux_1(.Entrada1(saidaMuxInstrucao[20:16]), .Entrada2(saidaMuxInstrucao[15:11]), .Select(RegDst_UC), .Saida(saidaMux1));
	
	Projeto_Mux_Entrada Mux_5(.Entrada1(saidaMux3), .Entrada2(userEntrada), .Entrada3(saidaPC), .Select(LeEntrada_UC), .Halt(Halt_UC), .Saida(saidaMux5));
	
	Projeto_BR BR(.clock(clock), 
					  .writeRegs(Escreve_UC_Reg), 
					  .dados_escrita(saidaMux5), 
					  .endereco_escrita(saidaMux1), 
					  .endereco_leitura1(saidaMuxInstrucao[25:21]), 
					  .endereco_leitura2(saidaMuxInstrucao[20:16]), 
					  .leituraRS(saidaBR1), 
					  .leituraRT(saidaBR2), 
					  .leituraRD(saidaBR));
	
	Projeto_Extensor_16 Extensor(.entrada(saidaMuxInstrucao[15:0]), .saida(saidaExtensor));
	
	Projeto_Mux Mux_2(.Entrada1(saidaExtensor), .Entrada2(saidaBR2), .Select(OrigALU_UC), .Saida(saidaMux2));
	
	Projeto_ULA ULA(.selecao(selecao_UC_ULA), .X(saidaBR1), .Y(saidaMux2), .Resultado(saidaULA), .Zero(zero));
	
	Offset_MD Offset_MD(.Clock(clock), .ID_Programa(saidaULA), .Set_Offset_MD(Set_Offset_MD_UC), .saida(saidaOffsetMD));
	
	Projeto_MD MD(.write_clock(clock), .read_clock(clock_fast), .data(saidaBR2), .Offset(saidaOffsetMD), .addr(saidaULA), .write(Escreve_UC_MD), .read(Le_UC_Mem), .saida(saidaMD));
	
	Projeto_Mux Mux_3(.Entrada1(saidaULA), .Entrada2(saidaMD), .Select(MemPara_UC_Reg), .Saida(saidaMux3));
	
	Projeto_UC UC(
	
					  .OP(saidaMuxInstrucao[31:26]), 
					  .EscreveReg(Escreve_UC_Reg), 
					  .selecaoULA(selecao_UC_ULA), 
					  .OrigALU(OrigALU_UC), 
					  .RegDst(RegDst_UC), 
					  .EscreveMem(Escreve_UC_MD), 
					  .LeMem(Le_UC_Mem),
					  .MemParaReg(MemPara_UC_Reg),
					  .Branch(Branch_UC),
					  .LeEntrada(LeEntrada_UC),
					  .JumpReg(JumpReg_UC),
					  .LoadProg(Load_UC_Prog),
					  .EscreveMI(Escreve_UC_MI),
					  .SetLCD(setLCD),
					  .SetOffsetMI(Set_Offset_MI_UC),
					  .SetOffsetMD(Set_Offset_MD_UC),
					  .Halt(Halt_UC),
					  .SetQuantum(Set_Quantum_UC)
					  );
	

endmodule