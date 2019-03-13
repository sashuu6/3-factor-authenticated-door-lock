//                                      $$\                            $$$$$$\  
//                                      $$ |                          $$  __$$\ 
//         $$$$$$$\  $$$$$$\   $$$$$$$\ $$$$$$$\  $$\   $$\ $$\   $$\ $$ /  \__|
//        $$  _____| \____$$\ $$  _____|$$  __$$\ $$ |  $$ |$$ |  $$ |$$$$$$$\  
//        \$$$$$$\   $$$$$$$ |\$$$$$$\  $$ |  $$ |$$ |  $$ |$$ |  $$ |$$  __$$\ 
//         \____$$\ $$  __$$ | \____$$\ $$ |  $$ |$$ |  $$ |$$ |  $$ |$$ /  $$ |
//        $$$$$$$  |\$$$$$$$ |$$$$$$$  |$$ |  $$ |\$$$$$$  |\$$$$$$  | $$$$$$  |
//        \_______/  \_______|\_______/ \__|  \__| \______/  \______/  \______/
//
// Project name: 3-factor authenticated lock (Arduino code)
// Author: Sashwat K
// Created On: 09 Mar 2019
// Revision: 2
// Last Edited: 09 Mar 2019

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <Keypad.h>

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

byte myIFID[4] = {145,14,128,185};

int firstAuthentication = A1;
int thirdAuthentication = A2;

char valKey;
int temp,n;
long passCode = 0;
long correctPassCode = 8454842;
int state1 = 0;
int state2 = 0;
int state3 = 0;
boolean state1_state;
boolean state2_state;

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
 
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  pinMode(firstAuthentication, INPUT);
  pinMode(thirdAuthentication, OUTPUT);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vsh's 3 - Factor");
  lcd.setCursor(0, 1);
  lcd.print("Authetication");
  delay(2000);
  lcd.clear();
}

void loop() {
  state1 = rfid_find();
  Serial.println(state1);
  if (state1 == 1) {
    if (state1_state == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("NFC tag");
      lcd.setCursor(0, 1);
      lcd.print("Recognised");
      state1_state = 1;
      delay(1000);
      lcd.clear();
    }
    state2 = keypad_authentication();
    Serial.println(state2);
    if (state2 == 1) {
      analogWrite(thirdAuthentication,100);
      Serial.println("Authentication complete");
    }
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Place");
    lcd.setCursor(0, 1);
    lcd.print("NFC tag");
    delay(500);
  }
}

boolean rfid_find() {
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;
    
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  
  if (rfid.uid.uidByte[0] == myIFID[0] || 
    rfid.uid.uidByte[1] == myIFID[1] || 
    rfid.uid.uidByte[2] == myIFID[2] || 
    rfid.uid.uidByte[3] == myIFID[3] ) {
    return 1;
  }
  else {
    return 0;
  }
}

boolean keypad_authentication() {
  int firstState = analogRead(firstAuthentication);
  if (state2_state == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter key");
    delay(1000);
    state2_state = 1;
  }
  if (firstState >= 500) {
    int i=0;
    valKey = keypad.getKey();
    if (valKey) {
      if (valKey == '#') {
        passCode = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("cleared");
        digitalWrite(thirdAuthentication, LOW);
        n=0;
        delay(500);
        lcd.clear();
        state2_state = 0;
      }
      else if(valKey == '*') {
        if(passCode == correctPassCode) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Access granted");
          lcd.setCursor(0, 1);
          return 1;
        }
        else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Access denied");
          digitalWrite(thirdAuthentication, LOW);
          delay(500);
          state2_state = 0;
          return 0;
        }
      }
      else {
        temp = valKey - '0';
        passCode = (passCode * 10) + temp;
        n++;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter passcode: ");
        lcd.setCursor(0, 1);
        while(i<n) {
          lcd.print("*");
          i++;
        }
        delay(500);
      }
    }
  }
  else {
    passCode = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Key switch");
    lcd.setCursor(0, 1);
    lcd.print("OFF");
    delay(500);
    lcd.clear();
    state2_state = 0;
  }
  delay(10);
}
