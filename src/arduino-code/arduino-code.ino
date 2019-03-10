//                                      $$\                            $$$$$$\  
//                                      $$ |                          $$  __$$\ 
//         $$$$$$$\  $$$$$$\   $$$$$$$\ $$$$$$$\  $$\   $$\ $$\   $$\ $$ /  \__|
//        $$  _____| \____$$\ $$  _____|$$  __$$\ $$ |  $$ |$$ |  $$ |$$$$$$$\  
//        \$$$$$$\   $$$$$$$ |\$$$$$$\  $$ |  $$ |$$ |  $$ |$$ |  $$ |$$  __$$\ 
//         \____$$\ $$  __$$ | \____$$\ $$ |  $$ |$$ |  $$ |$$ |  $$ |$$ /  $$ |
//        $$$$$$$  |\$$$$$$$ |$$$$$$$  |$$ |  $$ |\$$$$$$  |\$$$$$$  | $$$$$$  |
//        \_______/  \_______|\_______/ \__|  \__| \______/  \______/  \______/
//
// Project name: 3-factor authenticated lock
// Author: Sashwat K
// Created On: 09 Mar 2019
// Revision: 1
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

char key;
int temp;
long passCode = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {
  key = keypad.getKey();
  if (key) {
    if (key == '#') {
      passCode = 0;
      lcd.clear();
      Serial.print("passCode cleared");
    }
    else {
      temp = key - '0';
      passCode = (passCode * 10) + temp;
      Serial.println(passCode);
      lcd.clear();
      lcd.print("*");
      delay(1500);
    }
  }
}
