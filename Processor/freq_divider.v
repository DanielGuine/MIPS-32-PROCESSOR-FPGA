module freq_divider(
	input wire clk_fast,
	input slow,
	output reg out
);
	reg [24:0] count;
	reg toggle;
	
	always @(posedge clk_fast)
	begin
		if (slow == 1) begin
			if (count == 12500000) // 25000000
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
		else begin
			if (count == 390625) // 390625
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
		
	end
endmodule