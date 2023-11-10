#include <Arduino.h>
#include "WiFi_BLE.h"

/****************************************************************************
注意: 大多数手机BLE 调试APP的默认MTU都是23, 需要改大一些(越大越好, 我是设定为了256)
应用于FOC_LVGL项目中的部分代码进行了调整, 大多在cmd_Parse.cpp之中
默认都加在了代码最后面的部分
****************************************************************************/
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi_Data.WiFi_store[0].SSID = (char *)STA_SSID;
  WiFi_Data.WiFi_store[0].PassWord = (char *)STA_PASS;

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
  ProjectDataUpdate();

  delay(5);
}