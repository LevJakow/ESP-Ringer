void rgbshow(byte rgb_R,byte rgb_G,byte rgb_B, byte n)
{
  for(byte j = 0; j < NUM_LEDS; j = j + 1) pixels.setPixelColor(j, pixels.Color(0,0,0));
  for(byte j = 0; j < n; j = j + 1) pixels.setPixelColor(j, pixels.Color(rgb_R,rgb_G,rgb_B));
  pixels.show();
}
void rgbblink(byte rgb_R,byte rgb_G,byte rgb_B)
{
  rgbshow(rgb_R,rgb_G,rgb_B,NUM_LEDS);
  delay(3000);
  rgbshow(0,0,0,NUM_LEDS);
}
