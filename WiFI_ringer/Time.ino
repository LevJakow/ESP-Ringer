void timeSynch(int zone){
  if (WiFi.status() == WL_CONNECTED) {
    // Настройка соединения с NTP сервером
    configTime(zone * 3600, 0, "pool.ntp.org", "ru.pool.ntp.org");
    int i = 0;
    Serial.println("\nОжидается ответ сервера времени");
    while (!time(nullptr) && i < 100) {
      Serial.print(".");
      i++;
      delay(100);
    }
    delay(6000);
    Serial.println("Ответ сервера времени получен:");
    setTime(time(nullptr));
    Serial.println(GetTime());
    Serial.println(GetDate());
  }
}

// Получение текущего времени
String GetTime() {
 time_t now = time(nullptr); // получаем время с помощью библиотеки time.h
 String Times = ""; // Строка для результатов времени
 Times += ctime(&now); // Преобразуем время в строку формата Thu Jan 19 00:55:35 2017
 int i = Times.indexOf(":"); //Ишем позицию первого символа :
 Times = Times.substring(i - 2, i + 6); // Выделяем из строки 2 символа перед символом : и 6 символов после
 return Times; // Возврашаем полученное время
}

// Получение даты
String GetDate() {
 time_t now = time(nullptr); // получаем время с помощью библиотеки time.h 
 String Data = ""; // Строка для результатов времени
 Data += ctime(&now); // Преобразуем время в строку формата Thu Jan 19 00:55:35 2017
 int i = Data.lastIndexOf(" "); //Ишем позицию последнего символа пробел
 String Times = Data.substring(i - 8, i+1); // Выделяем время и пробел
 Data.replace(Times, ""); // Удаляем из строки 8 символов времени и пробел
 return Data; // Возврашаем полученную дату
}
