module protocol (
  input clk,
  input reset,
  input [7:0] in,
  output [7:0] out,
  output valid,
  output ready
);
  // Registradores para armazenar o pacote de requisição
  reg [7:0] header_req;
  reg [7:0] sensor_address_req;
  reg [7:0] command_req;
  reg [7:0] footer_req;

  // Registradores para armazenar o pacote de resposta
  reg [7:0] header_resp;
  reg [7:0] status_resp;
  reg [7:0] sensor_address_resp;
  reg [7:0] command_resp;
  reg [23:0] data_resp;
  reg [7:0] footer_resp;
  reg [3:0] state;

  // Estados do protocolo
 
    localparam IDLE = 4'b0000,
    REQ_HEADER = 4'b0001,
    REQ_SENSOR_ADDRESS = 4'b0010,
    REQ_COMMAND = 4'b0011,
    REQ_FOOTER = 4'b0100,
    RESP_HEADER = 4'b0101,
    RESP_STATUS = 4'b0110,
    RESP_SENSOR_ADDRESS = 4'b0111,
    RESP_COMMAND = 4'b1000,
    RESP_DATA = 4'b1001,
    RESP_FOOTER = 4'b1010;
 


  // Sinal indica se o pacote de requisição está completo
  reg req_valid;

  // Sinal indica se o pacote de resposta está completo
  reg resp_valid;

  always @(posedge clk or posedge reset) begin
    if (reset) begin
      state <= IDLE;
      req_valid <= 0;
      resp_valid <= 0;
    end else begin
      case (state)
        IDLE: begin
          if (in == 8'hFF) begin
            state <= REQ_HEADER;
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
        end

        REQ_FOOTER: begin
          footer_req <= in;

          if (footer_req == 8'h7F) begin
            req_valid <= 1;
            state <= IDLE;
          end else begin
            req_valid <= 0;
          end
        end

        RESP_HEADER: begin
          header_resp <= in;
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
        end
      endcase
    end
  end

  assign out = state == RESP_DATA ? data_resp[7:0] : 8'bz;
  assign valid = state == RESP_DATA ? 1'b1 : 1'b0;
  assign ready = state == IDLE ? 1'b1 : 1'b0;

endmodule