module control_aux(clk, rst, Dado, done, En, resp, sensorState);
	input clk, rst, En, sensorState;
	input [2:0] Dado;
	output reg done;
	output reg [7:0] resp;
	
	
	
	//logica de transição de estados
	parameter [2:0] status = 3'b000, temp = 3'b010, hum = 3'b011, tempC = 3'b110, humC = 3'b111, idle = 3'b101;
	reg [2:0] state, next;
	
	
	always @(posedge clk, negedge rst) begin
		if(!rst) state = idle;
		else state = next;
	end
	
	
	//verifica a mudança no sensoriamento continuo
	wire [1:0] Dedge;
	reg [1:0] Redge;
	always @(Dado) begin
		if(!rst) begin
			Redge = 2'b00;
		end
		else begin
			Redge = {Redge[0], Dado[2]}; //ao comparar o bit de sensoriamento com o bit normal
		end
	end
	assign Dedge = !Redge[0] & !Redge[1]; //verifica quando há o desligamento
	
	
	//maquina de estado sensivel as entradas
	always @(En, done, Dado) begin
		case(state) 
			idle: begin
				if(En & done) begin //Habilitado o recebimento e não estando enviando nada 
					if(Dado == 3'b000) begin // verifica se a requisição
						done <= 1'b0; //começa a tratar os dados
						next <= status; //vai para o estado da requisição		
					end
					else if(Dado == 3'b010) begin
						done <= 1'b0;
						next <= temp;
					end
					else if(Dado == 3'b011) begin
						done <= 1'b0;
						next <= hum;
					end
					else if(Dado == 3'b110) begin
						done <= 1'b0;
						next <= tempC;
					end
					else if(Dado == 3'b111) begin
						done <= 1'b0;
						next <= humC;
					end
					else begin
						next <= idle;// se não vai para o estado idle
					end
				end
				else begin
					next = idle;
				end
			end
			status: begin //passando o protocolo de reposta
				if(!done) begin
					if(sensorState) begin
						resp <= 8'b00011111;
					end
					else begin
						resp <= 8'b00000111;
					end
				end
				done <= 1;
				next = idle;
			end
			temp: begin
				if(!done) begin
					resp <= 8'b00001001;
				end
				done <= 1;
				next = idle;
			end
			hum: begin
				if(!done) begin
					resp <= 8'b00001000;
				end
				done <= 1;
				next = idle;
			end
			tempC: begin
				if(!done) begin
					if(Dedge) begin //no caso de esta no modo continuo verifica se houve alteração na borda
						resp <= 8'b00001010;
					end
					else begin
						resp <= 8'b00001100;
					end
				end
				done <= 1;
				next = idle;
			end
			humC: begin
				if(!done) begin
					if(Dedge) begin
						resp <= 8'b00001011;
					end
					else begin
						resp <= 8'b00001101;
					end
				end
				done <= 1;
				next <= idle;
			end
			default: next <= idle;
		endcase
	end


endmodule