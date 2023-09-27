module sensorSelector(start, clk, rst, endS, out, enable);
	input start,clk,rst;
	input [4:0] endS;
	output reg out;
	inout [31:0] enable;
	
	parameter idle = 0, identifie = 1;
	reg state, next;
	always @(posedge clk, negedge rst) begin
		if(!rst) state = idle;
		else state = next;
	end
	
	
	always@(endS, enable) begin
		case(state)
			idle: begin
				if(start) begin
					next = identifie;
					out = 5'b0;
				end
				else begin
					next = idle;
				end
			end
			identifie: begin
				if(endS == 5'b00000) begin
					out = enable[0];
				end
				else if(endS == 5'b00001) begin
					out = enable[1];
				end
				else if(endS == 5'b00010) begin
					out = enable[2];
				end
				else if(endS == 5'b00011) begin
					out = enable[3];
				end
				else if(endS == 5'b00100) begin
					out = enable[4];
				end
				else if(endS == 5'b00101) begin
					out = enable[5];
				end
				else if(endS == 5'b00110) begin
					out = enable[6];
				end
				else if(endS == 5'b00111) begin
					out = enable[7];;
				end
				else if(endS == 5'b01000) begin
					out = enable[8];
				end
				else if(endS == 5'b01001) begin
					out = enable[9];
				end
				else if(endS == 5'b01010) begin
					out = enable[10];
				end
				else if(endS == 5'b01011) begin
					out = enable[11];
				end
				else if(endS == 5'b01100) begin
					out = enable[12];
				end
				else if(endS == 5'b01101) begin
					out = enable[13];
				end
				else if(endS == 5'b01110) begin
					out = enable[14];
				end
				else if(endS == 5'b01111) begin
					out = enable[15];
				end
				else if(endS == 5'b10000) begin
					out = enable[16];
				end
				else if(endS == 5'b10001) begin
					out = enable[17];
				end
				else if(endS == 5'b10010) begin
					out = enable[18];
				end
				else if(endS == 5'b10011) begin
					out = enable[19];
				end
				else if(endS == 5'b10100) begin
					out = enable[20];
				end
				else if(endS == 5'b10101) begin
					out = enable[21];
				end
				else if(endS == 5'b10110) begin
					out = enable[22];
				end
				else if(endS == 5'b10111) begin
					out = enable[23];
				end
				else if(endS == 5'b11000) begin
					out = enable[24];
				end
				else if(endS == 5'b11001) begin
					out = enable[25];
				end
				else if(endS == 5'b11010) begin
					out = enable[26];
				end
				else if(endS == 5'b11011) begin
					out = enable[27];
				end
				else if(endS == 5'b11100) begin
					out = enable[28];
				end
				else if(endS == 5'b11101) begin
					out = enable[29];
				end
				else if(endS == 5'b11110) begin
					out = enable[30];
				end
				else if(endS == 5'b11111) begin
					out = enable[31];
				end
				else begin
					out = 0;
				end
			end
		endcase
	end
endmodule