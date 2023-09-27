### Pacote de requisição: PC para FPGA

    As requisições são compostas, no total, por 2 bytes separadas em 2 instantes (**endereço do sensor e comando de requisição,** respectivamente). Cada instante é retardado em 20 milissegundos com o intuito de respeitar o tempo em que a FPGA recebe, analisa e armazena os bits.

    O **endereço do sensor** é limitado na faixa de bits entre `00000001` e `00100000` (1 até 32). O endereço do sensor é uma informação que deve ser usada somente para decidir de qual sensor a FPGA deve consultar os dados. 

    O **comando de requisição** corresponde à operação que a FPGA fará. No protocolo implementado neste projeto, o usuário, através da interface do menu poderá informar 8 códigos de comando diferentes (de `0x01` até `0x08`), entretanto, apenas 6 desses códigos serão transmitidos para a FPGA. Isto porque os dois últimos códigos de comando do protocolo (`0x07` e `0x08`) nada mais são do que um complemento dos códigos `0x01` e `0x02` (consulta de temperatura e consulta de umidade, respectivamente) com o diferencial de que os dados são consultados e exibidos em loop constante até a interrupção manual do usuário.

    Abaixo está a tabela de **pacotes de requisição:**

<table>
    <tbody>
        <tr>
            <td><strong>Comando de requisição associado</strong></td>
            <td><strong>Endereço do sensor</strong></td>
            <td><strong>Comando de requisição</strong></td>
        </tr>
        <tr>
            <td>Solicita a situação atual do sensor</td>
            <td>(Entre <code>0x01</code> e <code>0x20</code>)</td>
            <td><code>0x00</code></td>
        </tr>
        <tr>
            <td>Solicita a medida de temperatura atual</td>
            <td>(Entre <code>0x01</code> e <code>0x20</code>)</td>
            <td><code>0x01</code></td>
        </tr>
        <tr>
            <td>Solicita a medida de umidade atual</td>
            <td>(Entre <code>0x01</code> e <code>0x20</code>)</td>
            <td><code>0x02</code></td>
        </tr>
        <tr>
            <td>Ativa sensoriamento contínuo de temperatura</td>
            <td>(Entre <code>0x01</code> e <code>0x20</code>)</td>
            <td><code>0x03</code></td>
        </tr>
        <tr>
            <td>Ativa sensoriamento contínuo de umidade</td>
            <td>(Entre <code>0x01</code> e <code>0x20</code>)</td>
            <td><code>0x04</code></td>
        </tr>
        <tr>
            <td>Desativa sensoriamento contínuo de temperatura</td>
            <td>(Entre <code>0x01</code> e <code>0x20</code>)</td>
            <td><code>0x05</code></td>
        </tr>
        <tr>
            <td>Desativa sensoriamento contínuo de umidade</td>
            <td>(Entre <code>0x01</code> e <code>0x20</code>)</td>
            <td><code>0x06</code></td>
        </tr>
        <tr>
            <td>Exibe o sensoriamento contínuo de temperatura</td>
            <td>(Entre <code>0x01</code> e <code>0x20</code>)</td>
            <td><code>0x07</code></td>
        </tr>
        <tr>
            <td>Exibe o sensoriamento contínuo de umidade</td>
            <td>(Entre <code>0x01</code> e <code>0x20</code>)</td>
            <td><code>0x08</code></td>
        </tr>
    </tbody>
</table>

---

### Pacote de resposta: FPGA para PC

    As respostas são compostas por 2 bytes no total. A composição essencial dos bytes que formam a resposta é dada por: **comando de resposta e dado**.

    Cada pacote de resposta terá um byte pivô chamado **comando de resposta**. Será através dele que o PC poderá analisar e supor o estado interno da FPGA (por exemplo, através do comando de resposta `00001100` é possível saber que a FPGA estará lendo o sensor e enviando os dados de temperatura que obter diretamente para o PC).  

    O byte de **dado** corresponde à parte inteira do valor de temperatura ou umidade obtidos do _DHT11_. A parte decimal dos valores e o checksum serão desconsiderados do protocolo para simplificação.

    Foi mencionado em um tópico anterior que o PC esperará em torno de 20 milissegundos para o envio de cada byte do protocolo a partir do momento em que o byte de endereço do sensor for enviado para que a FPGA possa armazenar e/ou tratar cada byte com a segurança de que não haverá perda de dados. Porém, a FPGA não fará pausas durante o envio dos bytes de resposta, pois todos os bytes enviados são salvos em _buffer_ no PC e isso garante que a FPGA possa enviar uma grande taxa de bytes quase que indeterminadamente e nenhum deles será perdido. Entretanto, o mesmo não poderia ser feito do PC para a FPGA, pois o limite de bytes que ela suporta armazenar é muito menor em comparação ao PC e, eventualmente ocorreria uma perda de bits recebidos por não haver mais lugar para armazená-los.

    Vale ressaltar que, apesar do fluxo do envio de dados da FPGA para o PC poder ser contínuo, a taxa de bits por segundos e baud (9600) deve ser respeitada para atender as especificações da UART.

    Abaixo está a tabela de **pacotes de resposta:**

<table>
    <tbody>
        <tr>
            <td><strong>Comando de requisição associado</strong></td>
            <td><strong>Comando de resposta</strong></td>
            <td><strong>Data</strong></td>
        </tr>
        <tr>
            <td>Solicita a situação atual do sensor</td>
            <td><code>00011111</code> (sensor com problema) ou <code>00000111</code> (sensor funcionando corretamente)</td>
            <td>Qualquer valor entre <code>00000000</code> e <code>11111111</code></td>
        </tr>
        <tr>
            <td>Solicita a medida de temperatura atual</td>
            <td><code>00001001</code> (medida de temperatura)</td>
            <td>Qualquer valor entre <code>00000000</code> e <code>11111111</code></td>
        </tr>
        <tr>
            <td>Solicita a medida de umidade atual</td>
            <td><code>00001000</code> (medida de umidade)</td>
            <td>Qualquer valor entre <code>00000000</code> e <code>11111111</code></td>
        </tr>
        <tr>
            <td>Ativa sensoriamento contínuo de temperatura</td>
            <td><code>00001100</code> (confirmação de ativação do sensoriamento contínuo de temperatura)</td>
            <td>Qualquer valor entre <code>00000000</code> e <code>11111111</code></td>
        </tr>
        <tr>
            <td>Ativa sensoriamento contínuo de umidade</td>
            <td><code>00001101</code> (confirmação de ativação do sensoriamento contínuo de umidade)</td>
            <td>Qualquer valor entre <code>00000000</code> e <code>11111111</code></td>
        </tr>
        <tr>
            <td>Desativa sensoriamento contínuo de temperatura</td>
            <td><code>00001010</code> (confirmação de desativação do sensoriamento contínuo de temperatura)</td>
            <td>Qualquer valor entre <code>00000000</code> e <code>11111111</code></td>
        </tr>
        <tr>
            <td>Desativa sensoriamento contínuo de umidade</td>
            <td><code>00001011</code>(confirmação de desativamento do sensoriamento contínuo de umidade)&nbsp;</td>
            <td>Qualquer valor entre <code>00000000</code> e <code>11111111</code></td>
        </tr>
    </tbody>
</table>