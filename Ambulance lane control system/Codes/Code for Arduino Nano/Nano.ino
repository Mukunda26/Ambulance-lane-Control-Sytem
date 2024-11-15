#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

// RFID setup
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

SoftwareSerial mySerial(5, 4);  // RX, TX

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("Nano setup complete. Waiting for RFID...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  Serial.print("Card detected: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  Serial.println("Ambulance detected, sending signal to Mega.");
  mySerial.write(1);  // Send signal to Mega for ambulance detection
  
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  delay(100);
}