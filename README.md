# TEC499-TP02-G03-PBL1

Projeto de Sensor Digital em FPGA utilizando Comunicação Serial

---

## Observações durante o desenvolvimento

- O tamanho do buffer pode interferir na velocidade de leitura dos bytes, assim como na quantidade de requisições de leitura por transmissão
- Na uart, lemos da direita para a esquerda (MSB para LSB)

---

## Comandos de requisição: PC para FPGA

As requisições são compostas, no total, por 4 bytes sendo separada em 4 instantes (cabeçalho, endereço do sensor, código do comando e checksum, respectivamente). Cada instante é retardado em 20 milissegundos com o intuito de respeitar o tempo em que a FPGA recebe, analisa e armazena os bits.

O **cabeçalho** serve como um indicador do início do protocolo. Assim que a FPGA receber o valor `11111111` correspondente ao valor do cabeçalho, ela deve se preparar para capturar o endereço do sensor no próximo instante. No ato de “se preparar”, a FPGA deve descartar quaisquer valores remanescentes de outras operações, ignorando qualquer outro comando que esteja em execução no momento (por exemplo, mesmo que a FPGA esteja em monitoramento contínuo, assim que ela receber o byte de cabeçalho, ela deve parar o monitoramento contínuo imediatamente, seja ele de temperatura ou umidade.

O **endereço do sensor** é limitado na faixa de bits entre `00000001` e `00100000` (1 até 32). O endereço do sensor é uma informação que deve ser usada somente para decidir de qual sensor a FPGA deve consultar os dados.

O **código de comando** corresponde à operação que a FPGA fará. No protocolo implementado neste projeto, o usuário, através da interface do menu poderá informar 8 códigos de comando diferentes (de 0x01 até 0x08), entretanto, apenas 6 desses códigos serão transmitidos para a FPGA. Isto porque os dois últimos códigos de comando do protocolo (0x07 e 0x08) nada mais são do que um complemento dos códigos 0x01 e 0x02 (consulta de temperatura e consulta de umidade, respectivamente) com o diferencial de que os dados são consultados e exibidos em loop constante até a interrupção manual do usuário.

O **checksum** corresponde ao complemento de dois da soma dos bytes de **cabeçalho, endereço do sensor e código de comando**. Vale observar que a soma deverá incluir os bits excedentes (_carries_).
