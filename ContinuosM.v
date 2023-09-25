module ContinuosM(enable,clk, doneTx, rst, dataDHT, out_loop, data_send);
	input clk, rst, enable, doneTx;
	input [7:0] dataDHT;
	output reg [7:0] out_loop;
	output data_send; 
	
	reg data_send = 1'b0; //confirma para o transmisso que a data pode ser enviada
	reg[13:0] count = 14'b10011100010000; // contando 10 segundos
	
	
	always @(posedge clk, negedge rst) begin
		if(!rst) begin
			//sensoriamento continuo é ativado quando enable é 0;
			if((doneTx) & (!enable)) begin //checa se o Tx não esta enviando nada e se o sensoriamento continuo esta ativado
				if(count != 14'b10011100010000) begin // enquanto 10 segundos não tiver passado incremente em 1
					data_send = 1'b0;
					count = count - 1'b1;
				end
				else begin // passado 10 segundos
					out_loop <= dataDHT; //Passando valor lido pelo DHT
					data_send = 1'b1; // ativando envio de dados do TX
					count = 14'b10011100010000;
				end
			end
		end
		else begin // se reset for clicado zera tudo
			count = 14'b10011100010000;
			data_send = 1'b0;
		end
	end
	
endmodule