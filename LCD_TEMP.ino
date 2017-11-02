//LCD DISPLAY START
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#define I2C_ADDR    0x27 // Define I2C Address where the PCF8574A is
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
int n = 1;
LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

//TEMP-SENSOR START
#include "DHT.h"
#define DHTPIN 52
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//MOTOR-SHIELD START
#include <AFMotor.h>
AF_DCMotor motor(1);
AF_DCMotor air(2);

void setup() {
  
      dht.begin();
      
      lcd.begin (16,2);
      //Hintergrundbeleuchtung aktivieren
      lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
      lcd.setBacklight(HIGH);
      lcd.home ();  
      
      motor.setSpeed(100);
      motor.run(RELEASE);
      air.setSpeed(255);
      air.run(RELEASE);
}

void loop() {
  lcd_display_temp();
  freshair();
}


void freshair() {
  //lcd.setCursor(0,0);
  //lcd.print("Belueftung:");
  //lcd.setCursor(0,1);
  //lcd.print("aktiviert");
  motor.run(FORWARD);
  delay(1200);
  motor.run(RELEASE);
  air.run(FORWARD);
  delay(10000);
  air.run(RELEASE);
  motor.run(BACKWARD);
  delay(750);
  motor.run(RELEASE);
  delay(3000);
}

void lcd_display_temp() {

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);

     //Seite 1 Konsole
  Serial.print("Luftfeuchtigkeit");
  Serial.print(h);
  
  //Seite 1 
      //Zeile 1
      lcd.setCursor ( 0, 0 );
      lcd.print("Luftfeuchtigkeit");
      //Zeile 2
      lcd.setCursor(0, 1);
      lcd.print(h);
      lcd.print(" %              ");
      delay(7000);

   //Seite 2 Konsole
  Serial.print("Temperatur");
  Serial.print(t);
  
  //Seite 2
      //Zeile 1
      lcd.setCursor ( 0, 0 );
      lcd.print("Temperatur      ");
      //Zeile 2
      lcd.setCursor(0, 1);
      lcd.print(t);
      lcd.print(" *C             ");
      delay(7000);
}




