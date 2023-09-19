module DHT11(
	input wire       	clk			,  
	input wire 		  	start     ,
	input wire	     	rst_n		,
	inout	          	dat_io		,
	output  reg [39:0]	data     ,
	output  			error					,
	output  			done
);
	wire din;
	reg read_flag;
	reg dout;
	reg[3:0] state;
	localparam s1 = 0;
	localparam s2 = 1;
	localparam s3 = 2;
	localparam s4 = 3;
	localparam s5 = 4;
	localparam s6 = 5;
	localparam s7 = 6;
	localparam s8 = 7;
	localparam s9 = 8;
	localparam s10 = 9;
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
	assign dat_io = read_flag ? 1'bz : dout;
	assign din = dat_io;
	assign done = (state == s10)?1'b1:1'b0;
	assign error = (data[7:0] == data[15:8] + data[23:16] + data[31:24] + data[39:32])?1'b0:1'b1;
	reg [5:0]data_cnt;
	reg start_f1,start_f2,start_rising;
	always@(posedge clk1_mhz)
	begin
		if(!rst_n)begin
			start_f1 <=1'b0;
			start_f2 <= 1'b0;
			start_rising<= 1'b0;
		end
		else begin
			start_f1 <= start;
			start_f2 <= start_f1;
			start_rising <= start_f1 & (~start_f2);
		end
	end
	reg [39:0] data_buf;
	reg [15:0]cnt ;
	always@(posedge clk1_mhz or negedge rst_n)
	begin
		if(rst_n == 1'b0)begin
			read_flag <= 1'b1;
			state <= s1;
			dout <= 1'b1;
			data_buf <= 40'd0;
			cnt <= 16'd0;
			data_cnt <= 6'd0;
			data<=40'd0;
		end
		else begin
			case(state)
				s1:begin
				if(start_rising && din==1'b1)begin
							state <= s2;
							read_flag <= 1'b0;
							dout <= 1'b0;
							cnt <= 16'd0;
							data_cnt <= 6'd0;
						end
						else begin
							read_flag <= 1'b1;
							dout<=1'b1;
							cnt<=16'd0;
						end	
					end
				s2:begin
						if(cnt >= 16'd19000)begin
							state <= s3;
							dout <= 1'b1;
							cnt <= 16'd0;
						end
						else begin
							cnt<= cnt + 1'b1;
						end
					end
				s3:begin
						if(cnt>=16'd20)begin
							cnt<=16'd0;
							read_flag <= 1'b1;
							state <= s4;
						end
						else begin
							cnt <= cnt + 1'b1;
						end
					end
				s4:begin
						if(din == 1'b0)begin
							state<= s5;
							cnt <= 16'd0;
						end
						else begin
							cnt <= cnt + 1'b1;
							if(cnt >= 16'd65500)begin
								state <= s1;
								cnt<=16'd0;
								read_flag <= 1'b1;
							end	
						end
					end
				s5:begin
						if(din==1'b1)begin
							state <= s6;
							cnt<=16'd0;
							data_cnt <= 6'd0;
						end
						else begin
							cnt <= cnt + 1'b1;
							if(cnt >= 16'd65500)begin
								state <= s1;
								cnt<=16'd0;
								read_flag <= 1'b1;
							end								
						end
					end
				s6:begin
						if(din == 1'b0)begin
							state <= s7;
							cnt <= cnt + 1'b1;
						end
						else begin
							cnt <= cnt + 1'b1;
							if(cnt >= 16'd65500)begin
								state <= s1;
								cnt<=16'd0;
								read_flag <= 1'b1;
							end							
						end
					end
				s7:begin//
						if(din == 1'b1)begin
							state <= s8;
							cnt <= 16'd0;
						end
						else begin
							cnt <= cnt + 1'b1;
							if(cnt >= 16'd65500)begin
								state <= s1;
								cnt<=16'd0;
								read_flag <= 1'b1;
							end							
						end
					end
				s8:begin
						if(din == 1'b0)begin
							data_cnt <= data_cnt + 1'b1;
							state <= (data_cnt >= 6'd39)?s9:s7;
							cnt<=16'd0;
							if(cnt >= 16'd60)begin
								data_buf<={data_buf[39:0],1'b1};
							end
							else begin
								data_buf<={data_buf[39:0],1'b0};
							end
						end
						else begin
							cnt <= cnt + 1'b1;
							if(cnt >= 16'd65500)begin
								state <= s1;
								cnt<=16'd0;
								read_flag <= 1'b1;
							end	
						end
					end
				s9:begin
						data <= data_buf;
						if(din == 1'b1)begin
							state <= s10;
							cnt<=16'd0;
						end
						else begin
							cnt <= cnt + 1'b1;
							if(cnt >= 16'd65500)begin
								state <= s1;
								cnt<=16'd0;
								read_flag <= 1'b1;
							end	
						end
					end
				s10:begin
						state <= s1;
						cnt <= 16'd0;
					end
				default:begin
						state <= s1;
						cnt <= 16'd0;
					end	
			endcase
		end		
	end
	
endmodule 