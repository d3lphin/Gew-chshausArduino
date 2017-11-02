/* INITIALISIERUNG */

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


//MOTOR-SHIELD START
#include <AFMotor.h>
AF_DCMotor pumpe(1);
AF_DCMotor propeller(2);
AF_DCMotor motor(3);

//TEMP-SENSOR START
#include "DHT.h"
#define DHTPIN 52
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);


// VARIABLEN
boolean dopen = false;
boolean watered = false;
float h = 0;         //h = Luftfeuchtigkeit
float t = 0;      //t = Temperatur in Grad (°C)
float f = 0;
int hg = 0;   //humadity ground


//SETUP
void setup() {

    //KONSOLEN PORT
    Serial.begin(9600);
    //wait (!Serial);
    
    //SKRIPT START
    dht.begin();
    lcd.begin(16,2);
    
    //DISPLAY START
    lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
    lcd.setBacklight(HIGH);
    lcd.home ();
    //ANZEIGE
    lcd.setCursor(0,0);
    lcd.print("STARTING...     ");
    lcd.setCursor(0,1);
    lcd.print("SCRIPT BY AR    ");
    delay(5000);
    lcd.setCursor(0,0);
    lcd.print("LOADING...       ");
    lcd.setCursor(0,1);
    lcd.print("INFORMATION      ");
    delay(1000);

    //MOTOR-SHIELD
    pumpe.run(FORWARD); //ABGESCHALTET / ANGESCHALTET = BACKWARD / RELEASE
    propeller.run(FORWARD);
    motor.run(RELEASE);
    motor.setSpeed(255);
}

void loop() {
  // put your main code here, to run repeatedly:

  dach();

}

void tempscan() {
  //DHT22
   h = dht.readHumidity();         //h = Luftfeuchtigkeit
   t = dht.readTemperature();      //t = Temperatur in Grad (°C)
   f = dht.readTemperature(true);
   hg = analogRead(A7);
   
  
}

void displaydata() {
  
  tempscan();
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

    //Seite 3
      //Zeile 1
      lcd.setCursor ( 0, 0 );
      lcd.print("Bodenfeuchte    ");
      //Zeile 2
      lcd.setCursor(0, 1);
      lcd.print(hg/10);
      lcd.print(" %              ");
      delay(7000);
}

void dach() {
  
  tempscan();

  
  displaydata();
  
  if ((t >= 30) && (dopen == false)) {
        
        //DISPLAY
        lcd.setCursor ( 0, 0 );
        lcd.print("Belueftung:     ");
        lcd.setCursor(0, 1);
        lcd.print(h);
        lcd.print("AKTIVIERT       ");
        
        dachopen();

        delay(3000);

        displaydata();
        
  }

  if ((dopen == true) && (t < 25)) {
          
          //DISPLAY
          lcd.setCursor ( 0, 0 );
          lcd.print("Belueftung:     ");
          lcd.setCursor(0, 1);
          lcd.print(h);
          lcd.print("DEAKTIVIERT     ");
          
          dachclose();
          
          delay(3000);

          displaydata();
       }
  
}

void dachopen() {

      dopen = true;
      motor.run(FORWARD);
      delay(1200);
      motor.run(RELEASE);
      propeller.run(BACKWARD);
      
}

void dachclose() {

      propeller.run(FORWARD);
      motor.run(BACKWARD);
      delay(700);
      motor.run(RELEASE);
      dopen = false;
}

void waterpump() {

  tempscan();
  
  if ((watered == false) && (h >= 600)) {
    pumpe.run(BACKWARD);
    delay(10000); //10s Bewässerung
    pumpe.run(FORWARD);

  }
  
}

void waitwatered() {
  if (watered == true) {
      for (int x = 0; x < 1000; x++) {
          if (x == 900) {
              watered = false;
          } else {
              delay(1000);
              continue;
          }
      
      }
    
  }
  
}

