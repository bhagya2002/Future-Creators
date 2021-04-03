// THIS CODE IS FOR A MINI WINDMILL PROJECT DESIGNED FOR SUSTAINABILITY
// PARTS NEEDED CAN BE FOUND IN THE "parts.txt" file

#define analog_pin A0
void setup() {
  Serial.begin(9600);

}

void loop() {

float x,y;
x = analogRead(analog_pin);
y = x*5.00/1024.00;
delay(400);
Serial.print("Sensor value:"); Serial.println(y);
delay(400);

}