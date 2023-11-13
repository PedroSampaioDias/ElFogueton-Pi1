#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h>

const char* ssid = "ElFogueton";
const char* password = "elfogueton";

AsyncWebServer server(80);

void setup() {
  Serial.begin(9660);
  randomSeed(analogRead(0));

  // Configurar o ESP32 como ponto de acesso
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  // Rota para a página "Hello World"
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", String(random(0, 100)));
  });

  // Iniciar o servidor
  server.begin();

  Serial.println("Endereço IP do Ponto de Acesso: " + WiFi.softAPIP().toString());
}

void loop() {
  // Lidar com eventos assíncronos, se necessário
}