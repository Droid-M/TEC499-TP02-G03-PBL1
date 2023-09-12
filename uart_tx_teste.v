module uart_tx_teste (
  input wire clk,
  input wire rst,
  input wire tx_en,
  input wire [7:0] tx_data,
  output reg tx_out
);

  localparam IDLE = 0;
  localparam START = 1;
  localparam DATA = 2;
  localparam STOP = 3;

  reg [2:0] state;
  reg [7:0] tx_byte;
  reg [7:0] tx_bit;

  always @(posedge clk or posedge rst) begin
    if (rst) begin
      state <= IDLE;
      tx_byte <= 0;
      tx_bit <= 0;
    end else begin
      case (state)
        IDLE: begin
          if (tx_en) begin
            state <= START;
            tx_byte <= tx_data;
            tx_bit <= 0;
          end
        end
        START: begin
          state <= DATA;
        end
        DATA: begin
          if (tx_bit == 8) begin
            state <= STOP;
          end else begin
            tx_out <= tx_byte[tx_bit];
            tx_bit <= tx_bit + 1;
          end
        end
        STOP: begin
          if (tx_en) begin
            state <= IDLE;
          end
        end
      endcase
    end
  end

endmodule