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
void lcd_write(unsigned char c);
void delay_LCD(unsigned long t);
void initializare_diacritice(void);
void scrie_diacritice_in_CGRAM(char matrice[8], unsigned char pozitie_DDRAM, unsigned char *diacritic);
void afisare_diacritice(unsigned char diacritic_afisat, unsigned char linia);
void dht11_init();
void find_response();
char read_dht11();
unsigned int read_adc();
void sterge_ecran(void);
void DHT_11_summation(void);
void read_HIH_5030(void);
void read_LM_35(void);
void Update_Current_Date_Time();
void main_RTC(void);
void Set_Time_Date();
void I2C_Initialize(const unsigned long feq_K);
void alarm(int hour, int minute, int second);
void main_LDR(void);

void read_LM_35(void)
{
  ADCON0 &= 0b111001111;
  ADCON0 |= 0b00001000;
  float temp = ADC_Read(1) * 5000.0 / 1024.0;
  temp = temp / 10.0;
  int s1 = (int)temp;
  int s2 = (int)((temp - (float)s1) * 10);
  lcd_goto(0x80);
  delay_LCD(100);
  char *temperature = (char *)"Temperature:";
  lcd_puts(temperature);
  char *pozitiv = (char *)"+";
  char *negativ = (char *)"-";
  lcd_goto(0xC0);
  delay_LCD(100);
  if (s1 >= 0)
  {
    lcd_puts(pozitiv);
  }
  if (s1 < 0)
  {
    lcd_puts(negativ);
  }
  char str1 = (s1 / 100) + '0';
  lcd_goto(0xC1);
  delay_LCD(100);
  lcd_putch(str1);
  if (s1 >= 100)
  {
    s1 = s1 % 100;
  }
  str1 = (s1 / 10) + '0';
  lcd_goto(0xC2);
  delay_LCD(100);
  lcd_putch(str1);
  s1 = s1 % 10;
  str1 = (s1) + '0';
  lcd_goto(0xC3);
  delay_LCD(100);
  lcd_putch(str1);
  str1 = s2 + '0';
  lcd_goto(0xC4);
  delay_LCD(100);
  lcd_putch('.');
  lcd_goto(0xC5);
  delay_LCD(100);
  lcd_putch(str1);
  lcd_goto(0xC6);
  delay_LCD(100);
  afisare_diacritice(0xDF, 0xC6);
  lcd_goto(0xC7);
  lcd_putch('C');
}
