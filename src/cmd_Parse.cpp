#include "cmd_Parse.h"

void cmd1()
{
    TX_Characteristics.setValue("received cmd:1");
    TX_Characteristics.notify();
}

void cmd2(cJSON *root)
{
    TX_Characteristics.setValue("received cmd:2");
    TX_Characteristics.notify();
    cJSON *cmd_idx = cJSON_GetObjectItem(root, "idx");
    cJSON *cmd_ssid = cJSON_GetObjectItem(root, "ssid");
    cJSON *cmd_pws = cJSON_GetObjectItem(root, "pws");
    if (cmd_idx == NULL || cmd_ssid == NULL || cmd_pws == NULL)
    {
        TX_Characteristics.setValue("json string error!!");
        TX_Characteristics.notify();
        return;
    }
    // 将数据存储至全局变量
    WiFi_Data.WiFi_store[cmd_idx->valueint].SSID = cmd_ssid->valuestring;
    WiFi_Data.WiFi_store[cmd_idx->valueint].PassWord = cmd_pws->valuestring;
#if DEBUG
    Serial.printf("idx: %d\r\n", cmd_idx->valueint);
    Serial.printf("ssid: %s\r\n", cmd_ssid->valuestring);
#endif // DEBUG
}

void cmd3(cJSON *root) // 读取设备保存的WiFi(一个)
{
    TX_Characteristics.setValue("received cmd:3");
    TX_Characteristics.notify();
}

void cmd4(cJSON *root) // 启/停设备
{
    TX_Characteristics.setValue("received cmd:4");
    TX_Characteristics.notify();
    cJSON *cmd_switch = cJSON_GetObjectItem(root, "switch");
    cJSON *cmd_worktime = cJSON_GetObjectItem(root, "worktime");
    if (cmd_switch == NULL || cmd_worktime == NULL)
    {
        TX_Characteristics.setValue("json string error!!");
        TX_Characteristics.notify();
        return;
    }
#if DEBUG
    Serial.printf("switch: %d\r\n", cmd_switch->valueint);
    Serial.printf("worktime: %d\r\n", cmd_worktime->valueint);
#endif // DEBUG
}

void cmd5(cJSON *root) // 设置时间
{
    TX_Characteristics.setValue("received cmd:5");
    TX_Characteristics.notify();

    cJSON *cmd_year = cJSON_GetObjectItem(root, "year");
    cJSON *cmd_month = cJSON_GetObjectItem(root, "month");
    cJSON *cmd_day = cJSON_GetObjectItem(root, "day");
    cJSON *cmd_hour = cJSON_GetObjectItem(root, "hour");
    cJSON *cmd_minute = cJSON_GetObjectItem(root, "minute");
    cJSON *cmd_second = cJSON_GetObjectItem(root, "second");

    if (cmd_year == NULL || cmd_month == NULL || cmd_day == NULL || cmd_hour == NULL || cmd_minute == NULL || cmd_second == NULL)
    {
        TX_Characteristics.setValue("json string error!!");
        TX_Characteristics.notify();
        return;
    }

    std::stringstream stringstream;
    stringstream << cmd_year->valueint << "-"
                 << cmd_month->valueint << "-"
                 << cmd_day->valueint << " "
                 << cmd_hour->valueint << ":"
                 << cmd_minute->valueint << ":"
                 << cmd_second->valueint;

    ProjectData.time = stringstream.str();

#if DEBUG
    Serial.printf("year: %d\r\n", cmd_year->valueint);
    Serial.printf("minute: %d\r\n", cmd_minute->valueint);
#endif // DEBUG
}

void cmd6(cJSON *root) // 设置时区
{
    TX_Characteristics.setValue("received cmd:6");
    TX_Characteristics.notify();
    cJSON *cmd_timezone = cJSON_GetObjectItem(root, "timezone");
    if (cmd_timezone == NULL)
    {
        TX_Characteristics.setValue("json string error!!");
        TX_Characteristics.notify();
        return;
    }
#if DEBUG
    Serial.printf("timezone: %d\r\n", cmd_timezone->valueint);
#endif // DEBUG
}

void cmd7(cJSON *root) // 查询时区
{
    TX_Characteristics.setValue("received cmd:7");
    TX_Characteristics.notify();
}

void cmd8(cJSON *root) // 查询设备信息
{
    TX_Characteristics.setValue("received cmd:8");
    TX_Characteristics.notify();
}

void cmd9(cJSON *root) // 设置设备工作时长
{
    TX_Characteristics.setValue("received cmd:9");
    TX_Characteristics.notify();
    cJSON *cmd_worktime = cJSON_GetObjectItem(root, "worktime");
    cJSON *cmd_speed = cJSON_GetObjectItem(root, "speed");
    cJSON *cmd_temp = cJSON_GetObjectItem(root, "temp");
    if (cmd_worktime == NULL || cmd_speed == NULL || cmd_temp == NULL)
    {
        TX_Characteristics.setValue("json string error!!");
        TX_Characteristics.notify();
        return;
    }
    // 存入设备数据包
    ProjectData.worktime = cmd_worktime->valueint;
    ProjectData.speed = cmd_speed->valueint;
    ProjectData.temp = cmd_temp->valueint;
#if DEBUG
    Serial.printf("worktime: %d\r\n", cmd_worktime->valueint);
    Serial.printf("speed: %d\r\n", cmd_speed->valueint);
#endif // DEBUG
}

void cmd10(cJSON *root) // 查询设备剩余时长
{
    TX_Characteristics.setValue("received cmd:10");
    TX_Characteristics.notify();
#if DEBUG
    Serial.printf("Remaining time: %d", ProjectData.worktime - ProjectData.runTime);
#endif // DEBUG
}

void cmd12(cJSON *root) // 服务器设置
{
    TX_Characteristics.setValue("received cmd:12");
    TX_Characteristics.notify();
    cJSON *cmd_serverip = cJSON_GetObjectItem(root, "serverip");
    cJSON *cmd_serverport = cJSON_GetObjectItem(root, "serverport");
    
    if (cmd_serverip == NULL || cmd_serverport == NULL)
    {
        TX_Characteristics.setValue("json string error!!");
        TX_Characteristics.notify();
        return;
    }

    WiFi_Data.serverip = cmd_serverip->valuestring;
    WiFi_Data.serverport = cmd_serverport->valueint;
#if DEBUG
    Serial.printf("server ip: %s\r\n", cmd_serverip->valuestring);
    Serial.printf("server port: %d\r\n", cmd_serverport->valueint);
#endif // DEBUG
}

void cmd13(cJSON *root) // 读取服务器设置
{
    TX_Characteristics.setValue("received cmd:13");
    TX_Characteristics.notify();
}

void cmd14(cJSON *root) // 心跳包设置
{
    TX_Characteristics.setValue("received cmd:14");
    TX_Characteristics.notify();
}

void cmd15(cJSON *root) // 读取心跳包设置
{
    TX_Characteristics.setValue("received cmd:15");
    TX_Characteristics.notify();
}

void cmd16(cJSON *root) // 通过WiFi向服务器发送事件日志
{
    TX_Characteristics.setValue("received cmd:16");
    TX_Characteristics.notify();
}

void cmd17(cJSON *root) // 通过蓝牙向宿主机发送事件日志
{
    TX_Characteristics.setValue("received cmd:17");
    TX_Characteristics.notify();
}

void cmd19(cJSON *root) // 初始化设备信息
{
    TX_Characteristics.setValue("received cmd:19");
    TX_Characteristics.notify();
    cJSON *cmd_devName = cJSON_GetObjectItem(root, "devName");
    if (cmd_devName == NULL)
    {
        TX_Characteristics.setValue("json string error!!");
        TX_Characteristics.notify();
        return;
    }

    ProjectData.device_ID = cmd_devName->valuestring;
#ifdef DEBUG
    Serial.printf("dev Name: %s\r\n", cmd_devName->valuestring);
#endif // DEBUG
}