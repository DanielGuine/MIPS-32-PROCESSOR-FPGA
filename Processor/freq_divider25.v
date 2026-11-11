module freq_divider25(
	input wire clk_fast,
	output reg out
);
	reg [24:0] count;
	reg toggle;
	
	always @(posedge clk_fast)
	begin
		if (count == 781250) // 25000000
		begin
			count <= 0;
			toggle <= ~toggle;
			out <= toggle;
		end
		else
		begin
			count <= count + 1;
		end
	end
endmodule