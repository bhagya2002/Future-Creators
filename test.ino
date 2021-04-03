// THIS CODE IS TO TEST THE ESP32 
// PARTS NEEDED CAN BE FOUND IN THE "parts.txt" file

int LED_BUILTIN = 2;
void setup() {
pinMode (LED_BUILTIN, OUTPUT);
}
void loop() {
digitalWrite(LED_BUILTIN, HIGH);
delay(1000);
digitalWrite(LED_BUILTIN, LOW);
delay(1000);
}