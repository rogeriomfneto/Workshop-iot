#include <ESP8266WiFi.h>

const char* ssid     = "CCSL";
const char* password = "flossrulez";
const char* host = "35.198.24.33";

void setup() {
  Serial.begin(115200);

  delay(1000);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.print("connecting to ");
  Serial.println(host);
  
  WiFiClient client;
  const int port = 8000;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }

  while (client.connected()) {
    while (client.available() != 0) {
      Serial.print(client.read());
    }
  
    while (Serial.available()) {
      client.print(Serial.read());
    }
  }

  client.stop();
  Serial.println();
  Serial.println("closing connection");
}
