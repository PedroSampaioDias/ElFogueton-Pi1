#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>

const char *ssid = "ElFogueton";
const char *password = "elfogueton";

// Configure o endereço IP estático, gateway e máscara de sub-rede
IPAddress staticIP(192, 168, 0, 100);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

AsyncWebServer server(80);

bool dataLoggerAtivo = false;

void setup() {
  Serial.begin(115200);

  if (!SD.begin(5)) {
    Serial.println("Failed to mount SD card");
    return;
  }

  // Configure o ponto de acesso com endereço IP estático
  WiFi.softAPConfig(staticIP, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.print("Endereço IP do Ponto de Acesso: ");
  Serial.println(WiFi.softAPIP());

  server.on("/iniciar_gravacao", HTTP_GET, [](AsyncWebServerRequest *request){
    dataLoggerAtivo = true;
    request->send(200, "text/plain", "Iniciando o Data Logger\n");
  });

  server.on("/parar_gravacao", HTTP_GET, [](AsyncWebServerRequest *request){
    dataLoggerAtivo = false;
    request->send(200, "text/plain", "Parando o Data Logger\n");
  });

  server.on("/status_gravacao", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", dataLoggerAtivo ? "Ativo\n" : "Inativo\n");
  });

  server.on("/obter_dados", HTTP_GET, [](AsyncWebServerRequest *request){
    File arquivo = SD.open("/Dados.txt", "r");
    if (arquivo && !dataLoggerAtivo) request->send(200, "text/plain", arquivo.readString());
    else request->send(404, "text/plain", "Erro: Arquivo não existe ou esta sendo escrito\n");
    arquivo.close();
  });

  server.begin();
}

void loop() {
  //if(dataLoggerAtivo) Serial.println("Data Logger ativo");
  //else Serial.println("Data Logger Inativo");
}