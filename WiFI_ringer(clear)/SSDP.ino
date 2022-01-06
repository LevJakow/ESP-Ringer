void SSDP_init(void) {
  // SSDP дескриптор
  HTTP.on("/description.xml", HTTP_GET, []() {
    SSDP.schema(HTTP.client());
  });
  //Если версия  2.0.0 закоментируйте следующую строчку
  SSDP.setDeviceType("upnp:rootdevice");
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName(configs._SSDP_Name);
  SSDP.setSerialNumber("0009092021");
  SSDP.setURL("/");
  SSDP.setModelName("SSDP-Test");
  SSDP.setModelNumber("000000000001");
  SSDP.setModelURL("http://Ringer.local");
  SSDP.setManufacturer("J@koW");
  SSDP.setManufacturerURL("megaman07@mail.ru");
  SSDP.begin();
}
