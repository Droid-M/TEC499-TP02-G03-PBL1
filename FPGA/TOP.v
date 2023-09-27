module TOP(
    	Clk                     ,
    	Rst_n                   ,   
   	Rx                      ,    
    	Tx                      ,
		modulo_sensors
	);

/////////////////////////////////////////////////////////////////////////////////////////
input           Clk             ; // Clock
input           Rst_n           ; // Reset
input           Rx              ; // RS232 RX line.
output          Tx              ; // RS232 TX line.


/////////////////////////////////////////////////////////////////////////////////////////
wire [7:0]    RxData          ; // Received data
wire [7:0]    	TxData     	; // Data to transmit.
wire          	RxDone          ; // Reception completed. Data is valid.
wire          	TxDone          ; // Trnasmission completed. Data sent.
wire            tick		; // Baud rate clock
wire          	TxEn            ;
wire 		RxEn		;
wire [3:0]      NBits    	;
wire [15:0]    	BaudRate        ; //328; 162 etc... (Read comment in baud rate generator file)
/////////////////////////////////////////////////////////////////////////////////////////
assign 		RxEn = 1'b1	;
assign 		TxEn = 1'b1	;
assign 		BaudRate = 16'd325; 	//baud rate set to 9600 for the HC-06 bluetooth module. Why 325? (Read comment in baud rate generator file)
assign 		NBits = 4'b1000	;	//We send/receive 8 bits
/////////////////////////////////////////////////////////////////////////////////////////


//Make connections between Rx module and TOP inputs and outputs and the other modules
UART_rx Receptor(
    	.Clk(Clk)             	,
   	.Rst_n(Rst_n)         	,
    	.RxEn(RxEn)           	,
    	.RxData(RxData)       	,
    	.RxDone(RxDone)       	,
    	.Rx(Rx)               	,
    	.Tick(tick)           	,
    	.NBits(NBits)
    );

///////////////////////////////Controle de protocolo de requisição da UART////////////////////////////////////
wire done_pc;
wire [4:0] endSensor;
wire [2:0] data_pc; 
protocol_control ControlReq(
	.RxDone(RxDone), 
	.RxData(RxData), 
	.clk(Clk),
	.rst(Rst_n),
	.done(done_pc),
	.sensor_adress(endSensor),
	.data(data_pc)
	);

////////////////////////////Modulo seletor de sensores//////////////////////////////////
inout [31:0] modulo_sensors;
wire sensorFunc;
sensorSelector selectSensor(.start(done_pc), .clk(Clk), .rst(Rst_n), .endS(endSensor), .out(sensorFunc), .enable(modulo_sensors));	

///////////////////////////////////////DHT11/////////////////////////////////////////////////
wire [39:0] dhtData;
wire dhtError, dhtDone;
DHT11 dht11China(
	.clk(Clk)			,  
	.start(trigger)     ,
	.rst_n(Rst_n)		,
	.dat_io(modulo_sensors[30])		,
	.data(dhtData)     ,
	.error(dhtError)					,
	.done(dhtDone)
);
assign trigger = done_pc || loop;

///////////////////Controle do fluxo de dados do DHT11 para cada requisição/////////////
wire doneCtr;
wire [7:0] dataCtrl;
wire continuos;
control fluxoDHT(
	.clk(Clk),
	.DHTdata(dhtData), 
	.DHTdone(dhtDone), 
	.DHTError(dhtErro), 
	.command(data_pc),
	.DoneCtrol(doneCtr), 
	.data(dataCtrl),
	.contEn(continuos));
	
////////////////////////////////////////////Controle aux para respostas-requisições///////////////////
wire doneAux;
wire [7:0]resp_aux;
control_aux ControleAuxResp(.clk(Clk), .rst(Rst_n), .Dado(data_pc), .done(doneAux), .En(done_pc), .resp(resp_aux), .sensorState(sensorFunc));

/////////////////////////////////////////Controle do comando de resposta////////////////////////////
wire doneRC;
wire [7:0] dataRc;
responseCounter controlResp(.TxD(TxDone),.doneCtr(doneCtr), .doneAux(doneAux), .resp(resAux), .dado(dataCtrl), .inEn(sensorFunc), .cntEn(continuos), .done(doneRC), .clk(Clk), .rst(Rst_n), .out(dataRc));

//////////////////////////////////////Modulo de monitoriamento continuo/////////////////////////
wire [7:0] infinity;
wire doneLc;
wire [7:0] loop;
leituraContinua(.clk(Clk), .rst(Rst_n), .doneCtrl(doneRC), .data_in(dataRc) ,.En(trigger), .Dados(infinity), .done(doneLC), .contEn(loop));

//Make connections between Tx module and TOP inputs and outputs and the other modules
UART_tx Transmissor(
   	.Clk(Clk)            	,
    	.Rst_n(Rst_n)         	,
    	.TxEn(doneLC)           	,
    	.TxData(infinity)      	,
   	.TxDone(TxDone)      	,
   	.Tx(Tx)               	,
   	.Tick(tick)           	,
   	.NBits(NBits)
    );

//Make connections between tick generator module and TOP inputs and outputs and the other modules
UART_BaudRate_generator BaudG(
    	.Clk(Clk)               ,
    	.Rst_n(Rst_n)           ,
    	.Tick(tick)             ,
    	.BaudRate(BaudRate)
    );



endmodule
