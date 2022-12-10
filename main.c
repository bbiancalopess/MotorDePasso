
/* 
Prog06C_Motor_passo_timerA:

Elaborar um programa que faça o controle do acionamento do kit motor de passo do
nosso laboratório (veja a Prática 16).
- Serão utilizadas as teclas 2, 4, 5, 6 e 8. O display deverá estar sempre atualizado com
a situação do motor. Portanto, devem ser sembre exibidas a direção de giro e a
velocidade.
- A indicação de velocidade deverá ser exibida na primeira linha do display e a situação
do motor(parado, esquerda, direita) na segunda linha
- As teclas e suas funções são:

Tecla Função Display
5 parar o motor “Parado”
4 girar para a esquerda “Esquerda”
6 girar para a direita “Direita”
2 velocidade++ atualiza a velocidade
8 velocidade-- atualiza a velocidade

- Fica a cargo do aluno definir as velocidades de giro do motor, desde que haja
uma proporção linear entre a velocidade mímina(igual a 1) e a velocidade máxima
(igual a 9).
- Logo após o RESET o motor deve estar parado e o valor da velocidade deve ser
igual a 5.
- Neste programa trabalharemos com o acionamento do motor sendo cadenciado
pelo timer A. Ná próxima implementação trabalharemos como a temporização por timer.
*/


#define Parado 0            // Possibilidades da variável Giro
#define Esquerda -1
#define Direita 1

// Variáveis Globais
char Tecla = 0;        // Contem o código ASCII da varredura do teclado
int Giro = Parado;     // Define o sentido de giro do motor
int Velocidade = 5;    // Determina a velocidade do motor

#include "io430.h"
#include "MLCD_lib.h"
#include "kb_lib.h"
#include "stdlib.h"


void main( void )
{ 
 unsigned char MSG_Esquerda[] = "Esquerda";
 unsigned char MSG_Direita[]  = "Direita ";
 unsigned char MSG_Parado[]   = "Parado  ";
 
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  TACCR0 = 320;
  TACTL &= ~TAIFG;
  
  // Configurações dos periféricos
  Init_Display();
  Config_Display();
  tec_config_rti();
  
  P1SEL &= ~(BIT0+BIT1+BIT2+BIT3);
  P1DIR |= (BIT0+BIT1+BIT2+BIT3);
  P1OUT &= ~(BIT0+BIT1+BIT2+BIT3);
  
  // Exibe as mensagens iniciais: motor parado e velocidade =5
  EnviaString("Velocidade = 5");
  EnviaComando(0xC0);  // Muda para início da 2a linha
  EnviaString(MSG_Parado);      
  
  
  __enable_interrupt();    // Habiliatação geral das INTs
      
  while (1)
  {
    if (Tecla)   // Verifica se uma tecla foi pressionada
    {
      switch(Tecla)
      {
        // Situação para tecla 5 pressionada
      case '5':                    // '5' equivale a 0x35
        if (Giro != Parado)
        {
          Giro = Parado;
          EnviaComando(0xC0);  // Muda para início da 2a linha
          EnviaString(MSG_Parado);  
          TACTL &= ~(MC0+MC1);          
        }
        break;
        
        // Situação para tecla 4 pressionada
      case '4':
        if (Giro != Esquerda)
        {
          Giro = Esquerda;
          EnviaComando(0xC0);  // Muda para início da 2a linha
          EnviaString(MSG_Esquerda);     
          TACTL = TASSEL0 + MC0 + TACLR + TAIE;
        }
        break;
        
        //Situação para tecla 6 pressionada
      case '6':
        if (Giro != Direita)
        {
          Giro = Direita;
          EnviaComando(0xC0);  // Muda para início da 2a linha
          EnviaString(MSG_Direita);  
          TACTL = TASSEL0 + MC0 + TACLR + TAIE;
        }
        break;
        
       //Situação para tecla 2 pressionada
      case '2':
        if (Velocidade < 9)
        {
          Velocidade++;
          EnviaComando(0x8D);
          EnviaDado(Velocidade+0x30);
          TACCR0  = TACCR0 - 64;
        }
        break;
        
       //Situação para tecla 8 pressionada
       case '8':
        if (Velocidade > 1)
        {
          Velocidade--;
          EnviaComando(0x8D);
          EnviaDado(Velocidade+0x30);
          TACCR0  = TACCR0 + 64;
        }
        break;
      }
      Tecla = 0;    // A tarefa de atendimento do pressionar da tecla já foi executada
    }
  }
}

  
 

//----------------------------------------------------------------------------
/*
Função que gera o sequenciamento do motor
A variável Giro determina o sentido de rotação do motor
*/
//----------------------------------------------------------------------------

#pragma vector=TIMERA1_VECTOR
__interrupt void Motor()
{
  static int Estado[2][4] = {0x00, 0x00, 0x00, 0x00, 0x0C, 0x06, 0x03, 0x09};                                         
  static int Step = 0;
  /* 
  A linha [0] da matriz (toda zerada) faz o motor parar
  A linha [1] da matriz faz o motor girar
  O sentido de giro é definido pela variação da variável Step
  */
    P1OUT = Estado[abs(Giro)][Step];
  
  /* 
  Promove o incremento o decremento da variável Step. 
  O resultado sempre abrange os dois bits menos significativos.
  Sendo assim, nunca vai ser diferente de 00, 01, 10, 11 em binário
  */
    Step = ((Giro+Step)&(BIT1+BIT0));
    TACTL &= ~TAIFG;
}


#pragma vector=PORT2_VECTOR
 __interrupt void Teclado_rti()
 {
   //Desabilita as INTs de P2
   P2IE &= ~(BIT0+BIT1+BIT2);
   //Periféricos com múltiplas fontes ressetado ao fonal da RTI (em inglês ISR = rotina de tratamento de INT)
   P2IFG &= ~(BIT0+BIT1+BIT2);
   
   __enable_interrupt(); //Reabilitação geral das INTS
   
 // Aguarda a passagem do tempo correspondente à trepidação da tecla
 // Normalmente, um tempo de delay de 5ms é suficiente.
 __delay_cycles((long)(Fclk*5e-3));


 // Passada a trepidação, só faz a varredura se alguma coluna estiver ativa.
 // Isto quer dizer que a INT ocorreu no fechamento da tecla
 if (P2IN &(BIT0+BIT1+BIT2))
 {
 Tecla = tec_rd_rti();
 }

 //Periféricos com múltiplas fontes de INT devem ter o flag correspondente
 // ressetado ao final da RTI (em inglês ISR = rotina de tratamento de INT)
 P2IFG &= ~(BIT0+BIT1+BIT2);
 
 //Reabilita as INTS de P2
 P2IE |= (BIT0+BIT1+BIT2);
 }
