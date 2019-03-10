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

int switchKey = D0;
int firstAuthentication = D1;
int thirdAuthentication = D2;

void setup() {
  Serial.begin(9600);
  pinMode(switchKey, INPUT);
  pinMode(firstAuthentication, INPUT);
}

void loop() {
  int keySwitchState = digitalRead(switchKey);
  Serial.print("Key switch: ");
  Serial.println(keySwitchState);
  if(keySwitchState == 1) {
    digitalWrite(firstAuthentication, HIGH);
  }
  else {
    digitalWrite(firstAuthentication, LOW);
  }
  
}
