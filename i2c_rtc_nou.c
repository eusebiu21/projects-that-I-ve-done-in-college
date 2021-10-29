/*Template Proiect AEMC - ETTI Iasi*/
#include <xc.h>
#include <pic.h>
#include <pic16f887.h>
#include <math.h>
#include <stdio.h>
#define LEFT RB0
#define RIGHT RB1
#define SET RB2
__PROG_CONFIG(1, 0x20D4); // config. uC
__PROG_CONFIG(2, 0x0000); // config. uC

#define _XTAL_FREQ 8000000
void lcd_clear(void);
void lcd_goto(unsigned char pos);
void lcd_puts(char *s);
void lcd_putch(char c);
void lcd_write(unsigned char c);
void delay_LCD(unsigned long t);
void Set_Time_Date();
int BCD_2_DEC(int to_convert);
int DEC_2_BCD(int to_convert);
void Update_Current_Date_Time();
void I2C_Wait();
void I2C_Stop();
void I2C_Start();
void I2C_Write(unsigned data);
unsigned short I2C_Read(unsigned short ack);
void I2C_Repeated_Start();
void I2C_Initialize(const unsigned long feq_K);
void I2C_Begin();
void I2C_Hold();
void I2C_End();
void alarm(int hour, int second, int minute);

/*Set the current value of date and time below*/
int sec = 0;
int min = 3;
int hour = 2;
int date = 06;
int month = 05;
int year = 18;
/*Time and Date Set*/

void main_RTC(void)
{

  //TRISD = 0x00; //Make Port D pins as outptu for LCD interfacing
  //set time and date on the RTC module

  Update_Current_Date_Time(); //Read the current date and time from RTC module

  //Split the into char to display on lcd
  char sec_0 = sec % 10;
  char sec_1 = (sec / 10);
  char min_0 = min % 10;
  char min_1 = min / 10;
  char hour_0 = hour % 10;
  char hour_1 = hour / 10;
  char date_0 = date % 10;
  char date_1 = date / 10;
  char month_0 = month % 10;
  char month_1 = month / 10;
  char year_0 = year % 10;
  char year_1 = year / 10;

  //Display the Time on the LCD screen
  lcd_goto(0x80);
  lcd_puts(" TIME: ");
  lcd_putch(hour_1 + '0');
  lcd_putch(hour_0 + '0');
  lcd_putch(':');
  lcd_putch(min_1 + '0');
  lcd_putch(min_0 + '0');
  lcd_putch(':');
  lcd_putch(sec_1 + '0');
  lcd_putch(sec_0 + '0');

  //Display the Date on the LCD screen
  lcd_goto(0xC0);
  lcd_puts("DATE: ");
  lcd_putch(date_1 + '0');
  lcd_putch(date_0 + '0');
  lcd_putch(':');
  lcd_putch(month_1 + '0');
  lcd_putch(month_0 + '0');
  lcd_putch(':');
  lcd_putch(year_1 + '0');
  lcd_putch(year_0 + '0');
  __delay_ms(500); //refresh for every 0.5 sec
}

void Set_Time_Date()
{
  I2C_Begin();
  I2C_Write(0xD0);
  I2C_Write(0);
  I2C_Write(DEC_2_BCD(sec));   //update sec
  I2C_Write(DEC_2_BCD(min));   //update min
  I2C_Write(DEC_2_BCD(hour));  //update hour
  I2C_Write(1);                //ignore updating day
  I2C_Write(DEC_2_BCD(date));  //update date
  I2C_Write(DEC_2_BCD(month)); //update month
  I2C_Write(DEC_2_BCD(year));  //update year
  I2C_End();
}

int BCD_2_DEC(int to_convert)
{
  return (to_convert >> 4) * 10 + (to_convert & 0x0F);
}

int DEC_2_BCD(int to_convert)
{
  return ((to_convert / 10) << 4) + (to_convert % 10);
}

void Update_Current_Date_Time()
{
  //START to Read
  I2C_Begin();
  I2C_Write(0xD0);
  I2C_Write(0);
  I2C_End();

  //READ
  I2C_Begin();
  I2C_Write(0xD1); // Initialize data read
  sec = BCD_2_DEC(I2C_Read(1));
  min = BCD_2_DEC(I2C_Read(1));
  hour = BCD_2_DEC(I2C_Read(1));
  I2C_Read(1);
  date = BCD_2_DEC(I2C_Read(1));
  month = BCD_2_DEC(I2C_Read(1));
  year = BCD_2_DEC(I2C_Read(1));
  I2C_End();

  //END Reading
  I2C_Begin();
  I2C_Write(0xD1); // Initialize data read
  I2C_Read(1);
  I2C_End();
}

void I2C_Repeated_Start()
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F))
    ;
  RSEN = 1;
}

void I2C_Init(int i2c_clk_freq)
{
  SSPCON = 0x28;
  SSPADD = (_XTAL_FREQ / (4 * i2c_clk_freq)) - 1;
  SSPSTAT = 0;
}

void I2C_Initialize(const unsigned long feq_K) //Begin IIC as master
{
  ANS3 = 0;
  TRISA3 = 0;
  RA3 = 0;
  TRISC3 = 1;
  TRISC4 = 1; //Set SDA and SCL pins as input pins

  SSPCON = 0b00101000;  //pg84/234
  SSPCON2 = 0b00000000; //pg85/234

  SSPADD = (_XTAL_FREQ / (4 * feq_K * 100)) - 1; //Setting Clock Speed pg99/234
  SSPSTAT = 0b00000000;                          //pg83/234
}

void I2C_Hold()
{
  while ((SSPCON2 & 0b00011111) || (SSPSTAT & 0b00000100))
    ; //check the bis on registers to make sure the IIC is not in progress
}

void I2C_Begin()
{
  I2C_Hold(); //Hold the program is I2C is busy
  SEN = 1;    //Begin IIC pg85/234
}

void I2C_End()
{
  I2C_Hold(); //Hold the program is I2C is busy
  PEN = 1;    //End IIC pg85/234
}

void I2C_Write(unsigned data)
{
  I2C_Hold();    //Hold the program is I2C is busy
  SSPBUF = data; //pg82/234
}

unsigned short I2C_Read(unsigned short ack)
{
  unsigned short incoming;
  I2C_Hold();
  RCEN = 1;

  I2C_Hold();
  incoming = SSPBUF; //get the data saved in SSPBUF

  I2C_Hold();
  ACKDT = (ack) ? 0 : 1; //check if ack bit received
  ACKEN = 1;             //pg 85/234

  return incoming;
}

void alarm(int hours, int minute, int second )
{

  int sec, min, hour, year, date, month;
  __delay_ms(200);
  I2C_Begin();
  I2C_Write(0xD0);
  I2C_Write(0);
  I2C_Repeated_Start();
  I2C_Write(0xD1);
  sec = I2C_Read(1);
  min = I2C_Read(1);
  hour = I2C_Read(1);
  I2C_Read(1);
  date = I2C_Read(1);
  month = I2C_Read(1);
  year = I2C_Read(0);
  I2C_End();
  sec = (sec >> 4) * 10 + (sec & 0x0F);
  min = (min >> 4) * 10 + (min & 0x0F);
  hour = (hour & 0b00010000) * 10 + (hour & 0x0F);

  char b[15];
  sprintf(b, "Time: %d%d:%d%d:%d%d", hour / 10, hour % 10, min / 10, min % 10, sec / 10, sec % 10);
  lcd_goto(0x94);
  lcd_puts(b);

  char c[15];
  sprintf(c, "Alarm: %d%d:%d%d:%d%d", hours / 10, hours % 10, minute / 10, minute % 10, second / 10, second % 10);
  lcd_goto(0xD4);
  if (hours != 99 && minute != 99 && second != 99)
    lcd_puts(c);

  if ((hours == hour) && (minute == min) && (second == sec))
  {
    lcd_clear();
    char d[20];

    sprintf(d, "   TREZIREA!!!!");
    lcd_goto(0x80);
    lcd_puts(d);
    RA3 = 1;
  }
}
