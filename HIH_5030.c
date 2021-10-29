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

void read_HIH_5030(void)
{
  ANSEL = 0xFF;
  ADCON0 = 0b11000001; //	Fosc/8	,  010-RA2 active	,0->A/D conversion off,  b0->1 =>A/D module operating
  ADCON1 = 0;
  TRISA = 0xFF; //toti pinii sunt de intrare
  char voltage_response[10];
  char adc_response[10];
  float adcValue = 0;  //ia valoarea de la A/D
  float voltage = 0;   //valoarea ADC converted to voltage
  float percentRH = 0; //din voltage in procent
  char RH_response[10];
  adcValue = read_adc();        //channel A
  voltage = 5 * adcValue / 255; //din ADC in voltage
  percentRH = (voltage + 0.0332) * 32.25 - 25.81;
  sprintf(RH_response, " RH = %.2f", percentRH);
  delay_LCD(800);
  lcd_goto(0x80);
  lcd_puts(RH_response);
}
