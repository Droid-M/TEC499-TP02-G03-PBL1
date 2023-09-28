## Problema #1 – Interfaces de E/S

**Projeto:** Sensor Digital em FPGA utilizando Comunicação Serial

**Instituição:** Universidade Estadual de Feira de Santana (UEFS)

**Curso:** Engenharia de computação

**Disciplina:** TEC499 MI - Sistemas digitais

**Turma:** TP02

**Semestre:** 2023.2

**Equipe:** 

*   Jeferson Almeida da Silva Ribeiro
*   João Vítor Cedraz Carneiro
*   Marcos Vinícius Ferreira dos Santos
*   Wesley Ramos

---

### 1\. Introdução

    O mercado de internet (Iot) das coisas está se expandindo rapidamente. Visando a introdução dos alunos nesse mercado e que eles possam obter a experiência fundamental para a formação de um profissional de computação, foi submetido a esses alunos que realizassem um projeto onde trabalhariam com a prototipação de um sensor de temperatura e umidade. Para tal, fez-se uso dos equipamentos disponibilizados em laboratório como a FPGA, o sensor de temperatura e umidade DHT11, dispositivo de comunicação UART e computadores desktops.

    Este relatório traz um resumo do que foi desenvolvido e um registro das experiências e resultados obtidos, considerando as falhas e os acertos.

---

### 2\. Metodologia

    Como descrito na introdução, alguns dispositivos presentes no laboratório foram usados durante a implementação da solução do problema. Não foi possível consultar mais detalhes sobre cada dispositivo, mas as especificações e informações principais que conseguimos reunir durante o desenvolvimento foram:

*   O sistema operacional usado nos computadores foi o Linux Ubuntu 64 bits.
*   O programa usado para desenvolver, compilar, descarregar e simular os códigos para a FPGA foi o Quartus II v. 20.1 - prime edition.
*   A FPGA usada foi a Cyclone IV.
*   O dispositivo de comunicação UART operou na faixa de 9600 bauds por segundo.
*   O sensor de temperatura e umidade usado foi o DHT11 (versão de 3 pinos).

    Passada a etapa de levantamento de informações sobre os dispositivos que utilizaríamos, a próxima etapa foi estabelecer um plano de desenvolvimento. Os tópicos a seguir abordarão essa etapa com mais descritividade.

#### 2.1. Protocolo

    Antes de preparar os algoritmos de comunicação e transferência de dados foi preciso entender quais e que tipo de dados serão transmitidos. Por recomendação do tutor, adotamos um protocolo de comunicação, que descreve detalhadamente como seria a comunicação entre o computador (PC) e a FPGA respeitando os padrões do protocolo UART.

    Para consultar nosso protocolo de comunicação, por favor, acesse [https://github.com/Droid-M/TEC499-TP02-G03-PBL1/blob/main/Docs/Protocol.md](https://github.com/Droid-M/TEC499-TP02-G03-PBL1/blob/main/Docs/Protocol.md).

#### 2.2. Programa em C

    O programa em C contou com a biblioteca [_**termios**_](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/termios.h.html) e a [_**unistd**_](https://en.wikibooks.org/wiki/C_Programming/POSIX_Reference/unistd.h) para transmitir e receber dados via UART. Como estamos lidando com 10 bits (1 de início + 8 de dados + 1 de parada), houve a necessidade de ajustar as configurações padrões do programa para que a paridade não fosse incluída e que apenas 1 bit de parada fosse considerado (ao invés de 2). 

    O programa em C está configurado para ler (`rx_char`) e transmitir (`tx_char`) 1 byte por vez, conforme as funções abaixo:

```c
char *rx_char()
{
char *buffer = (char *)malloc(2); // Alocar memória para o byte lido e o terminador nulo
if (buffer != NULL)
{
// Receber dados da porta serial
int numBytes = read(fd, buffer, 1); // Lê apenas 1 byte
if (numBytes == 1)
{
buffer[1] = '\0'; // Adicionar o terminador nulo
return buffer;
}
...
}
...
}

void tx_char(char *data)
{
write(fd, data, 1); // Envia apenas 1 byte independentemente do tamanho de 'data'
}
```

    A porta usada para o dispositivo UART no PC foi a “**ttyS0”.**

#### 2.3. Programa em verilog

    Ao todo, foram desenvolvidos 11 módulos em verilog obedecendo ao princípio de responsabilidade única. Dentre esses, os módulos principais são:

1.  **Controlador de requisição**: Trata as requisições enviadas pelo PC.
2.  **Controlador de resposta:** Agrupa e valida os dados obtidos do sensor com informações internas (no caso de leitura contínua).
3.  **TX:** Envia as informações obtidas pelo controlador de resposta para o PC de forma serial (bit a bit).
4.  **RX:** Recebe bit por bit a partir do sinal de início (start bit) e passa e as informações para o controlador de requisição.
5.  **DHT11:** Responsável por consultar e tratar os dados do sensor de temperatura e umidade.

    A figura abaixo ilustra os módulos presentes no programa feito para a FPGA e sua estrutura básica.  

![](https://33333.cdn.cke-cs.com/kSW7V9NHUXugvhoQeFaf/images/a8bee437f99d75c621840a0e26134d67caf6f345660c5ce8.png)

##### **Figura 1.** Módulos verilog

    Resumidamente, os módulos representados na figura 1 se relacionam conforme o seguinte fluxo:

1.  _**IHM**_ (código em C) manda 1 bytes para o _**RX**_ (FPGA). Ele fara isso 2 vezes por ciclo de funcionamento do projeto.
2.  O _**Rx**_ processa os dados pegando bit a bit e concatenando num vetor _**RxData**_ de 8 posições deslocando o dígito menos significativo (_LSB_) para direita. Concluindo a leitura dos 8 bits, o _**RxDone**_ sinaliza que todos os bits foram recebidos (indo para o nível lógico alto).
3.  O _**RxDone**_ serve como indicador de início do controle do protocolo (controlador de requisição). Ele vai capturar os 8 bits de dados e usá-los para o comando de requisição passando uma variável de 4 bits para o módulo **C**_**ontrol**_ e o **C**_**ontrolAux**_.
4.  O módulo _**Control**_ é o responsável por capturar o dado do sensor, mas ele não será usado nessa primeira leva de dados.
5.  Nesta parte o módulo **C**_**ontrolAux**_ é executado, pois o _response **Counter**_ identificará que se trata do primeiro bit de dados graças à sua máquina de estados.
6.  O _response **Counter**_ recebe o _**DoneCtrl**_ e o _**DoneAux**_ que informam quando os dados forem enviados para a próxima fase. 
7.  O _**ContEn**_ é responsável por ativar o sensoriamento continuo e também por permitir a transmissão dos dois bytes de dados que constituem a requisição e a medida solicitada.
8.  Nesta primeira etapa o _**ResponseCounter**_ entra em estado de suspensão e envia o byte de comando de resposta, além de avisar se o sensoriamento contínuo está ativo e se a transmissão foi finalizada.
9.  Esse sinal de finalização da transmissão enviado pelo _**ResponseCounter**_ serve para que o módulo transmissor da UART possa enviar dados bit a bit partindo _LSB_ para o _MSB_.
10.  Quando a transmissão acaba, o sinal de finalização do _**Tx**_ é acionado. Esse dado serve como um gatilho para o _**ResponseCounter**_ sair do modo de suspensão e ir para a segunda parte da máquina de estados. 
11.  A máquina de estados envia o sinal lido no sensor, transmite esse sinal e repete o ciclo inicial a partir da interface.
12.  O sensor é selecionado por um módulo à parte que está conectado a 32 pinos da GPIO. 
13.  O módulo contínuo manda um sinal ao módulo _**DHT11**_ para que ele consulte várias vezes, num intervalo de 4 segundos, os dados no sensor selecionado.

#### 2.4. Observações durante o desenvolvimento

*   O tamanho do _buffer_ pode interferir na velocidade de leitura dos bytes, assim como na quantidade de requisições de leitura por transmissão.
*   Na UART, lemos da direita para a esquerda (_MSB_ para _LSB_).

#### 2.5. Dificuldades enfrentadas

    A maior dificuldade que enfrentamos foi a implementação do módulo responsável pelo controle do sensor DHT11. Os padrões de comportamento do sensor fogem totalmente do que estávamos habituados a trabalhar até então e, por mais que fosse fácil entendê-lo, a implementação foi um processo doloroso. Devido ao prazo de entrega que se aproximava, tivemos que optar por buscar soluções prontas a respeito do DHT11 na internet para análise e estudo.

    Outra dificuldade que tivemos foi a implementação do protocolo (de requisição e de resposta) sincronizado. Houve um problema de comunicação entre os membros da equipe devido à divisão de responsabilidades então uma parte da equipe não estava totalmente ciente do que a outra parte desenvolvia até o momento de juntar todas as partes. O resultado foram funções divergentes cujas entradas e saídas não se conectavam. Então uma revisão de código foi necessária demandando mais tempo que o necessário para conclusão dessa etapa.

#### 2.6. Testes e simulações

    Na raiz do projeto existe um arquivo chamado “compile\_and\_execute\_codes.sh” responsável por automatizar o processo de compilação e execução do programa feito em _C_. Ao abrir esse arquivo num terminal (ou executá-lo diretamente em alguns sistemas operacionais), aparecerão 3 opções de arquivos para execução: “Menu principal”, “Testador simples” e “Sair”. A opção recomendada para quem deseja executar testes rápidos de conexão UART entre o PC e a FPGA é a opção 2 “Testador simples”. Para quem deseja executar o programa principal, basta escolher a opção 1 “Menu principal”.

    Para execução das simulações e testes do código verilog optamos pelo uso da ferramenta Quartus II. Entretanto, como se trata de um projeto com máquinas de estado complexas, o uso de _waveform_ se tornou praticamente inviável, nos restando apenas usar o recurso de visualização de máquinas de estado para executar a simulação _in software_. 

    Abaixo estão algumas capturas das visualizações de máquinas de estado.

![](https://33333.cdn.cke-cs.com/kSW7V9NHUXugvhoQeFaf/images/7a31baf68a4441868031316cd83c15e99eade64aa96baf79.png)

##### **Figura 2.** Máquina de estado do controle de protocolo

![](https://33333.cdn.cke-cs.com/kSW7V9NHUXugvhoQeFaf/images/aecc84f9f7716af6db05ae63c1e46e95c5c4a7437165c80b.png)

##### **Figura 3.** Máquina de estado do controle auxiliar

![](https://33333.cdn.cke-cs.com/kSW7V9NHUXugvhoQeFaf/images/c789853877218167d11f468aed9765c9eec219f93157bd1c.png)

##### **Figura 4.** Máquina de estado do _response counter_

     O código em _C_ que desenvolvemos também possui um [sistema de simulação](https://github.com/Droid-M/TEC499-TP02-G03-PBL1/tree/main/PC/models/linux/fake_uart.c). O intuito da simulação no código em _C_ foi facilitar o desenvolvimento nos momentos em que o dispositivo UART e a FPGA não estavam disponíveis e havia necessidade de validar o fluxo do programa e a implementação do protocolo (mencionado no tópico 2.1 deste relatório). Desta forma, criamos uma função que gera valores binários/hexadecimais aleatórios e tratamos esses valores no programa em C como se estivéssemos tratando valores enviados por uma FPGA real.

---

### Conclusão

    O processo de desenvolvimento da solução para o problema trouxe consigo uma série de dificuldades, porém, acreditamos que cada obstáculo que nos deparamos (como novos protocolos de comunicação, processo de transferência assíncrona, etc.) era mais uma forma de ampliar o conhecimento acerca deles. Também estamos satisfeitos em relação à maioria das ferramentas e conteúdo disponibilizados, porém, não houve orientação suficiente nos momentos mais críticos do desenvolvimento. E, talvez devido a isso adotamos soluções errôneas ao longo do desenvolvimento que poderiam ser evitadas de antemão, poupando tempo e esforços para a equipe. 

    Quanto à solução do problema, conseguimos implementar tudo que foi estabelecido nos requisitos (protocolo, módulo de comunicação UART, consulta ao sensor DHT11, interface de interação do usuário, etc.). Mas não conseguimos produzir uma solução 100% funcional, talvez por conta da desorientação ou pela falta de comunicação entre o grupo. O importante é que todos os conceitos trabalhados durante o problema 1 foram absorvidos com sucesso e temos ciência de que possuímos a capacidade de implementar soluções até mais complexas para o mercado Iot futuramente, a depender dos recursos e do planejamento.

---

### Referências

[https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/termios.h.html](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/termios.h.html)  
[https://en.wikibooks.org/wiki/C_Programming/POSIX_Reference/unistd.h](https://en.wikibooks.org/wiki/C_Programming/POSIX_Reference/unistd.h)  
[https://electronoobs.com/eng_arduino_tut26.php](https://electronoobs.com/eng_arduino_tut26.php)