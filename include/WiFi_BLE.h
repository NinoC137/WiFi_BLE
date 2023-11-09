#ifndef _WIFI_BLE_H
#define _WIFI_BLE_H

#include "WiFi.h"
#include "HTTPClient.h"

#include "cJSON.h"

#include <BLEDevice.h>
#include <BLE2902.h>

#define bleServer "BLE_SERVER"
#define STA_SSID "NinoC137"
#define STA_PASS "zyx666...+"

class WiFiData{
public:
  struct WiFiIndex
  {
    int idx;
    char* SSID;
    char* PassWord;
    IPAddress ipv4;
    String MacAddress;
  };

  WiFiIndex WiFi_store[3];
};

class MyServerCallbacks : public BLEServerCallbacks // 创建连接和断开调用类
{
public:
  void Connected(BLEServer *pServer); // 开始连接函数
  void Disconnected(BLEServer *pServer); // 断开连接函数
};

class MyCallbacks : public BLECharacteristicCallbacks
{
public:
  void onWrite(BLECharacteristic *pCharacteristic);
};

extern WiFiData WiFi_Data;
extern WiFiClient client;
extern HTTPClient http;
extern String readTCP;

extern std::string value;
extern char *json_string;
extern int cJsonParseEnd;

extern BLEUUID ServiceUUID;
extern BLECharacteristic RX_Characteristics;
extern BLEDescriptor RX_Descriptor;
extern BLECharacteristic TX_Characteristics;
extern BLEDescriptor TX_Descriptor;

void WiFi_BLE_setUp();
void BLEHandler();
void WiFiHandler();

#endif // !_WIFI_BLE_H