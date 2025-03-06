#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
String msg;

void setup()
{
	// initialize the LCD
	lcd.init();

	// Turn on the blacklight and print a message.
	lcd.backlight();
	Serial.begin(9600);
}

void loop(){

  while(Serial.available()) {
    delay(50);
    char c = Serial.read();
    msg+= c;
  }
  if(msg.length() >0) {
    lcd.clear();
    lcd.print(msg);
  }
  msg = "";
	
}
