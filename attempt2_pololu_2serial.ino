// Serial 1
#define resetPin1 3
#define errPin1 4

// Serial 2
#define resetPin2 5
#define errPin2 6

// some variable IDs 1
#define ERROR_STATUS1 0
#define LIMIT_STATUS1 3
#define TARGET_SPEED1 20
#define INPUT_VOLTAGE1 23
#define TEMPERATURE1 24

#define FORWARD_ACCELERATION1 5
#define REVERSE_ACCELERATION1 9
#define DECELERATION1 2

// some variable IDs 2
#define ERROR_STATUS2 0
#define LIMIT_STATUS2 3
#define TARGET_SPEED2 20
#define INPUT_VOLTAGE2 23
#define TEMPERATURE2 24
 
#define FORWARD_ACCELERATION2 5
#define REVERSE_ACCELERATION2 9
#define DECELERATION2 2

int readByte1()
{
  char c;
  if (Serial1.readBytes(&c, 1) == 0){return -1;}
  return (byte)c;
}

int readByte2()
{
  char d;
  if (Serial2.readBytes(&d, 1) == 0){return -1;}
  return (byte)d;
}

void exitSafeStart1()
{
  Serial1.write(0x83);
}

void exitSafeStart2()
{
  Serial2.write(0x83);
}

void setMotorSpeed1 (int speed)
{
  if (speed < 0)
  {
    Serial1.write(0x86);
    speed = -speed;
  }
  else
  {
    Serial1.write(0x85);
  }
  Serial1.write(speed & 0x1F);
  Serial1.write(speed >> 5 & 0x7F);
}

void setMotorSpeed2 (int speed)
{
  if (speed < 0)
  {
    Serial2.write(0x86);
    speed = -speed;
  }
  else
  {
    Serial2.write(0x85);
  }
  Serial2.write(speed & 0x1F);
  Serial2.write(speed >> 5 & 0x7F);
}

unsigned char setMotorLimit1 (unsigned char limitID1, unsigned int limitValue1)
{
  Serial1.write(0xA2);
  Serial1.write(limitID1);
  Serial1.write(limitValue1 & 0x7F);
  Serial1.write(limitValue1 >> 5 & 0x1F);
  return readByte1();
}

unsigned char setMotorLimit2 (unsigned char limitID2, unsigned int limitValue2)
{
  Serial2.write(0xA2);
  Serial2.write(limitID2);
  Serial2.write(limitValue2 & 0x7F);
  Serial2.write(limitValue2 >> 5 & 0x1F);
  return readByte2();
}

unsigned int getVariable1 (unsigned char variableID1)
{
  Serial1.write(0xA1);
  Serial1.write(variableID1);
  return readByte1() + 256*readByte1();
}

unsigned int getVariable2 (unsigned char variableID2)
{
  Serial2.write(0xA1);
  Serial2.write(variableID2);
  return readByte2() + 256*readByte2();
}

void setup()
{
  Serial1.begin(19200);

  pinMode(resetPin1, OUTPUT);
  digitalWrite(resetPin1, LOW);
  delay(1);
  
  pinMode(resetPin1, INPUT);
  delay(5);

  pinMode(errPin1, INPUT);

  Serial2.begin(19200);

  pinMode(resetPin2, OUTPUT);
  digitalWrite(resetPin2, LOW);
  delay(1);

  pinMode(resetPin2, INPUT);
  delay(5);

  pinMode(errPin2, INPUT);

  Serial1.write(0xAA);  // send baud-indicator byte
  setMotorLimit1(FORWARD_ACCELERATION1, 4);
  setMotorLimit1(REVERSE_ACCELERATION1, 10);
  setMotorLimit1(DECELERATION1, 20);

  Serial2.write(0xAA);  // send baud-indicator byte
  setMotorLimit2(FORWARD_ACCELERATION2, 4);
  setMotorLimit2(REVERSE_ACCELERATION2, 10);
  setMotorLimit2(DECELERATION2, 20);

  exitSafeStart1();
  exitSafeStart2();
}

void loop() 
{
  setMotorSpeed1(0);
  setMotorSpeed2(0);
  Serial1.println((int)getVariable1(TARGET_SPEED1));
  Serial2.println((int)getVariable2(TARGET_SPEED2));
  delay(500);
  
  setMotorSpeed1(800);
  setMotorSpeed2(800);
  Serial1.println((int)getVariable1(TARGET_SPEED1));
  Serial2.println((int)getVariable2(TARGET_SPEED2));
  delay(1500);
  
  setMotorSpeed1(0);
  setMotorSpeed2(0);
  Serial1.println((int)getVariable1(TARGET_SPEED1));
  Serial2.println((int)getVariable2(TARGET_SPEED2));
  delay(500);
  
  setMotorSpeed1(-800);
  setMotorSpeed2(-800);
  Serial1.println((int)getVariable1(TARGET_SPEED1));
  Serial2.println((int)getVariable2(TARGET_SPEED2));
  delay(1500);

  setMotorSpeed1(0);
  setMotorSpeed2(0);
  Serial1.println((int)getVariable1(TARGET_SPEED1));
  Serial2.println((int)getVariable2(TARGET_SPEED2));
  delay(500);

  setMotorSpeed1(1600);
  setMotorSpeed2(-1600);
  Serial1.println((int)getVariable1(TARGET_SPEED1));
  Serial2.println((int)getVariable2(TARGET_SPEED2));
  delay(1500);

  setMotorSpeed1(0);
  setMotorSpeed2(0);
  Serial1.println((int)getVariable1(TARGET_SPEED1));
  Serial2.println((int)getVariable2(TARGET_SPEED2));
  delay(500);

  setMotorSpeed1(-1600);
  setMotorSpeed2(1600);
  Serial1.println((int)getVariable1(TARGET_SPEED1));
  Serial2.println((int)getVariable2(TARGET_SPEED2));
  delay(1500);
 
  Serial1.print("VIN = ");
  Serial1.print(getVariable1(INPUT_VOLTAGE1));
  Serial1.println(" mV");

  Serial2.print("VIN = ");
  Serial2.print(getVariable2(INPUT_VOLTAGE2));
  Serial2.println(" mV");
 
  // if an error is stopping the motor, write the error status variable
  // and try to re-enable the motor
  if (digitalRead(errPin1) == HIGH or digitalRead(errPin2) == HIGH)
  {
    Serial1.print("Error Status: 0x");
    Serial1.println(getVariable1(ERROR_STATUS1), HEX);

    Serial2.print("Error Status: 0x");
    Serial2.println(getVariable2(ERROR_STATUS2), HEX);
    
    exitSafeStart1();
    exitSafeStart2();
  }
}

