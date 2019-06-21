/*
 * Servo Motor: FS5103R
 * Stop = 90
 * Full Speed CW = 0
 * Full Speed CCW = 180
 * Average Speed = 0.18 sec/60°
 * 
 * 
 * Hall Effect Sensor: 3144
 * Senses: "North"
 * 
 * Forklift: 3D Printed
 * # of Teeth on Gear:
 * # of Teeth per Inch:
 * # of Teeth per 360°
 * 
 */

#include <Servo.h>

Servo servoTest;
#define servoTestPin 10
#define still 90
#define CW 85
#define CCW 95

#define hallTest A1
int hallVal = 0;
 
void setup()
{
  Serial.begin(9600);

  servoTest.attach(servoTestPin);
}

void loop()
{
  hallVal = analogRead(hallTest);
  Serial.println(hallVal);
  delay(100);

  if (hallVal < 200)
  {
    servoTest.write(85);
    delay(1200);
  }
  {
    servoTest.write(still);
  }
}

