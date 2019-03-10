//                                      $$\                            $$$$$$\  
//                                      $$ |                          $$  __$$\ 
//         $$$$$$$\  $$$$$$\   $$$$$$$\ $$$$$$$\  $$\   $$\ $$\   $$\ $$ /  \__|
//        $$  _____| \____$$\ $$  _____|$$  __$$\ $$ |  $$ |$$ |  $$ |$$$$$$$\  
//        \$$$$$$\   $$$$$$$ |\$$$$$$\  $$ |  $$ |$$ |  $$ |$$ |  $$ |$$  __$$\ 
//         \____$$\ $$  __$$ | \____$$\ $$ |  $$ |$$ |  $$ |$$ |  $$ |$$ /  $$ |
//        $$$$$$$  |\$$$$$$$ |$$$$$$$  |$$ |  $$ |\$$$$$$  |\$$$$$$  | $$$$$$  |
//        \_______/  \_______|\_______/ \__|  \__| \______/  \______/  \______/
//
// Project name: 3-factor authenticated lock (NodeMCU code)
// Author: Sashwat K
// Created On: 10 Mar 2019
// Revision: 1
// Last Edited: 10 Mar 2019

#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = D5;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

byte myIFID[4] = {145,14,128,185};
int switchKey = D0;
int firstAuthentication = D3;
int thirdAuthentication = D4;

void setup() {
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  pinMode(switchKey, INPUT);
  pinMode(firstAuthentication, OUTPUT);
  pinMode(thirdAuthentication, INPUT);
  
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void loop() {
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
 
  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  int keySwitchState = digitalRead(switchKey);
  int thirdAuthenticationState = digitalRead(thirdAuthentication);
  if(keySwitchState == 1) {
    digitalWrite(firstAuthentication, HIGH);
    Serial.print("Key switch: ");
    Serial.println(keySwitchState);
    if(thirdAuthenticationState == 1) {
      Serial.println("2nd authentication complete");
      // Look for new cards

      if (rfid.uid.uidByte[0] == myIFID[0] || 
        rfid.uid.uidByte[1] == myIFID[1] || 
        rfid.uid.uidByte[2] == myIFID[2] || 
        rfid.uid.uidByte[3] == myIFID[3] ) {
          Serial.println(F("Yooo"));
        }
    }
    else {
      Serial.println("2nd authentication incomplete");
    }
  }
  else {
    digitalWrite(firstAuthentication, LOW);
  }
  // Halt PICC
  rfid.PICC_HaltA();
 
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}
