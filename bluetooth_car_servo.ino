// THIS CODE IS FOR A BLUETOOTH CAR RUNNING OFF SERVO MOTORS
// PARTS NEEDED CAN BE FOUND IN THE "parts.txt" file

#include <ServoTimer2.h>
#include <AltSoftSerial.h>

ServoTimer2 servoA;
ServoTimer2 servoB;
AltSoftSerial BTserial;
char button; //character sent when button is presse=d


void setup() {
  // put your setup code here, to run once:
  //defines pin numbers for Motor A
  /*servoA.attach(10);
  servoB.attach(11);*/
  Serial.begin(9600);
  BTserial.begin(9600);
  Serial.println("Starting...");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(BTserial.available()>0)
  {
    button = BTserial.read();
    Serial.write(button);

    switch (button) {
      case 'F':
        servoA.attach(10);
        servoB.attach(11);
        servoA.write(1700);
        servoB.write(1300);
        delay(20);
        break;

      case 'B':
        servoA.attach(10);
        servoB.attach(11);
        servoA.write(1300);
        servoB.write(1700);
        delay(20);
        break;

      case 'R':
        servoA.attach(10);
        servoB.attach(11);
        servoB.write(1500);
        servoA.write(1700);
        delay(20);
        break;

      case 'L':
        servoA.attach(10);
        servoB.attach(11);
        servoB.write(1300);
        servoA.write(1500);
        delay(20);
        break;

      /*case 'S':
        servoB.write(1500);
        servoA.write(1500); 
        break;*/
        case 'S':
        servoB.detach();
        servoA.detach();
        break;
    }
  }
  }