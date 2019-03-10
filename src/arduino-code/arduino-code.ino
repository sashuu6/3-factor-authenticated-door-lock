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

int firstAuthentication = 10;
int secondAuthentication = 11;

char key;
int temp,n;
long passCode = 0;
long correctPassCode = 8454842;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(firstAuthentication, INPUT);
  pinMode(secondAuthentication, OUTPUT);
  lcd.clear();
}

void loop() {
  int firstState = digitalRead(firstAuthentication);
  if (firstState == 1) {
    int i=0;
    key = keypad.getKey();
    if (key) {
      if (key == '#') {
        passCode = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("cleared");
        n=0;
        delay(500);
      }
      else if(key == '*') {
        if(passCode == correctPassCode) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Access granted");
          digitalWrite(secondAuthentication, HIGH);
          Serial.println("DONE");
          delay(500);
        }
        else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Access denied");
          delay(500);
        }
      }
      else {
        temp = key - '0';
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
    lcd.setCursor(0, 0);
    lcd.print("Key switch");
    lcd.setCursor(0, 1);
    lcd.print("OFF");
    delay(500);
    lcd.clear();
  }
}
