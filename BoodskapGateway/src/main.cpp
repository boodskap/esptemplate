#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <BoodskapMQTT.h>
#include <MqttAdapter.h>

#define BAUD_RATE 115200
char WIFI_SSID[SSID_SIZE] = STR(BOODSKAP_WIFI_SSID);
char WIFI_PASSWORD[PASSWD_SUZE] = STR(BOODSKAP_WIFI_PSWD);
char BOODSKAP_HOST[IP_ADDR_SIZE] = STR(BOODSKAP_MQTT_HOST);
const uint16_t BOODSKAP_PORT = atoi(STR(BOODSKAP_MQTT_PORT));
char DOMAIN_KEY[DOMAIN_KEY_SIZE] = STR(BOODSKAP_DOMAIN_KEY);
char API_KEY[API_KEY_SIZE] = STR(BOODSKAP_API_KEY);
char DEVICE_ID[DEVICE_ID_SIZE];

WiFiClient wifiClient;
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
class MqttCallack;

void process();
void connectToWifi();
void onWifiConnect(const WiFiEventStationModeGotIP &event);
void onWifiDisconnect(const WiFiEventStationModeDisconnected &event);
void processMessage(uint64_t mid, JsonObject &header, JsonObject &data);
void processData(size_t len, byte *data);

Ticker processor(process, 15000, 0, MILLIS);

class MqttCallback : public MqttAdapter
{

  virtual int onMessage(uint64_t mid, JsonObject &header, JsonObject &data)
  {
    processMessage(mid, header, data);
    return 1; //ack , return any other number for nack
  }

  virtual void onData(size_t len, byte *data)
  {
    processData(len, data);
  }

  virtual void onMqttDisconnect(AsyncMqttClientDisconnectReason reason){
    Boodskap.println("MQTT disconnected");
  }

};

MqttCallback listener;

void setup()
{

  Serial.begin(BAUD_RATE);

  delay(100);

  Serial.println();
  Serial.println();

  sprintf(DEVICE_ID, "ESP%08X", ESP.getChipId());
  strcpy(BOODSKAP_HOST, STR(BOODSKAP_MQTT_HOST));

  Boodskap.setPrinter(Serial); //Disable this to stop printing in Serial console

  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  Boodskap.setServer(BOODSKAP_HOST, BOODSKAP_PORT);
  Boodskap.setDomainKey(DOMAIN_KEY);
  Boodskap.setApiKey(API_KEY);
  Boodskap.setDeviceId(DEVICE_ID);
  Boodskap.setListener(&listener);


  Boodskap.print("Domain ");
  Boodskap.println(Boodskap.domainKey());
  Boodskap.print("API ");
  Boodskap.println(Boodskap.apiKey());
  Boodskap.print("Device ID ");
  Boodskap.println(Boodskap.deviceId());
  Boodskap.println("");

  connectToWifi();

  processor.start();
}

void loop()
{

  Boodskap.loop();

  processor.update();
}

uint16_t counter = 0;
/**
 * Entry point of your solution
 */
void process()
{

  if (!Boodskap.connected())
    return;

  StaticJsonDocument<MAX_PACKET_SIZE> msg;
  msg["counter"] = ++counter;
  msg["uptime"] = millis();

  //Boodskap.send(102, msg);
}

void processMessage(uint64_t mid, JsonObject &header, JsonObject &data)
{
    Serial.println("Processing message...");
    String msgStr;
    serializeJson(data, Serial);
    Serial.println();
}

void processData(size_t len, byte *data)
{
    Serial.println("Processing data...");
    Serial.println((const char*)data);
}


void connectToWifi()
{

  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void onWifiConnect(const WiFiEventStationModeGotIP &event)
{
  Serial.println("Connected to Wi-Fi.");
  Boodskap.connect();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected &event)
{
  Serial.println("Disconnected from Wi-Fi.");
  connectToWifi();
}


