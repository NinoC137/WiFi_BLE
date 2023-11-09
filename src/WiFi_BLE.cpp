#include "WiFi_BLE.h"

WiFiClient client; // ESP32设置为客户端, TCP连接服务器
const IPAddress serverIP(192, 168, 1, 1);
uint16_t serverPort = 8888;

String readTCP;

BLEUUID ServiceUUID("ab1ad444-6724-11e9-a923-1681be663d3e");                                                                                        // 服务的UUID
BLECharacteristic RX_Characteristics("ab1ad980-6724-11e9-a923-1681be663d3e", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE); // 接收字符串的特征值
BLEDescriptor RX_Descriptor(BLEUUID((uint16_t)0x2901));                                                                                             // 接收字符串描述符
BLECharacteristic TX_Characteristics("ab1adb06-6724-11e9-a923-1681be663d3e", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE); // 发送字符串的特征值
BLEDescriptor TX_Descriptor(BLEUUID((uint16_t)0x2902));                                                                                             // 发送字符串描述符

std::string value;
char *json_string;
int cJsonParseEnd;

WiFiData WiFi_Data;
// HTTP访问域名对象
HTTPClient http;

bool connected_state = false; // 创建设备连接标识符

void MyServerCallbacks::Connected(BLEServer *pServer) // 开始连接函数
{
    connected_state = true; // 设备正确连接
}
void MyServerCallbacks::Disconnected(BLEServer *pServer) // 断开连接函数
{
    connected_state = false; // 设备连接错误
}

void MyCallbacks::onWrite(BLECharacteristic *pCharacteristic) // 写方法
{
    value = pCharacteristic->getValue(); // 接收值
    if (value.empty())
    {
    }
}

void cmd1();
void cmd2(cJSON *root);

void WiFi_BLE_setUp()
{
    WiFi_Data.WiFi_store[0].ipv4 = WiFi.localIP();
    WiFi_Data.WiFi_store[0].MacAddress = WiFi.macAddress();

    Serial.print("IPv4 address:");
    Serial.println(WiFi_Data.WiFi_store[0].ipv4);
    Serial.print("Mac:");
    Serial.println(WiFi_Data.WiFi_store[0].MacAddress);

    BLEDevice::init(bleServer); // 初始化BLE客户端设备
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

    RX_Characteristics.setValue("RX init"); // 发送信息
    TX_Characteristics.setValue("TX init"); // 发送信息

    pService->start();
    pServer->getAdvertising()->start();
}

void BLEHandler()
{
    // 蓝牙信息处理部分  --  注意手机发送端的MTU应设置为256(反正不要是默认的23字节,json包发不过去)
    if (value.length() > 0)
    {
        json_string = (char *)value.data();
        Serial.printf("json string: %s\r\n value: %s\r\n", json_string, value.c_str());
        cJSON *root = cJSON_Parse(json_string);
        if (root == NULL)
        {
            Serial.printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        }
        cJSON *cmd = cJSON_GetObjectItem(root, "cmd");
        Serial.printf("cmd: %d\r\n", cmd->valueint);

        switch (cmd->valueint)
        {
        case 1:
            cmd1();
            break;
        case 2:
            cmd2(root);
            break;
        default:
            Serial.printf("error cmd!\r\n");
            TX_Characteristics.setValue("received cmd: ERROR!");
            TX_Characteristics.notify();
            break;
        }

        cJSON_Delete(root);
        value.clear();
    }
}

void WiFiHandler()
{
    // WIFI连接服务器部分
    int httpCode = http.GET();
    if (httpCode > 0) // HTTP请求无异常
    {
        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString(); // 读取get到的json串

            Serial.println(payload);
        }
    }
}

void cmd1()
{
    TX_Characteristics.setValue("received cmd:1");
    TX_Characteristics.notify();
}

void cmd2(cJSON *root)
{
    TX_Characteristics.setValue("received cmd:2");
    TX_Characteristics.notify();
    cJSON *idx = cJSON_GetObjectItem(root, "idx");
    cJSON *ssid = cJSON_GetObjectItem(root, "ssid");
    Serial.printf("idx: %d\r\n", idx->valueint);
    Serial.printf("ssid: %s\r\n", ssid->valuestring);
}