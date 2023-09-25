module control_DHT11(
	input wire [39:0] DHT11_out,
	input [2:0] command,
	input clk,
	output reg [7:0] out1
);
	
	//ESTADOS DA MÁQUINA
	localparam SENSORSTATE = 3'b000,
	TEMPERATURE = 3'b001,
	HUMIDITY = 3'b010;
	
	always @(posedge clk) begin

	case (command)
			SENSORSTATE:
				if (DHT11_out == 8'b11111111) begin
					out1 <= 8'b00011111;
				end
				
				else begin
					out1 <= 8'b00000111;
				end
				
			
			TEMPERATURE: begin
				out1 <= DHT11_out[39:32];
			end
			
			HUMIDITY: begin
				out1 <= DHT11_out[23:16];
			end
			
			default: begin out1 <= 8'b11111111;
			end
		endcase
	end
		
endmodule
