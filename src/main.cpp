#include <Arduino.h>

#include "WiFi.h"

#include "cJSON.h"

#include <BLEDevice.h>
#include <BLE2902.h>

#define STA_SSID "NinoC137"
#define STA_PASS "zyx666...+"

WiFiClient client; // ESP32设置为客户端, TCP连接服务器
const IPAddress serverIP(192, 168, 1, 1);
uint16_t serverPort = 8888;

String readTCP;

#define bleServer "BLE_SERVER"
static BLEUUID ServiceUUID("ab1ad444-6724-11e9-a923-1681be663d3e");                                                                                 // 服务的UUID
BLECharacteristic RX_Characteristics("ab1ad980-6724-11e9-a923-1681be663d3e", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE); // 接收字符串的特征值
BLEDescriptor RX_Descriptor(BLEUUID((uint16_t)0x2901));                                                                                             // 接收字符串描述符
BLECharacteristic TX_Characteristics("ab1adb06-6724-11e9-a923-1681be663d3e", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE); // 发送字符串的特征值
BLEDescriptor TX_Descriptor(BLEUUID((uint16_t)0x2902));                                                                                             // 发送字符串描述符

std::string value;
char *json_string;
int cJsonParseEnd;

bool connected_state = false; // 创建设备连接标识符

class MyServerCallbacks : public BLEServerCallbacks // 创建连接和断开调用类
{
  void Connected(BLEServer *pServer) // 开始连接函数
  {
    connected_state = true; // 设备正确连接
  }
  void Disconnected(BLEServer *pServer) // 断开连接函数
  {
    connected_state = false; // 设备连接错误
  }
};

class MyCallbacks : public BLECharacteristicCallbacks
{

  void onWrite(BLECharacteristic *pCharacteristic)
  {                                      // 写方法
    value = pCharacteristic->getValue(); // 接收值
    if(value.empty())
    {

    }
  }
};

/****************************************************************************/
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(STA_SSID, STA_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("IPv4 address:");
  Serial.println(WiFi.localIP());

  BLEDevice::init(bleServer);                                 // 初始化BLE客户端设备
  BLEDevice::setMTU(256);
  BLEServer *pServer = BLEDevice::createServer();             // BLEServer指针，创建Server
  BLEService *pService = pServer->createService(ServiceUUID); // BLEService指针，创建Service

  pServer->setCallbacks(new MyServerCallbacks()); // 设置连接和断开调用类

  pService->addCharacteristic(&RX_Characteristics);
  RX_Descriptor.setValue("RX String");
  RX_Characteristics.addDescriptor(new BLE2902());

  pService->addCharacteristic(&TX_Characteristics);
  TX_Descriptor.setValue("TX String");
  TX_Characteristics.addDescriptor(new BLE2902());

  RX_Characteristics.setCallbacks(new MyCallbacks()); // 设置回调函数
  TX_Characteristics.setCallbacks(new MyCallbacks()); // 设置回调函数

  RX_Characteristics.setValue("Hello World"); // 发送信息，hello world
  TX_Characteristics.setValue("Hello World"); // 发送信息，hello world

  pService->start();
  pServer->getAdvertising()->start();
}

void loop()
{
  if (value.length() > 0)
  {
    json_string = (char*)value.data();
    Serial.printf("json string: %s\r\n value: %s\r\n", json_string, value.c_str());
    cJSON *root = cJSON_Parse(json_string);
    if (root == NULL) {
        Serial.printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    }
    cJSON *cmd = cJSON_GetObjectItem(root, "cmd");
    cJSON *idx = cJSON_GetObjectItem(root, "idx");
    cJSON *ssid = cJSON_GetObjectItem(root, "ssid");

    Serial.printf("cmd: %d\r\n", cmd->valueint);
    Serial.printf("idx: %d\r\n", idx->valueint);
    Serial.printf("ssid: %s\r\n", ssid->valuestring);

    TX_Characteristics.setValue("received!");
    TX_Characteristics.notify();
    
    cJSON_Delete(root);
    value.clear();
  }
  delay(500);
}