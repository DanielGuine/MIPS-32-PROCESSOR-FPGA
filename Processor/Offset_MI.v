module Offset_MI (
	input Clock,
	input Set_Offset_MI,
	input Halt,
	output reg [31:0] saida
);

initial begin
	saida = 0;
end

always @(posedge Clock) begin
	if (Set_Offset_MI) begin
		saida <= 400;
	end
	else if(Halt) begin
		saida <= 0;
	end
end

endmodule
