/*Template Proiect AEMC - ETTI Iasi*/
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

int hour, minute, second;

void main(void)
{
  init_uC();
  init_LCD();
  init_ADC();
 // I2C_Initialize(100);
  //Set_Time_Date();

  while (1)
  {
    //  main_RTC();
/*
    if (RB0 == 0)
      hour++;
    if (hour == 25)
      hour = 0;
    if (RB1 == 0)
      minute++;
    if (minute == 61)
      minute = 0;
    if (RB2 == 0)
      second++;
    if (second == 61)
      second = 0;
    if (RB3 == 0)
    {
      lcd_clear();
      RA3 = 0;
      hour = second = minute = 0;
    }
    alarm(hour, minute, second);*/
    main_LDR();
	lcd_goto(0xC0);
	lcd_puts("TESTEST");
    //DHT_11_summation();
   //read_HIH_5030();
   //read_LM_35();
	
  }
}

void init_ADC()
{
  ADCON1 = 0x80;
  TRISA = 0x02;
  __delay_ms(10);
  //ADCON0 = 0x81;
}

unsigned int ADC_Read(char channel)
{
  __delay_ms(10);
  ADCON0 &= 0xc3;
  ADCON0 |= (channel << 2);
  ADCON0 |= 0xc5;
  __delay_ms(2);
  GO_nDONE = 1;
  while (GO_nDONE == 1)
    ;

  return ((ADRESH << 8) + ADRESL);
}

unsigned int read_adc()
{
  GO_nDONE = 1;
  while (GO_nDONE)
    ;
  return ADRESH;
}

void init_uC(void)
{
  OSCCON = 0x71; // setez Osc. intern uC de 8MHz // pag. 64
  TRISA = 0b11111111;
  TRISB = 0xFF;       // tot Portul B este de iesire
  TRISD = 0b00000000; // tot Portul B este de iesire
  ANSEL = 0x02;
  ANSELH = 0x00;
  PORTB = 0b00000000;      // initializez PORTB cu valori de 0 logic
  OPTION_REG = 0b00000111; // Frecv. intrare T0 = Frecv. Osc./4 (=8MHz/4) = 2MHz
                           // prescaler=256 => Frecv. T0 = 2MHz/256 = KHz (sau T=128us)
  TMR0IF = 0;              //
  TMR0 = 178;              // porneste numararea de la valoarea 178; pana la maxim (255)
  TMR0IE = 1;              // activez intreruperea ptr. T0
  GIE = 1;                 // activez Global intreruperile
}
