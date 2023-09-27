module control(
	input clk,
	input [39:0] DHTdata, 
	input DHTdone, 
	input DHTError, 
	input [2:0] command,
	output reg DoneCtrol, 
	output reg [7:0] data,
	output reg contEn);
	
	reg cont;
	reg state;
	localparam com = 0, dt = 1;
	
	
	always @(posedge clk) begin
		if (DHTdone) begin
			case (command)
				3'b000: begin 
					if (DHTError) begin 
						data <= 8'b00011111;
					end else begin data <= 8'b00000111; 
					end
				end
				3'b001: begin data <= DHTdata[39:32];	
					contEn <= 0;
				end
				3'b011: begin data <= DHTdata[23:16]; 
					contEn <= 0;
				end
				3'b110: begin data <= DHTdata[39:32];	
					contEn <= 1;
				end
				3'b111: begin data <= DHTdata[23:16];
					contEn <= 1;
				end
			endcase
			DoneCtrol <= 1;
		end else begin
			contEn <= 0;
			data <= 8'b0;
			DoneCtrol <= 0;
		end
		
	end
	
	
endmodule
