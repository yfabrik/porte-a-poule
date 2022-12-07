#include <DS3231.h> //Import DS3231 Code Library
#include <Wire.h> //Import Wire Code Library
 
DS3231 Clock;
bool Century=false;
bool h12;
bool PM;
byte XDAY, XHOUR, XMIN, XSEC, XBITS;
bool XDY, A12h, Apm;
byte year, month, date, DoW, hour, minute, second;
 
void setup() // Start the I2C interface
{ 
Wire.begin();
// THIS IS VERY IMPORTANT
// If we going to set the time, change these numbers to the date and time you want to set to, and then upload it to the arduino. 
// once you have finished setting the time, comment out the following clock.set functions and then re-upload it to the board. Otherwise your clock will reset every time you open the serial monitor.
 
Clock.setSecond(50);//To set the second 
Clock.setMinute(22);//To set the minute 
Clock.setHour(10);  //To set the hour 
Clock.setDoW(1);    //To set the day of the week
Clock.setDate(29);  //To set the date of the month
Clock.setMonth(11);  //To set the month of the year
Clock.setYear(22);  //To set the year (Last two digits of the year) 
Serial.begin(9600); // Start the serial interface at 9600 boud rate
}
void ReadDS3231()
{
int second,minute,hour,date,month,year,temperature; 
second=Clock.getSecond();
minute=Clock.getMinute();
hour=Clock.getHour(h12, PM);
date=Clock.getDate();
month=Clock.getMonth(Century);
year=Clock.getYear();
temperature=Clock.getTemperature();
Serial.print("20");
Serial.print(year,DEC);
Serial.print('-');
Serial.print(month,DEC);
Serial.print('-');
Serial.print(date,DEC);
Serial.print(' ');
Serial.print(hour,DEC);
Serial.print(':');
Serial.print(minute,DEC);
Serial.print(':');
Serial.print(second,DEC);
Serial.print('\n');
Serial.print("Temperature =");
Serial.print(temperature); 
Serial.print('\n');
}
void loop() {
	
ReadDS3231();
delay(1000);
}