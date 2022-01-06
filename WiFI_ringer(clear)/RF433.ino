void rf_svitch() {
  if (mySwitch.available()) {
    int value = mySwitch.getReceivedValue();
     if (value == 0) {
      Serial.println("Zerro RF Command");
    } else {
      Serial.print("Input command: ");
      Serial.println( value );
      last_rf_value = String(value);
      if (value == configs._code1) {
        mp3_play(configs._key1);
        rgbblink(0,255,0);
       } else
       if (value == configs._code2) {
        mp3_play(configs._key2); 
        rgbblink(0,255,0);
       } else
       if (value == configs._code3) {
        mp3_play(configs._key3); 
        rgbblink(0,255,0);
       } else
       if (value == configs._code4) {
        mp3_play(configs._key4);
        rgbblink(255,0,0);
       } else {
        Serial.println("Unknown code! ");
       }
      delay(1000);  
    }
    mySwitch.resetAvailable();
  }
}
