Projeto desenvolvido no ambiente IAR Embeded Workbench IDE; ver.: 5.60.2 e implementado em um microcontrolador MSP430F1611

O projeto tem o intuito de desenvolver o controle de um motor de passo, em que são controladas a direção e a velocidade.

O circuito utilizou de um teclado matricial 3x4, um display LCD 16x2 e um motor de passo.

Os valores exibidos no display serão:

o Situação de giro - esquerda, direita ou parado;

o Velocidade - de 1 a 9.

Funcionamento do software ao pressionar as seguintes teclas:

| Tecla | Função | Display |
| --- | --- | --- |
| 5 | parar o motor | “Parado” |
| 4 | girar para a esquerda | “Esquerda” |
| 6 | girar para a direita | “Direita” |
| 2 | velocidade++ | velocidade atualizada |
| 8 | velocidade-- | velocidade atualizada |

Logo após o RESET o motor deve estar parado e o valor da velocidade deve ser
igual a 5.
