#include "setup.h"
#include "gps.h"
#include "persistencia.h"

void setupSerial(int baudRate) {
  Serial.begin(baudRate);
  while (!Serial) {
    Serial.println("Erro ao abrir porta serial! Tentando novamente...");
  }
}

void setupGPSSerial(int baudRate) {
  GPS_SERIAL.begin(baudRate);
  while (!GPS_SERIAL) {
    Serial.println("Erro ao acessar GPS! Tentando novamente...");
  }
}

void setupSDCard(int pinoSD) {
  while (!SD.begin(pinoSD)) {
    Serial.println("Erro ao abrir SD Card! Tentando novamente...");
  }
}

void setupWiFi(const char* ssid, const char* password, const IPAddress& staticIP, const IPAddress& gateway, const IPAddress& subnet) {
  WiFi.softAPConfig(staticIP, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.print("Endereço IP do Ponto de Acesso: ");
  Serial.println(WiFi.softAPIP());
}

void setupEndpoints(AsyncWebServer& server, bool& dataLoggerAtivo, int& posUltimoByteLido) {
  server.on("/iniciar_gravacao", HTTP_GET, [&dataLoggerAtivo, &posUltimoByteLido](AsyncWebServerRequest* request) {
    handleIniciarGravacao(request, dataLoggerAtivo, posUltimoByteLido);
  });

  server.on("/parar_gravacao", HTTP_GET, [&dataLoggerAtivo, &posUltimoByteLido](AsyncWebServerRequest* request) {
    handlePararGravacao(request, dataLoggerAtivo, posUltimoByteLido);
  });

  server.on("/status_gravacao", HTTP_GET, [&dataLoggerAtivo](AsyncWebServerRequest* request) {
    handleStatusGravacao(request, dataLoggerAtivo);
  });

  server.on("/obter_dados", HTTP_GET, [&dataLoggerAtivo, &posUltimoByteLido](AsyncWebServerRequest* request) {
    handleObterDados(request, dataLoggerAtivo, posUltimoByteLido);
  });

  server.begin();
}

void handleIniciarGravacao(AsyncWebServerRequest* request, bool& dataLoggerAtivo, int& posUltimoByteLido) {
  dataLoggerAtivo = true;
  posUltimoByteLido = 0;
  request->send(200, "text/plain", "Iniciando o Data Logger\n");
}

void handlePararGravacao(AsyncWebServerRequest* request, bool& dataLoggerAtivo, int& posUltimoByteLido) {
  dataLoggerAtivo = false;
  posUltimoByteLido = 0;
  request->send(200, "text/plain", "Parando o Data Logger\n");
}

void handleStatusGravacao(AsyncWebServerRequest* request, bool& dataLoggerAtivo) {
  request->send(200, "text/plain", dataLoggerAtivo ? "Ativo\n" : "Inativo\n");
}

void handleObterDados(AsyncWebServerRequest* request, bool& dataLoggerAtivo, int& posUltimoByteLido) {
  if (dataLoggerAtivo) {
    request->send(404, "text/plain", "Erro: arquivo está sendo escrito!\n");
    return;
  }

  File arquivo = SD.open("/Dados.txt", "r");
  if (!arquivo) {
    request->send(404, "text/plain", "Erro: arquivo não está disponível!\n");
    return;
  }

  arquivo.seek(posUltimoByteLido);
  String linhasConcatenadas = "";
  for (int i = 0; i < 50 && arquivo.available(); i++) {
    String linha = arquivo.readStringUntil('\n');
    linhasConcatenadas += linha + "\n";
  }

  if (linhasConcatenadas.length() > 0) {
    request->send(200, "text/plain", linhasConcatenadas);
    posUltimoByteLido = arquivo.position();
  } else {
    request->send(200, "text/plain", "EOF\n");
    posUltimoByteLido = 0;
  }

  arquivo.close();
}
