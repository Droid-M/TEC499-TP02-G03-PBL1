module protocol_req(
	input clk,
	input reset,
	input [7:0] in,
	output reg [7:0] out,
	output valid
);

// Registradores para armazenar o pacote de requisição
	reg [7:0] header_req;
	reg [7:0] sensor_address_req;
	reg [7:0] command_req;
	reg [7:0] footer_req;
  
// Estados da máquina de estados
	localparam IDLE = 3'b000,
	REQ_HEADER = 3'b001,
	REQ_SENSOR_ADDRESS = 3'b0010,
	REQ_COMMAND = 3'b011,
	REQ_FOOTER = 3'b100,
	DONE = 3'b101;
	
// Registrador que guarda o valor do estado atual
	reg [2:0] state;
	
// Sinal indica se o pacote de requisição está completo
	reg req_valid;

// Controle da transição de estados
	always @(posedge clk or posedge reset) begin
		if (reset) begin
			state <= IDLE;
			req_valid <= 0;
		end else begin
			case (state)
				IDLE: begin
						if (in == 8'b11111111) begin
							state <= REQ_HEADER;
						end else begin
							state <= IDLE;
						end
				end

				REQ_HEADER: begin
					header_req <= in;
					state <= REQ_SENSOR_ADDRESS;
				end

				REQ_SENSOR_ADDRESS: begin
					sensor_address_req <= in;
					state <= REQ_COMMAND;
				end

				REQ_COMMAND: begin
					command_req <= in;
					state <= REQ_FOOTER;
					out <= command_req;
				end

				REQ_FOOTER: begin
					footer_req <= in;
					if (footer_req == 8'h7F) begin
						req_valid <= 1;
						state <= IDLE;
					end else begin
						state <= DONE;
						req_valid <= 1;
					end
				end
				
				default: begin out<= 8'b1; 
				end
			endcase
		end
	end
	
endmodule
