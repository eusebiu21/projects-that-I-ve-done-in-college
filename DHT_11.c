#include <xc.h>
#include <pic.h>
#include <pic16f887.h>
#include <math.h>
#include <stdio.h>

__PROG_CONFIG(1, 0x20D4); // config. uC
__PROG_CONFIG(2, 0x0000); // config. uC

#define LED1 RB0
#define LED2 RB1
#define _XTAL_FREQ 8000000
#define output_humidity RB5 //etich. output senzor DHT11
#define analog_input RA2

void init_LCD(void);
void lcd_clear(void);
void lcd_goto(unsigned char pos);
void lcd_puts(char *s);
void lcd_putch(char c);
void lcd_write(unsigned char c);
void delay_LCD(unsigned long t);
void initializare_diacritice(void);
void dht11_init();
void find_response();
char read_dht11();
unsigned int read_adc();
void sterge_ecran(void);
void DHT_11_summation(void);
//var. pt prelucrarea datelor de la senzorul DHT11
unsigned char Check_bit, Temp_byte_1, Temp_byte_2, RH_byte_1, RH_byte_2;
unsigned char Himudity, RH, Sumation;

void DHT_11_summation()
{
  dht11_init();
  find_response();

  if (Check_bit == 1)
  {
    RH_byte_1 = read_dht11();
    RH_byte_2 = read_dht11();
    Temp_byte_1 = read_dht11();
    Temp_byte_2 = read_dht11();
    Sumation = read_dht11();
    if (Sumation == ((RH_byte_1 + RH_byte_2 + Temp_byte_1 + Temp_byte_2) & 0XFF))
    {
      Himudity = Temp_byte_1;
      RH = RH_byte_1;
      lcd_goto(0x80);
      char *temp_mesaj = (char *)"Temp:";
      lcd_puts(temp_mesaj);
      //lcd_puts("                ");
      lcd_putch(48 + ((Himudity / 10) % 10));
      lcd_putch(48 + (Himudity % 10));
      lcd_putch(0xDF);
      lcd_putch(0x43); //ASCII pt C
      lcd_goto(0xC0);  //a 2-a linie
      char *umiditate_afisare = (char *)"Humidity:";
      lcd_puts(umiditate_afisare);
      //lcd_puts("                ");
      lcd_putch(48 + ((RH / 10) % 10));
      lcd_putch(48 + (RH % 10));

      // lcd_puts(0x25);	//ascii pt %
    }
    else
    {
      char *sum_error = (char *)"Check sum error";
      lcd_puts(sum_error);
    }
  }
  else
  {
    char *error = (char *)"Error ";
    char *no_response = (char *)"No response ";
    sterge_ecran();
    lcd_goto(0x80);
    lcd_puts(error);
    lcd_goto(0xC0);
    lcd_puts(no_response);
  }
  __delay_ms(1000);
}

//functie initializare timpi intarziere +citire DHT11
//functie output ->output_humidity
void dht11_init()
{
  TRISB = 0b00000000;
  output_humidity = 0; //RE0 sends 0 to the sensor
  __delay_ms(18);
  output_humidity = 1; //RE0 sends 1 to the sensor
  __delay_us(30);
  TRISB = 0b00100000; //RE0 configurat ca input
}

/*
 * This will find the dht22 sensor is working or not.
 */

void find_response()
{
  Check_bit = 0;
  __delay_us(40);
  if (output_humidity == 0)
  {
    __delay_us(80);
    if (output_humidity == 1)
    {
      Check_bit = 1;
    }
    __delay_us(50);
  }
}

/*
 This Function is for read dht22.
 */

char read_dht11()
{
  char data, for_count;
  for (for_count = 0; for_count < 8; for_count++)
  {
    while (!output_humidity)
      ;
    __delay_us(30);
    if (output_humidity == 0)
    {
      data &= ~(1 << (7 - for_count)); //Clear bit (7-b)
    }
    else
    {
      data |= (1 << (7 - for_count)); //Set bit (7-b)
      while (output_humidity)
        ;
    } //Wait until PORTD.F0 goes LOW
  }
  return data;
}

void sterge_ecran(void)
{
  char *space = (char *)"		";
  delay_LCD(100);
  lcd_goto(0x80);
  lcd_puts(space);
  delay_LCD(100);
  lcd_goto(0xC0); //linia 2
  lcd_puts(space);
}
