module responseCounter(TxD,doneCtr, doneAux, resp, dado, inEn, cntEn, done, clk, rst, out);

	input clk, rst, doneCtr,doneAux, inEn,TxD;
	input [7:0] resp, dado;
	output reg [7:0] out;
	output reg done, cntEn;
	
	parameter [1:0] idle = 2'b00, r0 = 2'b01, sleep = 2'b10, r1 = 2'b11;
	reg [1:0] state, next;
	
	
	//logica de transição de estados
	always @(posedge clk, negedge rst) begin
		if(!rst) state = idle;
		else state = next;
	end
	//maquina de estados sensiveis a finalização do controle e do auxiliar e de suas respostas
	always@(doneCtr, doneAux, resp, dado) begin
		case(state)
			idle: begin
				if(doneAux & doneCtr) begin // ao receber os dois arquivos completos
					next <= r0; // vai para o estado de envio do comando de resposta
					done <= 1'b0;
				end
				else begin
					next <= idle;
				end
			end
			r0:begin
				out <= resp; //pasa a resposta como saida
				done <= 1'b1; //sinaliza que terminou de enviar
				next <= sleep; // vai para o estado de espera
			end
			sleep: begin
				if(TxD) begin // verifica se a transmissão do primeiro byte terminou
					done <= 1'b0;
					next <= r1; //vai para o envio do byte de dados
				end
				else begin
					done <= 1'b0;
					next = sleep;
				end
			end
			r1: begin
				out <= dado; //envia o byte de dados
				done <= 1'b1;//sinaliza que terminou
				cntEn <= inEn;
				next = idle;
			end
		endcase
	end
	
endmodule