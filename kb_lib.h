
/* Título: bllbioteca de funcões do teclado kit MSP430F1611
Autor: Joel A. Santos
Data: abril/2019
Ambiente: IAR Embeded Workbench IDE; ver.: 5.5.2.1
última rev: 04/2019
------------------------------------------------------------------------------*/
/* Título: Leitura de teclado matricial por INTERRUPÇÃO
Autor: Joel A. Santos
Data: agosto/2020
Ambiente: IAR Embeded Workbench IDE; ver.: 5.60.2
última rev: 06/2021
------------------------------------------------------------------------------*/
/* Título: Revisão e implementação de mais algumas funções - Leitura de teclado matricial por INTERRUPÇÃO 
Autora: Bianca Lopes dos Santos
Data: agosto/2022
Ambiente: IAR Embeded Workbench IDE; ver.: 5.60.2
última rev: 12/2022
------------------------------------------------------------------------------*/

//-----------------------Declaração de Funções---------------------------------

//--------------------------Funções do Teclado--------------------------------

// Configura o I/O correspondente ao teclado
void tec_config_rti()
{
  P2SEL &= ~(BIT0+BIT1+BIT2);       // Configura P2 como I/O 
  P2DIR &= ~(BIT0+BIT1+BIT2);       //Define P2.0,P2.1 e P2.2 como entrada - colunas
  P2IE  |=  (BIT0+BIT1+BIT2);       // Habilita a INT gerada por estes bits
  P2IES &= ~(BIT0+BIT1+BIT2);       // Estes bits geram INT na borda de subida
  
  P3SEL &= ~(BIT0+BIT1+BIT2+BIT3);  // Configura P3 como I/O  
  P3DIR |=  (BIT0+BIT1+BIT2+BIT3);  // define linhas em P3 como saída
  P3OUT |=  (BIT0+BIT1+BIT2+BIT3);  // Seta estes bits para permiter a geração
                                    // de borda de subida em P2 quando uma tecla
                                    // qualquer for pressionada.
  //BIT4+BIT5+BIT6+BIT7
  //BIT0+BIT1+BIT2+BIT3
}

// Converte o código de varredura obtido na função tec_rd em ASCII
char ascii_convert(char key_code)
{
  unsigned char tab_tec[12] = {"0123456789AE"};
  unsigned char tab_key[12] = {0x28,0x41,0x21,0x11,0x42,0x22,0x12,0x44,0x24,0x14,0x48,0x18};
  unsigned int i=0;
  unsigned flag =0; // sinalisador de conversão de tecla
  
  while ( flag==0x00)
  {
    if ( key_code != tab_key[i])
    {
      i++;
    }
    else
      flag=0xFF;
  }
  return tab_tec[i];
}

// Faz a varredura do teclado e gera como saída o código ASCII da tecla presssionada.
// SE não houver tecla pressionada retorna 0x00
char tec_rd_rti()
{
  char col = 0, lin = BIT0, key_code=0;
  
    // Permanece em loop até encontrar uma coluna ativa
    // ou até o termino da varredura
    while (col==0x00 & lin < BIT4)
    {
      P3OUT = P3OUT &~(BIT0+BIT1+BIT2+BIT3); // Zera todas as saídas
      P3OUT = P3OUT | lin; //ativa uma linha
      
      // O delay da versão por polling foi retirado
      
      col = P2IN &(BIT0+BIT1+BIT2); //atualiza estado das colunas
      
      if (col!=0x00)
      {
        col= col << 4;// concatenar key code
        key_code=lin+col;
        Tecla = ascii_convert(key_code); //converter em ASCII
      }
      else
        lin = lin << 1; //ativar próxima linha
    }
  

  
  // Seta estes bits para permiter a geração de borda de subida em P2 
  // quando uma tecla qualquer for pressionada.
  P3OUT |=  (BIT0+BIT1+BIT2+BIT3);  
  
  return (Tecla); // retorna ASCII da tecla
  
  
}