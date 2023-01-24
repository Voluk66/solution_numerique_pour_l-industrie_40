Vo£uK
#1028

GiBdN — 21/10/2022 09:32
https://discord.gg/R9JxKWUV
Vo£uK — Hier à 13:26
https://iconic-treat-375111.nw.r.appspot.com/
OuternShip
Vo£uK — Hier à 15:00
XD
oulala on l'avais pas predit
GiBdN — Hier à 15:00
😦
GiBdN — Aujourd’hui à 13:54

Image
GiBdN — Aujourd’hui à 14:03
/*
  ArduinoMqttClient - WiFi Simple Sender

  This example code is in the public domain.
*/

#include <ESP8266WiFi.h>
#include <DHT.h>
#include <WiFiClient.h>
#include <String.h>

#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#elif defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_NICLA_VISION) || defined(ARDUINO_ARCH_ESP32)
  #include <WiFi.h>
#endif

#define DHTPIN 5     // Pin data du capteur DHT22
#define DHTTYPE DHT22   // Type de capteur DHT22
DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "TheLabIOT";    // your network SSID (name)
char pass[] = "Yaay!ICanTalkNow";    // your network password (use for WPA, or use as key for WEP)


WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "192.168.1.4";
int        port     = 1883;
char topic[]  = "ESIEA/grp8_test";

const long interval = 1000;
unsigned long previousMillis = 0;

int count = 0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  dht.begin();
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alives which
  // avoids being disconnected by the broker
  mqttClient.poll();

  double h = dht.readHumidity();
  double t = dht.readTemperature();

  // to avoid having delays in loop, we'll use the strategy from BlinkWithoutDelay
  // see: File -> Examples -> 02.Digital -> BlinkWithoutDelay for more info
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    if (isnan(h) || isnan(t)) {
      Serial.println("Erreur lors de la lecture des donnÃ©es du capteur DHT22!");
      return;
    }

    String send = "{\n\t\"topic\" : \"ESIEA/grp8_test\",\n\t\"data\" : {\n\t\t\"temperature\" : " + String(t) + ",\n\t\t\"humidity\" : " + String(h) + "\n\t}\n}";

    Serial.print("Sending message to topic: ");
... (10 lignes restantes)
Réduire
mqtt_sender.ino
4 Ko
﻿
GiBdN#7343
