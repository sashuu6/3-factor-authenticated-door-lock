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
int firstAuthentication = D3;
int thirdAuthentication = D4;

void setup() {
  Serial.begin(115200);
  pinMode(switchKey, INPUT);
  pinMode(firstAuthentication, OUTPUT);
  pinMode(thirdAuthentication, INPUT);
}

void loop() {
  int keySwitchState = digitalRead(switchKey);
  int thirdAuthenticationState = digitalRead(thirdAuthentication);
  if(keySwitchState == 1) {
    digitalWrite(firstAuthentication, HIGH);
    Serial.print("Key switch: ");
    Serial.println(keySwitchState);
    if(thirdAuthenticationState == 1) {
      Serial.println("2nd authentication complete");
    }
    else {
      Serial.println("2nd authentication incomplete");
    }
  }
  else {
    digitalWrite(firstAuthentication, LOW);
  }
  
}
