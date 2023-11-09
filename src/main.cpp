#include <Arduino.h>
#include "WiFi_BLE.h"

/****************************************************************************/
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  WiFi_Data.WiFi_store[0].SSID = (char*)STA_SSID;
  WiFi_Data.WiFi_store[0].PassWord = (char*)STA_PASS;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WiFi_Data.WiFi_store[0].SSID, WiFi_Data.WiFi_store[0].PassWord);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  WiFi_BLE_setUp();

  // http.begin("http://iot.lyhctech.com:6588"); //连接服务器对应域名
}

void loop()
{
  BLEHandler();
  WiFiHandler();
  delay(500);
}