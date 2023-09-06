## TEC499-TP02-G03-PBL1

    Projeto de Sensor Digital em FPGA utilizando Comunicação Serial.

---

### Observações durante o desenvolvimento

* O tamanho do _buffer_ pode interferir na velocidade de leitura dos bytes, assim como na quantidade de requisições de leitura por transmissão.
* Na UART, lemos da direita para a esquerda (_MSB_ para _LSB_).

---

### Pacote de requisição: PC para FPGA

    As requisições são compostas, no total, por 4 bytes separadas em 4 instantes (**cabeçalho, endereço do sensor, comando de requisição e rodapé**, respectivamente). Cada instante é retardado em 20 milissegundos com o intuito de respeitar o tempo em que a FPGA recebe, analisa e armazena os bits.

    O **cabeçalho** serve como um indicador do início do protocolo. Assim que a FPGA receber o valor `11111111` correspondente ao valor do cabeçalho, ela deve se preparar para capturar o endereço do sensor no próximo instante. No ato de “se preparar”, a FPGA deve descartar quaisquer valores remanescentes de outras operações, ignorando qualquer outro comando que esteja em execução no momento (por exemplo, mesmo que a FPGA esteja em monitoramento contínuo, assim que ela receber o byte de cabeçalho, ela deve parar o monitoramento contínuo imediatamente, seja ele de temperatura ou umidade).

    O **endereço do sensor** é limitado na faixa de bits entre `00000001` e `00100000` (1 até 32). O endereço do sensor é uma informação que deve ser usada somente para decidir de qual sensor a FPGA deve consultar os dados. 

    O **comando de requisição** corresponde à operação que a FPGA fará. No protocolo implementado neste projeto, o usuário, através da interface do menu poderá informar 8 códigos de comando diferentes (de `0x01` até `0x08`), entretanto, apenas 6 desses códigos serão transmitidos para a FPGA. Isto porque os dois últimos códigos de comando do protocolo (`0x07` e `0x08`) nada mais são do que um complemento dos códigos `0x01` e `0x02` (consulta de temperatura e consulta de umidade, respectivamente) com o diferencial de que os dados são consultados e exibidos em loop constante até a interrupção manual do usuário. 

    O **rodapé** serve como um indicador de fim de protocolo. Se a FPGA receber no instante 4 qualquer valor binário diferente de `01111111`, automaticamente ela descarta os 3 bytes registrados anteriormente e volta ao estado _IDLE_. Enquanto o 4º byte referente ao rodapé não for enviado, a FPGA vai permanecer em espera. Passado qualquer valor no 4º byte que seja diferente de `01111111`, a FPGA imediatamente deve devolver um **status** de erro para o PC.

Abaixo está a tabela de **pacotes de requisição:**

<table>
    <tbody>
        <tr>
            <td><strong>Comando de requisição associado</strong></td>
            <td><strong>Cabeçalho</strong></td>
            <td><strong>Endereço do sensor</strong></td>
            <td><strong>Comando de requisição</strong></td>
            <td><strong>Rodapé</strong></td>
        </tr>
        <tr>
            <td>Solicita a situação atual do sensor</td>
            <td><code>0xFF</code></td>
            <td>(Entre <code>0x01</code> e <code>0x20</code>)</td>
            <td><code>0x00</code></td>
            <td><code>0x7F</code></td>
        </tr>
        <tr>
            <td>Solicita a medida de temperatura atual</td>
            <td><code>0xFF</code></td>
            <td>(Entre <code>0x01</code> e <code>0x20</code>)</td>
            <td><code>0x01</code></td>
            <td><code>0x7F</code></td>
        </tr>
        <tr>
            <td>Solicita a medida de umidade atual</td>
            <td><code>0xFF</code></td>
            <td>(Entre <code>0x01</code> e <code>0x20</code>)</td>
            <td><code>0x02</code></td>
            <td><code>0x7F</code></td>
        </tr>
        <tr>
            <td>Ativa sensoriamento contínuo de temperatura</td>
            <td><code>0xFF</code></td>
            <td>(Entre <code>0x01</code> e <code>0x20</code>)</td>
            <td><code>0x03</code></td>
            <td><code>0x7F</code></td>
        </tr>
        <tr>
            <td>Ativa sensoriamento contínuo de umidade</td>
            <td><code>0xFF</code></td>
            <td>(Entre <code>0x01</code> e <code>0x20</code>)</td>
            <td><code>0x04</code></td>
            <td><code>0x7F</code></td>
        </tr>
        <tr>
            <td>Desativa sensoriamento contínuo de temperatura</td>
            <td><code>0xFF</code></td>
            <td>&nbsp;</td>
            <td><code>0x05</code></td>
            <td><code>0x7F</code></td>
        </tr>
        <tr>
            <td>Desativa sensoriamento contínuo de umidade</td>
            <td><code>0xFF</code></td>
            <td>(Entre <code>0x01</code> e <code>0x20</code>)</td>
            <td><code>0x06</code></td>
            <td><code>0x7F</code></td>
        </tr>
        <tr>
            <td>Exibe o sensoriamento contínuo de temperatura</td>
            <td><code>0xFF</code></td>
            <td>(Entre <code>0x01</code> e <code>0x20</code>)</td>
            <td><code>0x07</code></td>
            <td><code>0x7F</code></td>
        </tr>
        <tr>
            <td>Exibe o sensoriamento contínuo de umidade</td>
            <td><code>0xFF</code></td>
            <td>(Entre <code>0x01</code> e <code>0x20</code>)</td>
            <td><code>0x08</code></td>
            <td><code>0x7F</code></td>
        </tr>
    </tbody>
</table>

---

### Pacote de resposta: FPGA para PC

    As respostas são compostas por 8 bytes no total. A composição essencial dos bytes que formam a resposta é dada por: **cabeçalho, status, endereço do sensor, comando de resposta, dados\* e rodapé**.

    Assim como no pacote de requisição, o **cabeçalho** no pacote de resposta serve como um indicador do início do protocolo. Após o envio do pacote de requisição e a pausa de 20 milissegundos, o PC lê o primeiro byte do _buffer_, que a princípio é o primeiro byte do pacote de resposta da FPGA. Se o byte lido for diferente de `11111111` significa que houve algum problema na comunicação. Nesse caso, o usuário deve ser notificado do problema ou o pacote de requisição deve ser reenviado.

    O **status** indica se a FPGA conseguiu interpretar adequadamente o pacote de requisição enviado pelo PC, ou seja, indica se: 

* Todos os 4 bytes de requisição foram lidos corretamente (a começar pelo byte de cabeçalho).
* O sensor ao qual se quer consultar está acessível (isto porque pode acontecer de o byte referente ao endereço do sensor estar fora da faixa de `00000001` à `00100000` ou acontecer de alguma lógica interna do código verilog bloquear o acesso ao sensor no momento).
* O comando enviado pelo PC está na lista de comandos previamente definida na FPGA através do código verilog (pois pode acontecer de, por exemplo, o byte referente ao comando ser enviado como `00011000`, que não está na lista).
* O valor correto do rodapé foi enviado no 4º instante/byte (pois poderia acontecer de o 4º byte ser qualquer valor diferente de `01111111`).

    O **endereço do sensor** é uma informação que serve apenas para que o PC saiba a qual sensor atribuir os dados que estão sendo enviados pela UART, então é obrigatório que o valor retornado no byte referente ao endereço do sensor seja o mesmo valor recebido pela FPGA no pacote de requisição mais recente efetuado pelo PC.

    Cada pacote de resposta terá um byte pivô chamado **comando de resposta**. Será através dele que o PC poderá analisar e supor o estado interno da FPGA (por exemplo, através do comando de resposta `00001100` é possível saber que a FPGA estará lendo o sensor e enviando os dados de temperatura que obter diretamente para o PC).  

    Diferente do status, endereço do sensor e do rodapé compostos por 1 byte, os **dados** são compostos por 3 bytes. A necessidade de 3 bytes se faz presente nos pacotes de requisição que requerem o retorno do valor da temperatura e/ou umidade lida pelo sensor. Nesses casos, os bytes de dados são preenchidos e enviados respeitando a seguinte ordem:

1. Dado1: byte que informa se o sensor está funcionando (quando o comando de requisição for `0x00`) ou byte que informa a parte inteira do valor da temperatura ou da umidade (quando o comando de requisição for `0x01` ou `0x02`, por exemplo).
2. Dado2: byte que informa a parte decimal (quando o comando de requisição for `0x01` ou `0x02`, por exemplo).
3. Dado3: byte que informa se os dados lidos no sensor (quando o comando de requisição for `0x01` ou `0x02`, por exemplo) passaram na validação checksum, sendo `00000001` para “sucesso” e `00000000` para “falha”. Através desse byte o PC vai ter ciência de que os dados de temperatura ou umidade recebidos estão incorretos, podendo notificar o usuário acerca disso ou apenas solicitar uma nova leitura.

    O **rodapé** serve apenas para informar ao PC que a resposta chegou ao fim. Desta forma, quando o PC receber qualquer valor no 8º instante/byte que seja diferente de `01111111`, ele deve interpretar que houve alguma falha na transmissão ou recepção dos dados e, a partir daí, notificar o usuário sobre o ocorrido.

    Foi mencionado em um tópico anterior que o PC esperará em torno de 20 milissegundos para o envio de cada byte do protocolo a partir do momento em que o byte de cabeçalho for enviado para que a FPGA possa armazenar e/ou tratar cada byte com a segurança de que não haverá perda de dados. Porém, a FPGA não fará pausas durante o envio dos bytes de resposta, pois todos os bytes enviados são salvos em _buffer_ no PC e isso garante que a FPGA possa enviar uma grande taxa de bytes quase que indeterminadamente e nenhum deles será perdido. Entretanto, o mesmo não poderia ser feito do PC para a FPGA, pois o limite de bytes que ela suporta armazenar é muito menor em comparação ao PC e, eventualmente ocorreria uma perda de bits recebidos por não haver mais lugar para armazená-los.

    Abaixo está a tabela de **pacotes de resposta:**

<table>
    <tbody>
        <tr>
            <td><strong>Comando de requisição associado</strong></td>
            <td><strong>Cabeçalho</strong></td>
            <td><strong>Status</strong></td>
            <td><strong>Endereço do sensor</strong></td>
            <td><strong>Comando de resposta</strong></td>
            <td><strong>Data1</strong></td>
            <td><strong>Data2</strong></td>
            <td><strong>Data3</strong></td>
            <td><strong>Rodapé</strong></td>
        </tr>
        <tr>
            <td>Solicita a situação atual do sensor</td>
            <td><code>11111111</code></td>
            <td><code>00000001</code> (sucesso na interpretação do pacote de requisição) ou <code>00000000</code> (falha na interpretação do pacote de requisição)</td>
            <td>Entre <code>00000001</code> e <code>00100000</code></td>
            <td><code>00011111</code> (sensor com problema) ou <code>00000111</code> (sensor funcionando corretamente)</td>
            <td><code>00011111</code> (sensor com problema) ou <code>00000111</code> (sensor funcionando perfeitamente)</td>
            <td><code>00000000</code></td>
            <td><code>00000000</code></td>
            <td><code>01111111</code></td>
        </tr>
        <tr>
            <td>Solicita a medida de temperatura atual</td>
            <td><code>11111111</code></td>
            <td><code>00000001</code> (sucesso na interpretação do pacote de requisição) ou <code>00000000</code> (falha na interpretação do pacote de requisição)</td>
            <td>Entre <code>00000001</code> e <code>00100000</code></td>
            <td><code>00001001</code> (medida de temperatura)</td>
            <td>(qualquer valor binário que corresponda à parte inteira do valor de temperatura medido)</td>
            <td>(qualquer valor binário que corresponda à parte decimal do valor de temperatura medido)</td>
            <td><code>00000001</code> (passou na validação do checksum) ou <code>00000000</code> (falhou na validação do checksum)&nbsp;</td>
            <td><code>01111111</code></td>
        </tr>
        <tr>
            <td>Solicita a medida de umidade atual</td>
            <td><code>11111111</code></td>
            <td><code>00000001</code> (sucesso na interpretação do pacote de requisição) ou <code>00000000</code> (falha na interpretação do pacote de requisição)</td>
            <td>Entre <code>00000001</code> e <code>00100000</code></td>
            <td><code>00001000</code> (medida de umidade)</td>
            <td>(qualquer valor binário que corresponda à parte inteira do valor de umidade medido)</td>
            <td>(qualquer valor binário que corresponda à parte decimal do valor de umidade medido)</td>
            <td><code>00000001</code> (passou na validação do checksum) ou <code>00000000</code> (falhou na validação do checksum)&nbsp;</td>
            <td><code>01111111</code></td>
        </tr>
        <tr>
            <td>Ativa sensoriamento contínuo de temperatura</td>
            <td><code>11111111</code></td>
            <td><code>00000001</code> (sucesso na interpretação do pacote de requisição) ou <code>00000000</code> (falha na interpretação do pacote de requisição)</td>
            <td>Entre <code>00000001</code> e <code>00100000</code></td>
            <td><code>00001100</code> (confirmação de ativação do sensoriamento contínuo de temperatura)</td>
            <td>(qualquer valor binário que corresponda à parte inteira do valor de temperatura medido)</td>
            <td>(qualquer valor binário que corresponda à parte decimal do valor de temperatura medido)</td>
            <td><code>00000001</code> (passou na validação do checksum) ou <code>00000000</code> (falhou na validação do checksum)&nbsp;</td>
            <td><code>01111111</code></td>
        </tr>
        <tr>
            <td>Ativa sensoriamento contínuo de umidade</td>
            <td><code>11111111</code></td>
            <td><code>00000001</code> (sucesso na interpretação do pacote de requisição) ou <code>00000000</code> (falha na interpretação do pacote de requisição)</td>
            <td>Entre <code>00000001</code> e <code>00100000</code></td>
            <td><code>00001101</code> (confirmação de ativação do sensoriamento contínuo de umidade)</td>
            <td>(qualquer valor binário que corresponda à parte inteira do valor de umidade medido)</td>
            <td>(qualquer valor binário que corresponda à parte decimal do valor de umidade medido)</td>
            <td><code>00000001</code> (passou na validação do checksum) ou <code>00000000</code> (falhou na validação do checksum)&nbsp;</td>
            <td><code>01111111</code></td>
        </tr>
        <tr>
            <td>Desativa sensoriamento contínuo de temperatura</td>
            <td><code>11111111</code></td>
            <td><code>00000001</code> (sucesso na interpretação do pacote de requisição) ou <code>00000000</code> (falha na interpretação do pacote de requisição)</td>
            <td>Entre <code>00000001</code> e <code>00100000</code></td>
            <td><code>00001010</code> (confirmação de desativação do sensoriamento contínuo de temperatura)</td>
            <td><code>00001010</code></td>
            <td><code>00000000</code></td>
            <td><code>00000000</code></td>
            <td><code>01111111</code></td>
        </tr>
        <tr>
            <td>Desativa sensoriamento contínuo de umidade</td>
            <td><code>11111111</code></td>
            <td><code>00000001</code> (sucesso na interpretação do pacote de requisição) ou <code>00000000</code> (falha na interpretação do pacote de requisição)</td>
            <td>Entre <code>00000001</code> e <code>00100000</code></td>
            <td><code>00001011</code>(confirmação de desativamento do sensoriamento contínuo de umidade)&nbsp;</td>
            <td><code>00001011</code></td>
            <td><code>00000000</code></td>
            <td><code>00000000</code></td>
            <td><code>01111111</code></td>
        </tr>
    </tbody>
</table>