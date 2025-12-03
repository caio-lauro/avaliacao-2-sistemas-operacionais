Avaliacao 2 Sistemas Operacionais
Simulador de um Sistema Detector de Incêndios Florestais

Estudantes: Caio Lauro de Lima, Luma da Silva Bergmann, Yuri Daniel Moreira Gomes

____________________________________________________________________________________

                             Instruções de Compilação
____________________________________________________________________________________

Para compilação do projeto, são necessários os utilitários:
    • Make;
    • GCC;
Além disso, faz-se necessário o uso da biblioteca <pthread.h>.
Para compilar, basta rodar
    make
na raíz do projeto.
O executável será gerado em bin/main

____________________________________________________________________________________

                             Instruções de Utilização
____________________________________________________________________________________

Para boa utilização, é necessário o uso de um terminal com: suporte à true color,
para a devida exibição das cores de fundo e de texto; UTF-8, para exibição do 
emoji de fogo (🔥); e um terminal com pelo menos 100 células de largura por 35
células de altura, para exibição de toda a matriz simultaneamente e de forma 
correta.

Para utilizar o programa, basta rodar o comando
    bin/main
na raíz do projeto.

Para finalização do programa, dado a natureza de sua execução por tempo 
indeterminado, é necessário o uso de CTRL-C na janela do terminal para mandar um
sinal de interrupção (SIGINT) para o programa.

Durante a execução do programa, é gerado um arquivo de log, incendios.log, na 
pasta raíz do projeto, que contém mensagens no formato:

Incêndio detectado por (NÚMERO DA THREAD), nas coordendas (X, Y) em HH:MM:SS

____________________________________________________________________________________

                                Explicações Gerais
____________________________________________________________________________________

Estrutura do projeto:
.
├── LICENSE (Licensa de uso: MIT License)
├── Makefile (Makefile utilizado por make para compilação do projeto)
├── README.txt (Este arquivo)
├── bin (Diretório para o executável)
│   └── main (Executável)
├── incendios.log (Log dos incêndios)
├── include (Diretório para os arquivos Header)
├── obj (Diretório para os arquivos objeto usados na compilação)
└── src (Diretório para os arquivos de implementação)

A seed utilizada por srand é baseada no tempo desde o começo da Era Unix (tempo
retornado por time(NULL) desde 1º de janeiro de 1970). A partir dessa seed, são
geradas posições pseudo-aleatórias para a thread incendiária atear fogo.

O arquivo de log é sempre sobrescrito ao ser aberto inicialmente em src/main, em 
modo de escrita, assim que o programa é iniciado. Então, na thread central, o 
arquivo é brevemente aberto em modo de append, onde recebe a mensagem no formato 
explicitado anteriormente e é fechado logo após, de modo que as alterações fiquem
salvas quando o programa é finalizado através do sinal de interrupção.

Cada thread sensor e a thread central possuem filas de mensagem, utilizadas para
contornar o possível problema de sobrescrever mensagens, permitindo o envio de 
múltiplas mensagens para cada uma dessas threads. Cada fila de mensagens possui
um mutex mtx, bloqueado quando existem operações nas filas e uma variável de 
condição cond, que sinaliza quando um novo elemento é adicionado na fila para a 
função de pop (que remove o primeiro elemento da fila), evitando-se assim um pop
numa fila vazia.
