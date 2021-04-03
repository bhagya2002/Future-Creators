// THIS CODE IS FOR A BLUETOOTH CAR RUNNING OFF DYNAMO MOTORS
// PARTS NEEDED CAN BE FOUND IN THE "parts.txt" file

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
 
#define L298N_IN1 12
#define L298N_IN2 13
#define L298N_IN3 14
#define L298N_IN4 27
 
BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;
 
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
 
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
 
void StartCar() {
  digitalWrite(L298N_IN1, HIGH);
  digitalWrite(L298N_IN2, LOW);
  digitalWrite(L298N_IN3, HIGH);
  digitalWrite(L298N_IN4, LOW);
}
void StopCar() {
  digitalWrite(L298N_IN1, LOW);
  digitalWrite(L298N_IN2, LOW);
  digitalWrite(L298N_IN3, LOW);
  digitalWrite(L298N_IN4, LOW);
}
void RightTurn() {
  digitalWrite(L298N_IN1, LOW);
  digitalWrite(L298N_IN2, HIGH);
  digitalWrite(L298N_IN3, HIGH);
  digitalWrite(L298N_IN4, LOW);
}
void LeftTurn() {
  digitalWrite(L298N_IN1, HIGH);
  digitalWrite(L298N_IN2, LOW);
  digitalWrite(L298N_IN3, LOW);
  digitalWrite(L298N_IN4, HIGH);
}
 
void ReverseCar() {
  digitalWrite(L298N_IN1, LOW);
  digitalWrite(L298N_IN2, HIGH);
  digitalWrite(L298N_IN3, LOW);
  digitalWrite(L298N_IN4, HIGH);
}
 
 
 
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };
 
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};
 
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic * pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      String myRX = (String)rxValue.c_str();
      if (rxValue.length() > 0) {
        Serial.print("Received Value: " + myRX);
        //        for (int i = 0; i < rxValue.length(); i++)
        //          Serial.print(rxValue[i]);
 
        if (myRX == "!B516") {
          Serial.println("start");
          StartCar();
        }
        if (myRX == "!B507" || myRX == "!B804" || myRX == "!B606" || myRX == "!B705") {
          Serial.println("stop");
          StopCar();
        }
        if (myRX == "!B813") {
          Serial.println("right");
          RightTurn();
        }
        if (myRX == "!B714") {
          Serial.println("left");
          LeftTurn();
        }
        if (myRX == "!B615") {
          Serial.println("reverse");
          ReverseCar();
        }
 
        Serial.println();
        Serial.println("*********");
      }
    }
};
 
 
void setup() {
  Serial.begin(115200);
 
  // Create the BLE Device
  BLEDevice::init("Bhagya's Car");
 
  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
 
  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);
 
  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID_TX,
                        BLECharacteristic::PROPERTY_NOTIFY
                      );
 
  pTxCharacteristic->addDescriptor(new BLE2902());
 
  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID_RX,
      BLECharacteristic::PROPERTY_WRITE
                                          );
 
  pRxCharacteristic->setCallbacks(new MyCallbacks());
 
  // Start the service
  pService->start();
 
  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
 
  pinMode(L298N_IN1, OUTPUT);
  pinMode(L298N_IN2, OUTPUT);
  pinMode(L298N_IN3, OUTPUT);
  pinMode(L298N_IN4, OUTPUT);
}
 
 
void loop() {
 
  if (deviceConnected) {
    pTxCharacteristic->setValue(&txValue, 1);
    pTxCharacteristic->notify();
    txValue++;
    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
  }
 
  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
}