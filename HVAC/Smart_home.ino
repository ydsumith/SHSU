#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2

/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

LiquidCrystal_I2C lcd (0x27, 16, 2); // set the LCD address to 0x27 for a16 chars and 2 line display

int relay_pin = 13;
double Tset = 27;
double Tcur = 0;
int plusbuttonPin = 9;
int minusbuttonPin = 10;
int buttonState = 0;
double T_tolerance = 2;

int Heater = 0;
int AC = 0;

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

/********************************************************************/
/********************************************************************/
/********************************************************************/
void setup(void)
{
  pinMode(relay_pin, OUTPUT);
  pinMode(plusbuttonPin, INPUT);
  pinMode(minusbuttonPin, INPUT);


  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("Hi");
  Serial.begin(9600);
  // start serial port
  Serial.println("Dallas Temperature IC Control Library Demo");
  // Start up the library
  sensors.begin();
}
/********************************************************************/
/********************************************************************/
/********************************************************************/
void loop(void)
{
  //Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperature readings
  //Serial.println("DONE");
  //Serial.print("Temperature is: ");
  Tcur = sensors.getTempCByIndex(0);
  //Serial.print(Tcur); // Why "byIndex"?

  buttonState = digitalRead(plusbuttonPin);
  if (buttonState == HIGH) {
    // increase temperature:
    Tset = Tset + 1;
  }

  buttonState = digitalRead(minusbuttonPin);
  if (buttonState == HIGH) {
    // decrease the temperature:
    Tset = Tset - 1;
  }


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tc=");
  lcd.print(Tcur);
  //lcd.print(",");
  lcd.print(",Ts= ");
  lcd.print(Tset);
  //lcd.print(" C");
  delay(100);

  //if relay_pin == HIGH then fan is turned on and heater is turned off
  //if relay_pin == LOW then fan is turned off and heater is turned on

  if (Tcur <= Tset - T_tolerance)
  {
    digitalWrite(relay_pin, LOW);    // turn the A/C off and Heater on
    Heater = 1;
    AC = 0;
    //delay(4000);                       // wait for a second
  }
  else if (Tcur  > Tset + T_tolerance) {
    digitalWrite(relay_pin, HIGH);   // turn the A/C on and Heater off
    //delay(4000);                       // wait for a second
    Heater = 0;
    AC = 1;
  }

  lcd.setCursor(0, 1);
  lcd.print("Heat: ");
  if (Heater == 1)
  {
    lcd.print("ON");
  }
  else {
    lcd.print("OFF");
  }

  lcd.print(", AC:");
  if (AC == 1)
  {
    lcd.print("ON");
  }
  else {
    lcd.print("OFF");
  }
}
