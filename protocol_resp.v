module protocol_resp(
	input clk,
	input wire [7:0] command,
	input wire [7:0] in,
	output reg [7:0] out
);
	reg [2:0] state;
	
	localparam IDLE = 3'b000,
	RESP_HEADER = 3'b001,
	RESP_STATUS = 3'b010,
	RESP_SENSOR_ADDRESS = 3'b011,
	RESP_COMMAND = 3'b100,
	RESP_DATA = 3'b101,
	RESP_FOOTER = 3'b110;
	
	always @(posedge clk or posedge reset) begin
		if (reset) begin
			state <= IDLE;
		end else begin
// Response bytes
			case(state)
				RESP_HEADER: begin
				header_resp <= 8'b11111111;
				state <= RESP_STATUS;
			end

			RESP_STATUS: begin
				status_resp <= in;
				state <= RESP_SENSOR_ADDRESS;
			end

			RESP_SENSOR_ADDRESS: begin
				sensor_address_resp <= in;
				state <= RESP_COMMAND;
			end

			RESP_COMMAND: begin
				command_resp <= in;
				state <= RESP_DATA;
			end

			RESP_DATA: begin
				data_resp <= {data_resp[15:0], in};
				state <= RESP_FOOTER;
			end

			RESP_FOOTER: begin
				footer_resp <= in;

				if (footer_resp == 8'h7F) begin
					resp_valid <= 1;
					state <= IDLE;
				end else begin
					resp_valid <= 0;
				end
			endcase
		end
	end
        
		  
endmodule
