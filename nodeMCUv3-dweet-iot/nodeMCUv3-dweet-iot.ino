// Biblioteca especial do ESP
#include <ESP8266WiFi.h>

#include <ArduinoJson.h>

// Parametros WiFi
const char* ssid = "CCSL";
const char* password = "flossrulez";

// Pino do LED
#define  LED_PIN D5

// Intervalo entre cada mudanca de estado do LED
#define INTERVAL 10000

// Host
const char* host = "dweet.io";

String nome_da_coisa = "minha_coisa";
boolean led_state = true;
long last_change;
 

void setup() {

  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
    
  Serial.println("Attempting to connect to WPA network...");
  wifiConnection();
  last_change = millis();
}

// Funcao para (re)conectar a rede WiFi
void wifiConnection() {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("Endereco IP: ");
    Serial.println(WiFi.localIP());
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
      wifiConnection();
  } else {
    
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      delay(1000);
      return;
    }

    // Envia uma requisicao para o servidor do dweet.io
    client.print(String("GET https://dweet.io/get/latest/dweet/for/") + String(nome_da_coisa) + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    // Aguardamos um tempo para que os pacotes de resposta sejam recebidos
    delay(2000);
    
    String command_json = "";
    boolean reading_json = false;
    // Le a resposta do servidor e captura os dados no formato json
    while(client.available()) {
      char c = client.read();
      if (reading_json && c != '\n' && c != '\r') {
        command_json += c;
        continue;
      }
      if (c == '{') {
        reading_json = true;
        command_json += c;
      }
    }
    client.stop();
    
    command_json.replace("[", "");
    command_json.replace("]", "");
    // Parser de formato json
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(command_json);
    if (!root.success()) {
      Serial.println("parseObject() failed");
      int errorCode       = root["Error"]["Code"];
      const char* message = root["Error"]["Message"];
      Serial.println(message);
    }

    // A propriedade da coisa pode ser recuperada acessando o objeto
    // json no dicionario with->content->(propriedade), veja o exemplo abaixo.
    const char* led_state = root["with"]["content"]["led"];
    Serial.print("led_state: ");
    Serial.println(led_state);

    // Usa a propriedade do led para ligar ou desligar um led
    digitalWrite(LED_PIN, String(led_state).toInt());
    
    // Deixamos ainda mais meio segundo entre cada final de comunicacao e nova requisicao
    delay(500);
  }

}
