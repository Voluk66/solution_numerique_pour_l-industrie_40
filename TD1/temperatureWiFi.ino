#include <DHT.h>
#include <WiFiClient.h>

const char* gatewayIP = "192.168.1.1";

#define DHTPIN 5     // Pin data du capteur DHT22
#define DHTTYPE DHT22   // Type de capteur DHT22
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);

  dht.begin();

  // Connexion à la passerelle
  WiFi.config(IPAddress(192, 168, 1, 87), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  WiFi.begin();
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion en cours...");
  }
  Serial.println("Connecté à la passerelle");

  // Imprimer l'adresse IP
  Serial.println(WiFi.localIP());
}

void loop() {
  if (client.connect(gatewayIP, 80)) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("Erreur lors de la lecture des données du capteur DHT22!");
      return;
    }

    String data = "Température: " + String(t) + "°C " + "Humidité: " + String(h) + "% ";

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println(data);
    client.println("</html>");

    delay(1);
    client.stop();
  }
}
