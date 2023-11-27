#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>

const char *ssid = "ElFogueton";
const char *password = "elfogueton";

IPAddress staticIP(192, 168, 0, 100);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

AsyncWebServer server(80);

bool dataLoggerAtivo = false;
int lastReadPosition = 0;

void setup() {
  Serial.begin(115200);

  if (!SD.begin(5)) {
    Serial.println("Failed to mount SD card");
    return;
  }

  WiFi.softAPConfig(staticIP, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.print("Endereço IP do Ponto de Acesso: ");
  Serial.println(WiFi.softAPIP());

  server.on("/iniciar_gravacao", HTTP_GET, [](AsyncWebServerRequest *request){
    dataLoggerAtivo = true;
    lastReadPosition = 0; // Reset the read position when starting recording
    request->send(200, "text/plain", "Iniciando o Data Logger\n");
  });

  server.on("/parar_gravacao", HTTP_GET, [](AsyncWebServerRequest *request){
    dataLoggerAtivo = false;
    lastReadPosition = 0; // Reset the read position when stopping recording
    request->send(200, "text/plain", "Parando o Data Logger\n");
  });

  server.on("/status_gravacao", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", dataLoggerAtivo ? "Ativo\n" : "Inativo\n");
  });

  server.on("/obter_dados", HTTP_GET, [](AsyncWebServerRequest *request){
    File arquivo = SD.open("/Dados.txt", "r");
    if (arquivo && !dataLoggerAtivo) {
      // Set the file position to the last read position
      arquivo.seek(lastReadPosition);

      // Read and concatenate the next 50 lines
      String concatenatedLines = "";
      for (int i = 0; i < 50 && arquivo.available(); i++) {
        String line = arquivo.readStringUntil('\n');
        concatenatedLines += line + "\n";
      }

      if (concatenatedLines.length() > 0) {
        // If lines are concatenated, send them
        request->send(200, "text/plain", concatenatedLines);
        // Update the last read position
        lastReadPosition = arquivo.position();
      } else {
        // If no lines are left, we've reached the end of the file
        request->send(200, "text/plain", "EOF\n");
        // Reset the last read position for the next iteration
        lastReadPosition = 0;
      }

      arquivo.close();
    } else {
      request->send(404, "text/plain", "Erro: Arquivo não existe ou está sendo escrito\n");
    }
  });

  server.begin();
}

void loop() {
  // Your loop code here
}
