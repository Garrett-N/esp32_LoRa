/* 
 *  LoRa Rx Module
 *  Uses: 
 *    ESP32 
 *    Featherwing GPS modules
 *    Hope RF 915Mhz LoRa Transceivers
 *    915.25 MHz
 *    
 *  Recieves LoRa packet and blinks LED
 *  Using to test range of Lora transmission
 *  with a basic 3.25" plain wire antenna
 */


#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2
// led for receive packet
#define LED 12
// pins for gps uart
#define RXD2 16
#define TXD2 17

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  //initialize gps serial 
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  while (!Serial);
  Serial.println("LoRa Receiver");
  pinMode(LED, OUTPUT);
  

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(915.25E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.println("Received LoRa packet!!'");

    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData);
      Serial.print("' with RSSI ");
      Serial.println(LoRa.packetRssi());

      // read gps
      Serial.println("GPS Data:");
      while (Serial2.available()) {
        Serial.print(char(Serial2.read()));
      }
      Serial.println("\n");
    }
    // Toggle external led
    digitalWrite(LED,HIGH);
    delay(100);
    digitalWrite(LED,LOW);  
  }
}
