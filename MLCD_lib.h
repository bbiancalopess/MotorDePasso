#include "io430.h"


// Coloque aqui os seus defines.
#define RS BIT7         // O pino RS está conectado em P3.7
#define EN BIT6         // O pino Enable está conectado em P3.6
#define Fclk 750e3
#define _100us 100e-6*Fclk // Num pulsos de clk para delay de 100us
#define _2ms (long)(2E-3*Fclk)
#define _250ms (long)(250E-3*Fclk)
#define _5ms (long)(5E-3*Fclk)
#define _3s (long)(3*Fclk)

#define CEFET "CEFET_MG"
#define Turma_subturma "ELT3B_T1"
#define ID_prog "Prog_04"
#define Data "13/06/2022"
#define Limpa  "                "




// Protótipo das funções
void Init_Display();
void Config_Display();
void EnviaComando(unsigned char Comando);
void EnviaDado(unsigned char Dado);
void Envia();
void EnviaString(unsigned char *string);




void start( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  // Inicializa o I/O ligado ao MLCD
  Init_Display(); 
  
  // Configura o MLCD para funcionamento básico
  Config_Display();
  
  // Envia um caractere para o MLC
  
  
  

}



/* Função inicializa I/O para o display
Barramento de dados P5.0 até P5.7
Pino RS P3.7
Pino En P3.6

*/




void Init_Display()
{
  P5SEL = 00;    // Configura 8 bits como I/O
  P5DIR = 0xFF;  // Configura 8 bits como saída
  
  P3SEL &= ~(EN+RS);  // Configura Bits 6 e 7 como I/O
  // A forma acima é a versão reduzida de: 
  // P3SEL = P3SEL & ~(BIT6+BIT7)
  P3DIR |= (EN+RS);  // Configura bits 6 e 7 com saída
}
/*
Inicializa o controladora do MLCD para o funcionamento
básico
Matriz 5x7, duas linhas ativas, cursor ligado
*/
void Config_Display()
{
__delay_cycles(_250ms);   // Delay de 250ms  
EnviaComando (0x38);
__delay_cycles(_5ms);   // Delay de 5ms
EnviaComando (0x0C);
EnviaComando (0x06);
EnviaComando (0x01);
__delay_cycles(_2ms);   // Delay de 2ms

}

// Função que envia um comando para o MLCD
// Neste caso o pino RS = 0
void EnviaComando(unsigned char Comando)
{
 P5OUT = Comando;
 P3OUT = P3OUT & ~RS;  // Faz RS = 0
 Envia();
 }

// Função que envia um dado para o MLCD
// Neste caso o pino RS = 1
void EnviaDado(unsigned char Dado)
{
 P5OUT = Dado;
 P3OUT = P3OUT | RS;  // Faz RS = 1
 Envia();
 }

// Gera o pulso de Enable
void Envia()
{
 P3OUT = P3OUT & ~EN;  //Faz Enable = 0
 P3OUT = P3OUT |  EN;  //Faz Enable = 1 
 P3OUT = P3OUT & ~EN;  //Faz Enable = 0
 __delay_cycles(_100us);  // Delay de 100us
}

// Envia uma frase
void EnviaString(unsigned char *string)
{
 while(*string)
 {
  EnviaDado(*string);
  string++;
 }


}

void Tela_de_abertura()
  {
     EnviaComando(0x84);
    EnviaString(CEFET);
    EnviaComando(0xC4);
    EnviaString(Turma_subturma);
    __delay_cycles(_3s);
    EnviaComando(0x80);
    EnviaString(Limpa);
    EnviaComando(0xC0);
    EnviaString(Limpa);
    EnviaComando(0x84);
    EnviaString(ID_prog);
    EnviaComando(0xC2);
    EnviaString(Data);
    __delay_cycles(_3s);
    EnviaComando(0x80);
    EnviaString(Limpa);
    EnviaComando(0xC0);
    EnviaString(Limpa);
    EnviaComando(0x80);
  }
