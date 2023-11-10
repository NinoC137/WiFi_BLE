#ifndef _WIFI_BLE_H
#define _WIFI_BLE_H

#define DEBUG 1

#include <sstream>

#include "WiFi.h"
#include "HTTPClient.h"

#include "cJSON.h"
#include "cmd_Parse.h"

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
    char* devType = (char*)"esp32";
    IPAddress ipv4;
    std::string MacAddress;
    std::string devID;
  };

  std::string serverip;
  int serverport;

  WiFiIndex WiFi_store[3];
};

class HeartBeatPacket{  //心跳包
public:
  long keepAliveTime;
  long keepLiveCnt;
};

class ProjectDataPacket{
public:
  bool blestatus;
  bool wifistatus;
  bool switchStatus;
  int speed;
  int temp;
  std::string time;

  int worktime; //单位为秒
  int runTime;  //单位为秒

  std::string device_ID;
};

class MyServerCallbacks : public BLEServerCallbacks // 创建连接和断开调用类
{
public:
  void onConnect(BLEServer *pServer); // 开始连接函数
  void onDisconnect(BLEServer *pServer); // 断开连接函数
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

extern HeartBeatPacket HeartBeat;
extern ProjectDataPacket ProjectData;

extern int my_timezone;
extern long gmtOffset_sec;
extern struct tm timeinfo;

extern std::string value;
extern char *json_string;
extern int cJsonParseEnd;

extern BLEUUID ServiceUUID;
extern BLECharacteristic RX_Characteristics;
extern BLEDescriptor RX_Descriptor;
extern BLECharacteristic TX_Characteristics;
extern BLEDescriptor TX_Descriptor;

void WiFi_BLE_setUp();  //在WiFi开启之后调用此函数
void BLEHandler();
void WiFiHandler();
void ProjectDataUpdate();
void HeartBeatUpdate();

void updateLocalTime();

#endif // !_WIFI_BLE_H