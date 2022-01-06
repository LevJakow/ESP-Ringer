BLYNK_WRITE(V1) //функция, отслеживающая изменение виртуального пина 1
{
  delay(1000);
  Serial.print("Выполнена команда BLYNK: play=");
  Serial.println(param.asString());
  switch (param.asInt()){
    case 1: mp3_play(configs._key1);
      break;
    case 2: mp3_play(configs._key2);
      break;
    case 3: mp3_play(configs._key3);
      break;
    case 4: mp3_play(configs._key4);
      break;
    default: mp3_play(param.asInt());
  }
  rgbblink(255,0,255);
}
