module uart_transmitter (
    input wire clk,
    input wire rst,
    input wire start_tx, // Sinal de início de transmissão
    input wire [7:0] data, // Dados a serem transmitidos (8 bits)
    output wire tx // Sinal de saída transmitido
);

reg [3:0] state; // Registrador de estado (4 bits)
reg [2:0] count; // Registrador de contagem (3 bits)
reg tx_reg; // Registrador de transmissão
wire tx_idle = (state == 4'b0000); // Sinal indicando estado IDLE

assign tx = tx_reg; // Atribui o valor do registrador de transmissão ao sinal de saída "tx"

always @(posedge clk or posedge rst) begin
    if (rst) begin
        state <= 4'b0000; // Reinicia o estado para IDLE
        count <= 3'b0; // Reinicia a contagem
        tx_reg <= 1'b1; // Define o sinal de transmissão para alto (1) ao reiniciar
    end else begin
        case (state)
            4'b0000: begin // IDLE
                if (start_tx) // Se o sinal de início de transmissão for ativado
                    state <= 4'b0001; // Muda para o próximo estado (START)
            end
            4'b0001: begin // START
                count <= 3'b0; // Inicializa a contagem
                tx_reg <= 1'b0; // Define o sinal de transmissão para baixo (0) para indicar o início da transmissão
                state <= 4'b0010; // Muda para o próximo estado (DATA)
            end
            4'b0010: begin // DATA
                if (count < 3'b111) begin // Enquanto ainda não transmitiu todos os bits
                    tx_reg <= data[count]; // Transmite o bit atual
                    count <= count + 1; // Incrementa a contagem
                end else begin // Quando todos os bits forem transmitidos
                    state <= 4'b0011; // Muda para o próximo estado (STOP)
                end
            end
            4'b0011: begin // STOP
                tx_reg <= 1'b1; // Define o sinal de transmissão para alto (1) para indicar o bit de parada
                state <= 4'b0000; // Retorna ao estado IDLE após a transmissão
            end
        endcase
    end
end

endmodule
