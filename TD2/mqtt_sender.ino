/*
  Sources : 
  ArduinoMqttClient - WiFi Simple Sender (public domain)
  TD1 code
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

#define DHTPIN 5     // sensor digital pin
#define DHTTYPE DHT22   // sensor type
DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "Groupe8";
char pass[] = "arduino1";
/*char ssid[] = "TheLabIOT";    // IOT gateway
char pass[] = "Yaay!ICanTalkNow";*/

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

//public broker & topic
const char broker[] = "broker.hivemq.com";
int        port     = 1883;
char topic[]  = "spBv1.0/ESIEA/DDATA/raspberry/esp8266_8";

const long interval = 1000;
unsigned long previousMillis = 0;

//int log_broker = 0;

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

  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    if (isnan(h) || isnan(t)) {
      Serial.println("Error while reading sensor's data");
      return;
    }

    //
    //  Send RAW data (red-node in charge of parsing data and setting up the JSON)
    //
    String send = String(t)+"/"+String(h);


    Serial.print("Sending message to topic: ");
    Serial.println(topic);

    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topic);
    mqttClient.print(send);
    mqttClient.endMessage();

    Serial.println();
  }
}