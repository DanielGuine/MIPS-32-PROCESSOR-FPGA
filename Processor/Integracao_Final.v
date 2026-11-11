module Integracao_Final(SW, V_BT, G_CLOCK_50, IRDA_RXD, UART_TXD, LEDR, LEDG, HEX0, HEX1, HEX2, HEX3, HEX4, HEX5, HEX6, HEX7, LCD_DATA, LCD_RS, LCD_RW, LCD_EN, LCD_ON, LCD_BLON);
 
input [17:0] SW;
input [3:0] V_BT;
input G_CLOCK_50;
input IRDA_RXD;
output UART_TXD;
output [0:9] LEDR;
output [0:7] LEDG;
output [0:6] HEX0;
output [0:6] HEX1;
output [0:6] HEX2;
output [0:6] HEX3;
output [0:6] HEX4;
output [0:6] HEX5;
output [0:6] HEX6;
output [0:6] HEX7;
inout [7:0] LCD_DATA;
output LCD_RS;
output LCD_RW;
output LCD_EN;
output LCD_ON;
output LCD_BLON;

// Saida Divisor de Frequência
wire out_clock;

// Saida CPU
wire [0:31] saidaCPU;

// Saida Extensor
wire [0:31] saidaExtensor;

// Saida Halt
wire SaidaHalt;

// Saida da flag que acende o LED verde indicando que pode ser inserida uma entrada
wire SaidaFlagEntrada;

// Saida da contagem do PC
wire [0:31] saidaContagemPC;

// Saida Flag carregandoPrograma para pausar o PC
wire saidaCarregandoPrograma;

// Saida Flag para permitir escrita no LCD
wire setLCD;

// Saida clock lento
wire clk_slow;

wire [31:0] teste;

wire FlagQuantumAtivo;

// Saidas do IR
wire ir_ready_pulse;
wire [31:0] ir_data_full;
wire [7:0] ir_key_code = ir_data_full[23:16];

wire [31:0] entrada_para_cpu;

reg [0:9] LEDR;
reg [0:7] LEDG;
reg [0:6] HEX0;
reg [0:6] HEX1;
reg [0:6] HEX2;
reg [0:6] HEX3;
reg [0:6] HEX4;
reg [0:6] HEX5;
reg [0:6] HEX6;
reg [0:6] HEX7;
reg [0:31] count;
reg [0:31] hundreds;  // Dígito das centenas saida (0-31)
reg [0:31] tens;  // Dígito das dezenas saida (0-31)
reg [0:31] unit;  // Dígito das unidades saida (0-31)
reg [0:31] hundreds_c;  // Dígito das centenas contador (0-31)
reg [0:31] tens_c;  // Dígito das dezenas contador (0-31)
reg [0:31] unit_c;  // Dígito das unidades contador (0-31)
reg [0:31] tens_e;  // Dígito das dezenas entrada (0-31)
reg [0:31] unit_e;  // Dígito das unidades entrada (0-31)
reg [0:14] entradaUser;
reg [31:0] porta_ir;


always @ (posedge out_clock)
begin
    entradaUser = { SW[14:1] };
end
	 
IR_RECEIVE receptor_ir (
   .iCLK(G_CLOCK_50),
   .iRST_n(~SW[0]),
   .iIRDA(IRDA_RXD),
   .oDATA_READY(ir_ready_pulse),
   .oDATA(ir_data_full)
);

always @(posedge G_CLOCK_50) begin
    if (SW[0]) begin 
        porta_ir <= 32'd0; // Reseta a porta
    end 
    else if (ir_ready_pulse) begin
        // Quando recebe um sinal válido salva a tecla
        porta_ir <= {23'd0, 1'b1, ir_key_code}; 
    end
    else if (V_BT[1] == 1'b0) begin 
        porta_ir[8] <= 1'b0; 
    end
end

assign entrada_para_cpu = (SW[17]) ? porta_ir : saidaExtensor;

// Logica repetiçao do botao
reg ir_ready_anterior;
reg [24:0] tempo_inatividade; // Timer de espera para identificar repetiçao (~300ms)
reg [24:0] timer_repeticao;   // Timer para gerar cliques a cada 100ms
reg btn_ir_simulado;          // Autoriza entrada na CPU
reg pulso_repeticao;          // Autoriza mandar para a UART
reg [7:0] ultima_tecla_valida; // Guarda a última tecla decodificada do IR

always @(posedge G_CLOCK_50) begin
    if (SW[0]) begin // Reset
        tempo_inatividade <= 25'd0;
        timer_repeticao <= 25'd0;
        btn_ir_simulado <= 1'b0;
        pulso_repeticao <= 1'b0;
        ir_ready_anterior <= 1'b0;
        ultima_tecla_valida <= 8'd0;
    end
    else begin
        ir_ready_anterior <= ir_ready_pulse;
        pulso_repeticao <= 1'b0;

        if (ir_ready_pulse) begin
            tempo_inatividade <= 25'd15000000; // Define inatividade para 300ms
            timer_repeticao <= 25'd5000000;    // Inicia um ciclo de repetição de 100ms
            btn_ir_simulado <= 1'b1;
            ultima_tecla_valida <= ir_key_code; // Trava a repetição nesta tecla específica
        end 
        else if (tempo_inatividade > 0) begin
            // Janela de repetiçao
            if (IRDA_RXD == 1'b0) begin
                // Atividade no pino renova a janela de repetição
                tempo_inatividade <= 25'd15000000;
            end else begin
                tempo_inatividade <= tempo_inatividade - 1'b1;
            end

            // Conta o tempo para disparar o próximo pulso automático
            if (timer_repeticao > 0) begin
                timer_repeticao <= timer_repeticao - 1'b1;
            end else begin
                timer_repeticao <= 25'd5000000; // Reinicia o timer de 100ms
                pulso_repeticao <= 1'b1;        // Dispara a UART no auto-repeat
            end

            // Mantém o botão da CPU apertado nos primeiros 20ms do ciclo
            if (timer_repeticao > 25'd4000000) begin
                btn_ir_simulado <= 1'b1;
            end else begin
                btn_ir_simulado <= 1'b0;
            end
        end 
        else begin
            timer_repeticao <= 25'd0;
            btn_ir_simulado <= 1'b0;
        end
    end
end

// Gatilho final da UART
wire ir_gatilho_uart = (ir_ready_pulse & ~ir_ready_anterior) | pulso_repeticao;

// Fio que decide o que vai para a UART:
// Se for o momento exato do clique, pega direto do decodificador.
// Se for repetição automática, pega a tecla salva.
wire [7:0] tecla_para_uart = (ir_ready_pulse) ? ir_key_code : ultima_tecla_valida;

wire tx_busy;

uart modulo_uart_esp32 (
    .clk_50m(G_CLOCK_50),      
    .data_in(tecla_para_uart), 
    
    // Dispara a UART
    .Tx_en(~ir_gatilho_uart),   
    
    .Tx(UART_TXD),             
    .Tx_busy(tx_busy),         
    
    // Lado da Recepção (Desativado)
    .Rx(1'b1),                 
    .Rx_en(1'b1),              
    .ready(),                  
    .ready_clr(1'b0),          
    .data_out(),               
    
    .LEDR(), 
    .Tx2()
);

wire autorizacao_cpu;
assign autorizacao_cpu = (SW[17]) ? ~btn_ir_simulado : V_BT[0];

Clock_Halt_Control Halt(.clk_50MHz(G_CLOCK_50), .halt(SW[16]), .clk_out(SaidaHalt));

//Modulo extensor da entrada 16bits para 32bits
Projeto_Extensor_16 Extensor(.entrada(entradaUser), .saida(saidaExtensor));

//Modulo Divisor de Frequencia
freq_divider freq_divider(.clk_fast(SaidaHalt), .slow(SW[15]), .out(out_clock));

freq_divider25 freq_divider25(.clk_fast(G_CLOCK_50), .out(clk_slow));

//Modulo Geral CPU
Projeto_CPU CPU(
							 .clock(out_clock),
							 .reset(SW[0]),
							 .userEntrada(entrada_para_cpu),
							 .testeMD(teste),
							 .clock_fast(G_CLOCK_50),
							 .clock_slow(clk_slow),
							 .aceitaFlagEntrada(autorizacao_cpu),
							 .saidaBR(saidaCPU),
							 .saidaFlagEntrada(SaidaFlagEntrada),
							 .saidaContagemPC(saidaContagemPC),
							 .carregandoPrograma(saidaCarregandoPrograma),
							 .setLCD(setLCD),
							 .FlagQuantumAtivo(FlagQuantumAtivo)
							 );

//Modulo Display LCD
Display_LCD Display_LCD(.clk_50MHz(G_CLOCK_50), .Rs(saidaCPU), .Set_LCD(setLCD), .LCD_ON(LCD_ON), .LCD_BLON(LCD_BLON), .LCD_RW(LCD_RW), .LCD_EN(LCD_EN), .LCD_RS(LCD_RS), .LCD_DATA(LCD_DATA));


// ----- Entrada e Saida ------
    always @ (*)
    begin
		  
			LEDG[0] = SaidaFlagEntrada;
			LEDG[1] = saidaCarregandoPrograma;
			LEDG[2] = FlagQuantumAtivo;
		

        hundreds_c = (saidaContagemPC / 100) % 10;
        tens_c = (saidaContagemPC / 10) % 10;;
        unit_c = saidaContagemPC % 10;
    
        
        // Decodificação do dígito das centenas contador
        case (hundreds_c)
            0: HEX7 = 7'b0000001;
            1: HEX7 = 7'b1001111;
            2: HEX7 = 7'b0010010;
            3: HEX7 = 7'b0000110;
            4: HEX7 = 7'b1001100;
            5: HEX7 = 7'b0100100;
            6: HEX7 = 7'b0100000;
            7: HEX7 = 7'b0001111;
            8: HEX7 = 7'b0000000;
            9: HEX7 = 7'b0001100; 
            default: HEX7 = 7'b1111111; 
        endcase
        
        // Decodificação do dígito das dezenas contador
        case (tens_c)
            32'B00000000000000000000000000000000: HEX6 = 7'B0000001;
            32'B00000000000000000000000000000001: HEX6 = 7'B1001111;
            32'B00000000000000000000000000000010: HEX6 = 7'B0010010;
            32'B00000000000000000000000000000011: HEX6 = 7'B0000110;
            32'B00000000000000000000000000000100: HEX6 = 7'B1001100;
            32'B00000000000000000000000000000101: HEX6 = 7'B0100100;
            32'B00000000000000000000000000000110: HEX6 = 7'B0100000;
            32'B00000000000000000000000000000111: HEX6 = 7'B0001111;
            32'B00000000000000000000000000001000: HEX6 = 7'B0000000;
            32'B00000000000000000000000000001001: HEX6 = 7'B0001100;
            default: HEX6 = 7'B1111111;
        endcase
        
        // Decodificação do dígito das unidades contador
        case (unit_c)
            32'B00000000000000000000000000000000: HEX5 = 7'B0000001;
            32'B00000000000000000000000000000001: HEX5 = 7'B1001111;
            32'B00000000000000000000000000000010: HEX5 = 7'B0010010;
            32'B00000000000000000000000000000011: HEX5 = 7'B0000110;
            32'B00000000000000000000000000000100: HEX5 = 7'B1001100;
            32'B00000000000000000000000000000101: HEX5 = 7'B0100100;
            32'B00000000000000000000000000000110: HEX5 = 7'B0100000;
            32'B00000000000000000000000000000111: HEX5 = 7'B0001111;
            32'B00000000000000000000000000001000: HEX5 = 7'B0000000;
            32'B00000000000000000000000000001001: HEX5 = 7'B0001100;
            default: HEX5 = 7'B1111111;
        endcase
        
        tens_e = (saidaExtensor / 10) % 10;;
        unit_e = saidaExtensor % 10;
        
        // Decodificação do dígito das dezenas entrada
        case (tens_e)
            32'B00000000000000000000000000000000: HEX4 = 7'B0000001;
            32'B00000000000000000000000000000001: HEX4 = 7'B1001111;
            32'B00000000000000000000000000000010: HEX4 = 7'B0010010;
            32'B00000000000000000000000000000011: HEX4 = 7'B0000110;
            32'B00000000000000000000000000000100: HEX4 = 7'B1001100;
            32'B00000000000000000000000000000101: HEX4 = 7'B0100100;
            32'B00000000000000000000000000000110: HEX4 = 7'B0100000;
            32'B00000000000000000000000000000111: HEX4 = 7'B0001111;
            32'B00000000000000000000000000001000: HEX4 = 7'B0000000;
            32'B00000000000000000000000000001001: HEX4 = 7'B0001100;
            default: HEX4 = 7'B1111111;
        endcase
        
        // Decodificação do dígito das unidades entrada
        case (unit_e)
            32'B00000000000000000000000000000000: HEX3 = 7'B0000001;
            32'B00000000000000000000000000000001: HEX3 = 7'B1001111;
            32'B00000000000000000000000000000010: HEX3 = 7'B0010010;
            32'B00000000000000000000000000000011: HEX3 = 7'B0000110;
            32'B00000000000000000000000000000100: HEX3 = 7'B1001100;
            32'B00000000000000000000000000000101: HEX3 = 7'B0100100;
            32'B00000000000000000000000000000110: HEX3 = 7'B0100000;
            32'B00000000000000000000000000000111: HEX3 = 7'B0001111;
            32'B00000000000000000000000000001000: HEX3 = 7'B0000000;
            32'B00000000000000000000000000001001: HEX3 = 7'B0001100;
            default: HEX3 = 7'B1111111;
        endcase
    end



    always @(saidaCPU) begin
        // Extrair o dígito das centenas, dezenas e unidades
		  hundreds = (saidaCPU / 100) % 10;
        tens = (saidaCPU / 10) % 10;
        unit = saidaCPU % 10;
    end
	 
	 // Decodificação do dígito das dezenas
    always @(*) begin
        case (hundreds)
            32'B00000000000000000000000000000000: HEX2 = 7'B0000001;
            32'B00000000000000000000000000000001: HEX2 = 7'B1001111;
            32'B00000000000000000000000000000010: HEX2 = 7'B0010010;
            32'B00000000000000000000000000000011: HEX2 = 7'B0000110;
            32'B00000000000000000000000000000100: HEX2 = 7'B1001100;
            32'B00000000000000000000000000000101: HEX2 = 7'B0100100;
            32'B00000000000000000000000000000110: HEX2 = 7'B0100000;
            32'B00000000000000000000000000000111: HEX2 = 7'B0001111;
            32'B00000000000000000000000000001000: HEX2 = 7'B0000000;
            32'B00000000000000000000000000001001: HEX2 = 7'B0001100;
            default: HEX2 = 7'B1111111;
        endcase
    end

    // Decodificação do dígito das dezenas
    always @(*) begin
        case (tens)
            32'B00000000000000000000000000000000: HEX1 = 7'B0000001;
            32'B00000000000000000000000000000001: HEX1 = 7'B1001111;
            32'B00000000000000000000000000000010: HEX1 = 7'B0010010;
            32'B00000000000000000000000000000011: HEX1 = 7'B0000110;
            32'B00000000000000000000000000000100: HEX1 = 7'B1001100;
            32'B00000000000000000000000000000101: HEX1 = 7'B0100100;
            32'B00000000000000000000000000000110: HEX1 = 7'B0100000;
            32'B00000000000000000000000000000111: HEX1 = 7'B0001111;
            32'B00000000000000000000000000001000: HEX1 = 7'B0000000;
            32'B00000000000000000000000000001001: HEX1 = 7'B0001100;
            default: HEX1 = 7'B1111111;
        endcase
    end

    // Decodificação do dígito das unidades
    always @(*) begin
        case (unit)
            32'B00000000000000000000000000000000: HEX0 = 7'B0000001;
            32'B00000000000000000000000000000001: HEX0 = 7'B1001111;
            32'B00000000000000000000000000000010: HEX0 = 7'B0010010;
            32'B00000000000000000000000000000011: HEX0 = 7'B0000110;
            32'B00000000000000000000000000000100: HEX0 = 7'B1001100;
            32'B00000000000000000000000000000101: HEX0 = 7'B0100100;
            32'B00000000000000000000000000000110: HEX0 = 7'B0100000;
            32'B00000000000000000000000000000111: HEX0 = 7'B0001111;
            32'B00000000000000000000000000001000: HEX0 = 7'B0000000;
            32'B00000000000000000000000000001001: HEX0 = 7'B0001100;
            default: HEX0 = 7'B1111111;
        endcase
    end
    
// ----------------------------

endmodule