#include <ESP8266WiFi.h>        
#include <ESP8266WebServer.h> 
#include <ESP8266HTTPUpdateServer.h>  
#include <WiFiClient.h>
#include <ESP8266SSDP.h>    
#include <ESP8266mDNS.h>    
#include <FS.h>                 
#include <ArduinoJson.h>     
#include <ESP8266HTTPUpdateServer.h> 
#include <DFPlayer_Mini_Mp3.h>
#include <TimeLib.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_NeoPixel.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();
// Web интерфейс для устройства
ESP8266WebServer HTTP(80);
ESP8266HTTPUpdateServer httpUpdater;
WiFiClient client;
MDNSResponder mdns;
char auth[] = "*************************"; //тут токен из e-mail
// Для файловой системы
File fsUploadFile;
struct Mode {
  byte _start = 1; //триггер начала дня
  byte _stop = 1; //триггер конца рабочего дня
  byte _hello = 1; //триггер приветствия
  byte _days = 1; //триггер дня недели: пн, вт...
  byte _history = 1; //триггер исторической справки
  byte _light = 1; //триггер световой индикации
  byte _mdns = 1; //режим видимости mdns
  byte _blynk = 1; //режим видимости blynk
};
struct Config {
  String _ssid     = "Your_wifi"; // Для хранения SSID
  String _password = "Your_wifi_key"; // Для хранения пароля сети
  String _ssidAP = "ESP-Ringer";   // SSID AP точки доступа
  String _passwordAP = ""; // пароль точки доступа
  String _MDNS_Name = "Ringer";
  String _SSDP_Name = "СЦ"; // Имя организации
  String _logo = "/null.png"; // логотип организации
  String _user1 = "/null.png";
  String _user2 = "/null.png"; //
  String _user3 = "/null.png"; //
  int _key1 = 1;
  int _key2 = 2;
  int _key3 = 3;
  int _key4 = 4;
  int _code1 = 4456640;
  int _code2 = 4456496;
  int _code3 = 4456460;
  int _code4 = 4456451;
  String clientIP = ""; 
  int timezone = 4;               // часовой пояс GTM
  String timeStart = "09:00:00";
  String timeStop = "18:00:00";
  int volumn = 30; //громкость динамиков
};
String last_rf_value;
#define NUM_LEDS 1

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, D5, NEO_GRB + NEO_KHZ800);
Config configs;
Mode modes;
// Определяем переменные wifi

String jsonConfig = "{}";
SoftwareSerial mp3Serial(D1, D2);  // RX and TX

void setup() {
  Serial.begin(115200);
  Serial.println("");
  pinMode(D3, INPUT);
  mySwitch.enableReceive(D6);//включаем модуль RF
  FS_init();   //Запускаем файловую систему
  loadConfig();
  if (modes._light == 1) pixels.begin();
  //иницализация mp3 модуля
  mp3Serial.begin (9600);
  mp3_set_serial (mp3Serial); 
  delay(1000); 
  mp3_set_volume (configs.volumn);
  //Запускаем WIFI
  WIFIinit();
  //Настраиваем и запускаем SSDP интерфейс
  SSDP_init();
  if ((modes._mdns == 1)&&(MDNS.begin(configs._MDNS_Name, WiFi.localIP()))) {
    Serial.println("MDNS Started");
    MDNS.addService("http", "tcp", 80);
  }
  httpUpdater.setup(&HTTP);
  //Настраиваем и запускаем HTTP интерфейс
  HTTP_init();
  if (modes._blynk == 1) Blynk.run(); //запускаем работу blynk
  PultReset(); //перезапускаем удаленный пульт
}

void loop() {
  // Serial.print(hour());Serial.print(":");
  //Serial.print(minute());Serial.print(":");
  // Serial.print(second());Serial.println(".");
  //MDNS.update();
  // Check if a client has connected
  HTTP.handleClient();
  if (modes._mdns == 1) MDNS.update();
  if (GetTime() == "08:50:00") ESP.restart(); //перезапуск ESP каждый день
  if (GetTime() == configs.timeStart) daybegin(); //оповещение о начале рабочего дня
  if ((modes._stop == 1)&&(GetTime() == configs.timeStop)){ //оповещение о конце рабочего дня
    Serial.println("Пора домой!");
    mp3_play(150+random(4));
    delay(1000);
  }
  rf_svitch();
}

void daybegin(){
   if (modes._start == 1){
    Serial.println("Приветствие");
    mp3_play(120);
    delay(100);
    while (!digitalRead(D3)) delay (10);
   }
   if (modes._days == 1) {
    Serial.println("Сегодня *день недели*");
    mp3_play(130+weekday());
    delay(100);
    while (!digitalRead(D3)) delay (10);
   }
   if (modes._hello == 1) {
     Serial.println("Пожелание");
     mp3_play(140+random(6));
     delay(100);
     while (!digitalRead(D3)) delay (10);
   }
   if (modes._history == 1) {
     Serial.println("В этот день...");
     mp3_play(1000+50*(month()-1)+day());
     delay(100);
     while (!digitalRead(D3)) delay (10);
   }
}
