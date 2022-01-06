void WIFIinit() {
  // Попытка подключения к точке доступа
  //WiFi.disconnect();
  //WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  byte tries = 50;
  WiFi.begin(configs._ssid.c_str(), configs._password.c_str());
  Serial.println("SSID: "+configs._ssid);
  Serial.println("Password: "+configs._password);
  // Делаем проверку подключения до тех пор пока счетчик tries
  // не станет равен нулю или не получим подключение
  delay(1000);
  while (--tries && WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    rgbshow(0,0,120,(50-tries)/3); //rgbshow(R,G,B,число ламп)
    delay(400);
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    // Если не удалось подключиться запускаем в режиме AP
    Serial.println("");
    mp3_play(102);
    delay(10);
    Serial.println("WiFi - режим точки доступа");
    StartAPMode();
    rgbblink(255,0,0);
  }
  else {
    // Иначе удалось подключиться отправляем сообщение
    // о подключении и выводим адрес IP
    delay(10);
    mp3_play(101);
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    rgbblink(0,255,0);
    timeSynch(configs.timezone);
    if (modes._blynk == 1) {
      Blynk.config(auth);
       while (Blynk.connect(1000) == false) { 
     }
    } 
    }
}

bool StartAPMode() {
  // Отключаем WIFI
  WiFi.disconnect();
  delay(1000);
//  WiFi.mode(WIFI_OFF);
  IPAddress apIP(192, 168, 4, 1);
 // delay(1000);
  // Меняем режим на режим точки доступа
  WiFi.mode(WIFI_AP);
  // Задаем настройки сети
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  // Включаем WIFI в режиме точки доступа с именем и паролем
  // хронящихся в переменных _ssidAP _passwordAP
  WiFi.softAP(configs._ssidAP.c_str(), configs._passwordAP.c_str());
  Serial.println("IP address: ");
  Serial.println(apIP);
  Serial.println("SSID точки доступа: ");
  Serial.println(configs._ssidAP.c_str());
  Serial.println("Пароль для подключения: ");
  Serial.println(configs._passwordAP.c_str());
  return true;
}
