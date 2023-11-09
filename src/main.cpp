#include <Arduino.h>

#include "WiFi.h"

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
  { // 写方法

    std::string value = pCharacteristic->getValue(); // 接收值
    if (value.length() > 0)
    {
      Serial.println("*********");
      Serial.print("new string: ");
      for (int i = 0; i < value.length(); i++) // 遍历输出字符串
        Serial.print(value[i]);

      Serial.println();
      Serial.println("*********");
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
  Serial.print("IP地址:");
  Serial.println(WiFi.localIP());

  BLEDevice::init(bleServer); // 初始化BLE客户端设备

  BLEServer *pServer = BLEDevice::createServer();             // BLEServer指针，创建Server
  pServer->setCallbacks(new MyServerCallbacks());             // 设置连接和断开调用类
  BLEService *pService = pServer->createService(ServiceUUID); // BLEService指针，创建Service

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
    Serial.print("IP Address:");
  Serial.println(WiFi.localIP());
  delay(500);
  // Serial.println("连接服务器");
  // if (client.connect(serverIP, serverPort)) // 我们连接服务器对应的IP地址和端口
  // {
  //   Serial.println("连接服务器成功");
  //   while (client.connected() || client.available()) // 如果已连接，或有收到的未读取的数据
  //   {
  //     if (client.available()) // 如果有收到数据
  //     {
  //       readTCP = client.readString(); // 读取服务器发送的数据
  //       Serial.println(readTCP);       // 串口输出服务器发送来的数据
  //       readTCP = "";                  // 将接收到的字符清空
  //     }
  //   }
  //   Serial.println("关闭当前连接");
  //   client.stop(); // 关闭客户端
  // }
  // else
  // {
  //   Serial.println("连接服务器失败");
  //   client.stop(); // 关闭客户端
  // }
  // delay(5000);
}