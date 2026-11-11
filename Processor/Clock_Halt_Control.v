module Clock_Halt_Control(
    input wire clk_50MHz,   // Clock de entrada de 50 MHz
    input wire halt,        // Sinal de halt para pausar o clock
    output wire clk_out     // Clock de saída com controle de halt
);

reg gated_clk;              // Clock interno, controlado pelo halt

// Sempre que houver borda positiva do clock
always @(posedge clk_50MHz) begin
    if (halt) begin
        gated_clk <= 0;    // Se halt for ativo, desativa o clock (mantém em 0)
    end else begin
        gated_clk <= ~gated_clk; // Se halt for inativo, continua alternando o clock
    end
end

assign clk_out = gated_clk;  // Atribui o clock controlado como saída

endmodule
