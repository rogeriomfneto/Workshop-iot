// Biblioteca especial do ESP
#include <ESP8266WiFi.h>

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

    if (millis() - last_change > INTERVAL) {
      last_change = millis();
      led_state = !led_state;
      digitalWrite(LED_PIN, led_state);
    }
    
    // Envia uma requisicao para o servidor do dweet.io
    client.print(String("GET /dweet/for/") + nome_da_coisa + "?led=" + String(led_state) + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    // Aguardamos um tempo para que os pacotes de resposta sejam recebidos
    delay(2000);
    
    // Le todas as linhas da resposta do servidor e as imprime pela serial
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    Serial.println();
    
    // Deixamos ainda mais meio segundo entre cada final de comunicacao e nova requisicao
    delay(500);
  }

}
