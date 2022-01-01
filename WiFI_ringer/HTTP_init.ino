
void HTTP_init(void) {
  HTTP.on("/temp.json", handle_TempJSON); // формирование configs.json страницы для передачи данных в web интерфейс
  // API для устройства
  HTTP.on("/ssdp", handle_Set_Ssdp);     // Установить имя SSDP устройства по запросу вида /ssdp?ssdp=proba
  HTTP.on("/ssid", handle_Set_Ssid);     // Установить имя и пароль роутера по запросу вида /ssid?ssid=home2&password=12345678
  HTTP.on("/ssidap", handle_Set_Ssidap); // Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439
  HTTP.on("/timers", handle_Set_Timers); //Установка времени начала и конца рабочего дня
  HTTP.on("/restart", handle_Restart);   // Перезагрузка модуля по запросу вида /restart?device=ok
  HTTP.on("/user1set", handle_user1);
  HTTP.on("/user2set", handle_user2);
  HTTP.on("/user3set", handle_user3);
  HTTP.on("/keys", handle_keys);
  HTTP.on("/logo", handle_logo);
  HTTP.on("/play", play_user);  
  HTTP.on("/volume", setvolume);  
  HTTP.on("/rf", last_rf);
  HTTP.on("/rfset", handle_rf);
  HTTP.on("/clientip",setclientip);
  HTTP.on("/Time", handle_Time);     // Синхронизировать время устройства по запросу вида /Time
  HTTP.on("/TimeZone", handle_time_zone);    // Установка временой зоны по запросу вида http://192.168.0.101/TimeZone?timezone=3
  HTTP.on("/mode",handle_mode);
  // Запускаем HTTP сервер
  HTTP.begin();
}
void handle_mode() { //установка ip клиента
  //modes._blynk = HTTP.arg("tblynk").toBool(); 
 // modes._mdns = HTTP.arg("tmdns").toBool();     
  saveConfig();           
  HTTP.send(200, "text/plain", "OK");  
}
void setclientip() { //установка ip клиента
  configs.clientIP = HTTP.arg("clientip");      
  saveConfig();           
  HTTP.send(200, "text/plain", "OK");  
}
void setvolume() { //установка громкости
  configs.volumn = HTTP.arg("volume").toInt();      
  saveConfig();           
  HTTP.send(200, "text/plain", "OK");  
  mp3_set_volume (configs.volumn);
}
void play_user() {
  int puser = 0; //номер проигрываемого звука пользователя
  puser = HTTP.arg("play").toInt();      
  Serial.println("Проигран "+String(puser)+"-й звук");
  HTTP.send(200, "text/plain", "OK"); 
  mp3_play(puser);
  rgbshow(0,0,255,NUM_LEDS);
  delay(3000);
  rgbshow(0,0,255,0);
}
void handle_keys() {
  configs._key1 = HTTP.arg("key1").toInt();
  configs._key2 = HTTP.arg("key2").toInt();
  configs._key3 = HTTP.arg("key3").toInt();
  configs._key4 = HTTP.arg("key4").toInt();
  saveConfig();
  HTTP.send(200, "text/plain", "OK");
}
void handle_logo() {
  configs._logo = HTTP.arg("logo"); // Получаем значение logo из запроса сохраняем в глобальной переменной
  saveConfig();                 // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");
}
void handle_user1() {
  configs._user1 = HTTP.arg("user1set"); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
  if (configs._user1.length()==0) configs._user1 = "/null.png";
  saveConfig();                 // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}
void handle_user2() {
  configs._user2 = HTTP.arg("user2set"); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
  if (configs._user2.length()==0) configs._user2 = "/null.png";
  saveConfig();                 // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}
void handle_user3() {
  configs._user3 = HTTP.arg("user3set"); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
  if (configs._user3.length()==0) configs._user3 = "/null.png";
  saveConfig();                 // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}
void handle_rf() { //записываем новые коды с кнопок пульта
  configs._code1 = HTTP.arg("code1").toInt();
  configs._code2 = HTTP.arg("code2").toInt();
  configs._code3 = HTTP.arg("code3").toInt();
  configs._code4 = HTTP.arg("code4").toInt();
  saveConfig();
  HTTP.send(200, "text/plain", "OK"); 
}
void last_rf(){ //передаем код последнего полученного кода с пульта
  HTTP.send(200, "text/plain", "!!!"+last_rf_value+"!!!"); 
}
// Функции API-Set
// Установка SSDP имени по запросу вида http://192.168.0.101/ssdp?ssdp=proba
void handle_Set_Ssdp() {
  configs._SSDP_Name = HTTP.arg("ssdp"); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
  saveConfig();                 // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}

void handle_Set_Timers() {
  configs.timeStart = HTTP.arg("timeStart")+":00";   // Получаем  время начала рабочего дня
  configs.timeStop = HTTP.arg("timeStop")+":00";    // Получаем  время конца рабочего дня
  saveConfig();                        // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

// Установка параметров для подключения к внешней AP по запросу вида http://192.168.0.101/ssid?ssid=home2&password=12345678
void handle_Set_Ssid() {
  configs._ssid = HTTP.arg("ssid");            // Получаем значение ssid из запроса сохраняем в глобальной переменной
  configs._password = HTTP.arg("password");    // Получаем значение password из запроса сохраняем в глобальной переменной
  saveConfig();                        // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}
//Установка параметров внутренней точки доступа по запросу вида http://192.168.0.101/ssidap?ssidAP=home1&passwordAP=8765439
void handle_Set_Ssidap() {              //
  configs._ssidAP = HTTP.arg("ssidAP");         // Получаем значение ssidAP из запроса сохраняем в глобальной переменной
  configs._passwordAP = HTTP.arg("passwordAP"); // Получаем значение passwordAP из запроса сохраняем в глобальной переменной
  saveConfig();                         // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}
// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handle_Restart() {
  String restart = HTTP.arg("device");          // Получаем значение device из запроса
  if (restart == "ok") {                         // Если значение равно Ок
    HTTP.send(200, "text / plain", "Reset OK"); // Oтправляем ответ Reset OK
    ESP.restart();                                // перезагружаем модуль
  }
  else {                                        // иначе
    HTTP.send(200, "text / plain", "No Reset"); // Oтправляем ответ No Reset
  }
}

void handle_time_zone() {               
  configs.timezone = HTTP.arg("timezone").toInt(); // Получаем значение timezone из запроса конвертируем в int сохраняем в глобальной переменной
  saveConfig();
  HTTP.send(200, "text/plain", "OK");
}

void handle_Time(){
  timeSynch(configs.timezone);
  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}

void PultReset() {
  if (client.connect(configs.clientIP, 80))
  {
    Serial.print("Перезапускаем ПДУ");
    client.println ("GET /restart?device=ok HTTP/1.0");
    client.println();
    delay(100);
  }
  client.stop();
}
void handle_TempJSON() {
  String root="";  // Формировать строку для отправки в браузер json формат
  //{"SSDP":"SSDP-test","ssid":"home","password":"i12345678","ssidAP":"WiFi","passwordAP":"","ip":"192.168.0.101"}
  // Резервируем память для json обекта буфер может рости по мере необходимти, предпочтительно для ESP8266
  DynamicJsonDocument json(1024); 
  deserializeJson(json, root);
  // Заполняем поля json
  json["ssidAP"] = configs._ssidAP;
  json["passwordAP"] = configs._passwordAP;
  json["ssid"] = configs._ssid;
  json["password"] = configs._password;
  json["MDNS"] = configs._MDNS_Name;
  json["SSDP"] = configs._SSDP_Name;
  json["timezone"] = configs.timezone;
  json["timeStart"] = configs.timeStart;
  json["timeStop"] = configs.timeStop;
  json["logo"] = configs._logo;
  json["user1"] = configs._user1;
  json["user2"] = configs._user2;
  json["user3"] = configs._user3;
  json["key1"] = configs._key1;
  json["key2"] = configs._key2;
  json["key3"] = configs._key3;
  json["key4"] = configs._key4;
  json["code1"] = configs._code1;
  json["code2"] = configs._code2;
  json["code3"] = configs._code3;
  json["code4"] = configs._code4;
  json["volume"] = configs.volumn;
  json["client"] = configs.clientIP;
  json["tstart"] = modes._start;
  json["tstop"] = modes._stop;
  json["thello"] =  modes._hello ; //триггер приветствия
  json["tdays"] = modes._days;
  json["thistory"] =  modes._history;
  json["tight"] =  modes._light;
  json["tmdns"] = modes._mdns;
  json["tblynk"] =  modes._blynk;
  json["time"] = GetTime();
  json["ip"] = WiFi.localIP().toString();
  // Помещаем созданный json в переменную root
  root = "";
  serializeJson(json,root);
  HTTP.send(3000, "text/json", root);
}
