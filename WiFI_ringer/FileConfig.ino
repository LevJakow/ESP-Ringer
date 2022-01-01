// Загрузка данных сохраненных в файл  config.json
bool loadConfig() {
  // Открываем файл для чтения
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
  // если файл не найден  
    Serial.println("Failed to open config file");
  //  Создаем файл записав в него даные по умолчанию
    saveConfig();
    configFile.close();
    return false;
  }
  // Проверяем размер файла, будем использовать файл размером меньше 1024 байта
  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    configFile.close();
    return false;
  }

// загружаем файл конфигурации в глобальную переменную
  jsonConfig = configFile.readString();
  configFile.close();
  // Резервируем памяь для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266 
    DynamicJsonDocument root(1024);
    deserializeJson(root, jsonConfig);
  // Теперь можно получить значения из root  
    configs._ssidAP = root["ssidAP"].as<String>(); // Так получаем строку
    configs._passwordAP = root["passwordAP"].as<String>();
    configs.timezone = root["timezone"];               // Так получаем число
    configs.timeStart = root["timeStart"].as<String>();
    configs.timeStop = root["timeStop"].as<String>();
    configs._SSDP_Name = root["SSDP"].as<String>();
    configs._ssid = root["ssid"].as<String>();
    configs._password = root["password"].as<String>();
    configs._MDNS_Name = root["MDNS"].as<String>();
    configs._logo = root["logo"].as<String>();
    configs._user1 = root["user1"].as<String>();
    configs._user2 = root["user2"].as<String>();
    configs._user3 = root["user3"].as<String>();
    configs._key1 = root["key1"].as<int>();
    configs._key2 = root["key2"].as<int>();
    configs._key3 = root["key3"].as<int>();
    configs._key4 = root["key4"].as<int>();
    configs._code1 = root["code1"].as<int>();
    configs._code2 = root["code2"].as<int>();
    configs._code3 = root["code3"].as<int>();
    configs._code4 = root["code4"].as<int>();
    configs.volumn = root["volume"].as<int>();
    configs.clientIP = root["client"].as<String>();
    modes._start = root["tstart"].as<byte>();
    modes._stop = root["tstop"].as<byte>(); //триггер конца рабочего дня
    modes._hello = root["thello"].as<byte>(); //триггер приветствия
    modes._days = root["tdays"].as<byte>(); //триггер дня недели: пн, вт...
    modes._history = root["thistory"].as<byte>(); //триггер исторической справки
    modes._light = root["tight"].as<byte>(); //триггер световойиндикации
    modes._mdns = root["tmdns"].as<byte>(); //режим видимости mdns
    modes._blynk = root["tblynk"].as<byte>(); //режим видимости blynk
    return true;
}

// Запись данных в файл config.json
bool saveConfig() {
  // Резервируем память для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266 
  DynamicJsonDocument json(1024);
  deserializeJson(json, jsonConfig);
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
  // Помещаем созданный json в глобальную переменную json.printTo(jsonConfig);
  serializeJson(json,jsonConfig);
  // Открываем файл для записи
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    configFile.close();
    return false;
  }
  // Записываем строку json в файл 
  serializeJson(json,configFile);
  configFile.close();
  return true;
  }
