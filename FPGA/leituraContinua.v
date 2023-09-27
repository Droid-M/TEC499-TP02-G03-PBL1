module leituraContinua(clk, rst, doneCtrl, data_in ,En, Dados, done, contEn);
	input clk, rst, En, doneCtrl;
	input  [7:0] data_in;
	output reg [7:0] Dados;
	output done, contEn;
	
	reg done = 1'b0; // variavel que identifica termino de envio
	reg contEn = 1'b0; // variavel de ativação do continuo 
	reg [7:0] buffer;
	
	//logica de mudança de estados
	parameter idle = 1'b0, loop = 1'b1;
	reg state, next;
	always @(posedge clk, negedge rst) begin
		if(!rst) state <= idle;
		else state <= next;
	end
	
	
	
	reg [1:0] countTime = 2'b00; // contador de segundos
	//divisor de frequencia para 1MHz para contar 4 segundos
	reg [5:0] contador_universal = 0;
	reg clk1_mhz;
	always @(posedge clk) begin 
		if (contador_universal < 50) begin
			contador_universal = contador_universal+1;
			clk1_mhz = 0;
		end
		else begin
			contador_universal = 0;
			clk1_mhz = 1;
		end
	end
	
	
	
	//Transição sensivel a mudança do enable e do fim do envio do modulo de controle
	always @(En, doneCtrl) begin
		case(state) 
			idle: begin
				if(En & doneCtrl) begin //As duas variaveis em nivel logico alto
					next <= loop; // mandam para o estado de envio continuo
					done <= 1'b0; // indica que esta enviando
				end
				else begin // se não fica no mesmo estado 
					next <= idle;
					done <= 1'b0;
				end
			end
			loop: begin
				if(!doneCtrl) begin //em processo de envio
					if(countTime == 2'b11) begin
						countTime <= countTime + 1'b1; // conta 4 segundos
					end
					else begin
						done <= 1'b1;
						contEn <= En;
						countTime <= 2'b00;
						buffer <= data_in; // envia os dados
					end
				end
				
				else begin
					next <= idle;
				end
			end
		endcase
	end




endmodule