#include <xc.h>
#include <pic.h>
#include <pic16f887.h>
#include <math.h>
#include <stdio.h>

__PROG_CONFIG(1, 0x20D4); // config. uC
__PROG_CONFIG(2, 0x0000); // config. uC

#define _XTAL_FREQ 8000000


unsigned int ADC_Read(char channel);
void init_ADC(void);
void init_uC(void);
void interrupt etti(void); // functie de intreruperi globala ptr. TOATE intreruperile de pe un
void init_LCD(void);
void lcd_clear(void);
void lcd_goto(unsigned char pos);
void lcd_puts(char *s);
void lcd_putch(char c);
unsigned int read_adc();
void main_LDR(void);

void main_LDR(void)
{
  ANSEL = 0xFF;
  ADCON0 = 0b11000001; //	Fosc/8	,  010-RA2 active	,0->A/D conversion off,  b0->1 =>A/D module operating
  ADCON1 = 0;
  TRISA = 0xFF; //toti pinii sunt de intrare
  float lightLevel;
  lightLevel = read_adc();
  char s[20];
  sprintf(s, "Level = %.2f", lightLevel);
  lcd_goto(0x80);
  lcd_puts(s);
}
