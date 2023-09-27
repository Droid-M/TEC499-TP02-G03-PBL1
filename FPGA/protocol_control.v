module protocol_control(
	input RxDone, 
	input [7:0] RxData, 
	input clk,
	input rst,
	output reg done,
	output reg[4:0] sensor_adress,
	output reg [2:0] data
	);
	
// Estados da máquina
	localparam IDLE = 2'b00,
	SENSOR = 2'b01,
	COMMAND = 2'b10,
	SEND = 2'b11;
	
// Registradores 
	reg [1:0] state;
	reg [2:0] command;
	reg [4:0] sensor;
	
// always sensível a borda de subida do clk
	always @(posedge clk)begin
		if (rst) begin // rst = 1 -> estado = IDLE
			state <= IDLE;
		end else begin
			case (state)
				IDLE: begin
					if (rst) begin //rst = 1 -> estado = IDLE
						state <= IDLE;
					end else begin state <= SENSOR; end //transiçãp para o sendor
				end
				SENSOR: begin
				// salvando o endereço no registrador sensor
					done = 1'b0;
					if (RxDone) begin // se a transmissão for finalizada, muda de estado
						state <= COMMAND;
						sensor <= RxData[4:0];
					end else begin state <= SENSOR; end // se a transmissão não for finalizada, estado se mantém.
				end
				COMMAND: begin // se a transmissão for finalizada há transição de estados
					if (RxDone) begin
						state <= SEND;
						command <= RxData[2:0];
					end else begin state <= COMMAND; end //se a transmissão não for finalizada, estado se mantém
				end
				SEND: begin
				// saida do endereço e o comando;
					data <= command;
					done <= 1'b1;
					sensor_adress <= sensor;
				end
			endcase
		end
	
	end
	
	
endmodule
