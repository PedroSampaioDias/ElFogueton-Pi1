#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <SD.h>

#define GPS_SERIAL Serial2
#define QTD_COLUNAS 6

IPAddress enderecoIPestatico(192, 168, 0, 100);
IPAddress gateway(192, 168, 0, 1);
IPAddress mascaraSubrede(255, 255, 255, 0);

AsyncWebServer servidorWeb(80);

TinyGPSPlus gps;

bool dataLoggerAtivo = false;
int posUltimoByteLido = 0;
char dadosInstantaneos[QTD_COLUNAS][20];

int configurarDadosInstantaneos(char dados[QTD_COLUNAS][20], TinyGPSPlus &gps);
void salvarDadosCSV(char dados[QTD_COLUNAS][20], String nomeArquivo);

void setup() {
  Serial.begin(9600);
  while(!Serial);

  GPS_SERIAL.begin(9600);
  while(!GPS_SERIAL);

  while (!SD.begin(5)) {
    Serial.println("Falha ao montar o cartão SD");
  }

  WiFi.softAPConfig(enderecoIPestatico, gateway, mascaraSubrede);
  WiFi.softAP("ElFogueton");
  Serial.print("Endereço IP do Ponto de Acesso: ");
  Serial.println(WiFi.softAPIP());

  servidorWeb.on("/iniciar_gravacao", HTTP_GET, [](AsyncWebServerRequest *request){
    dataLoggerAtivo = true;
    posUltimoByteLido = 0; // Resetar a posição de leitura ao iniciar a gravação
    request->send(200, "text/plain", "Iniciando o Data Logger\n");
  });

  servidorWeb.on("/parar_gravacao", HTTP_GET, [](AsyncWebServerRequest *request){
    dataLoggerAtivo = false;
    posUltimoByteLido = 0; // Resetar a posição de leitura ao parar a gravação
    request->send(200, "text/plain", "Parando o Data Logger\n");
  });

  servidorWeb.on("/status_gravacao", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", dataLoggerAtivo ? "Ativo\n" : "Inativo\n");
  });

  servidorWeb.on("/obter_dados", HTTP_GET, [](AsyncWebServerRequest *request){
    if(dataLoggerAtivo) {
      request->send(404, "text/plain", "Erro: arquivo está sendo escrito!\n");
      return;
    }

    File arquivo = SD.open("/Dados.txt", "r");
    if(!arquivo) {
      request->send(404, "text/plain", "Erro: arquivo não está disponível!\n");
      return;
    }

    arquivo.seek(posUltimoByteLido);

    String linhasConcatenadas = "";
    for (int i = 0; i < 50 && arquivo.available(); i++) {
      String linhas = arquivo.readStringUntil('\n');
      linhasConcatenadas += linhas + "\n";
    }

    if (linhasConcatenadas.length() > 0) {
      request->send(200, "text/plain", linhasConcatenadas);
      posUltimoByteLido = arquivo.position();
    } else {
      request->send(200, "text/plain", "EOF\n");
      posUltimoByteLido = 0;
    }

    arquivo.close();
  });

  servidorWeb.on("/apagar_dados", HTTP_GET, [](AsyncWebServerRequest *request){
    if(dataLoggerAtivo) {
      request->send(404, "text/plain", "Erro: arquivo está sendo escrito!\n");
      return;
    }

    if(SD.remove("/Dados.txt")) {
      request->send(200, "text/plain", "Arquivo '/Dados.txt' removido com sucesso!\n");
    } else {
      request->send(404, "text/plain", "Erro ao remover o arquivo '/Dados.txt'\n");
    }
  });

  servidorWeb.on("/tamanho_arquivo", HTTP_GET, [](AsyncWebServerRequest *request){
    if(dataLoggerAtivo) {
      request->send(404, "text/plain", "Erro: arquivo está sendo escrito!\n");
      return;
    }

    File arquivo = SD.open("/Dados.txt", "r");
    if(!arquivo) {
      request->send(404, "text/plain", "Erro: arquivo não está disponível!\n");
      return;
    }

    request->send(200, "text/plain", "Tamanho do arquivo: " + String(arquivo.size()) + " bytes\n");

    arquivo.close();
  });

  servidorWeb.on("/reiniciar_esp32", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Reiniciando ESP32...\n");
    ESP.restart();
  });

  servidorWeb.on("/quantidade_satelites", HTTP_GET, [](AsyncWebServerRequest *request){
    if (GPS_SERIAL.available() > 0) {
      if (gps.encode(GPS_SERIAL.read())) {
        request->send(200, "text/plain", "Quantidade de Satélites: " + String(gps.satellites.value()) + "\n");
    } 
   } else request->send(400, "text/plain", "Quantidade de Satélites não disponível.\n");
  });

  servidorWeb.begin();
}

void loop() {
  if (dataLoggerAtivo) {
    int dadosConfigurados = configurarDadosInstantaneos(dadosInstantaneos);
    if(dadosConfigurados) {
      salvarDadosCSV(dadosInstantaneos, "/Dados.txt");
    }
  }
}

int configurarDadosInstantaneos(char dados[QTD_COLUNAS][20]) {
  if (GPS_SERIAL.available() > 0) {
    if (gps.encode(GPS_SERIAL.read())) {
      if (gps.location.isValid() && gps.altitude.isValid() && gps.speed.isValid() && gps.date.isValid() && gps.time.isValid()) {
        snprintf(dados[0], 20, "%f\0", gps.location.lat());
        snprintf(dados[1], 20, "%f\0", gps.location.lng());
        snprintf(dados[2], 20, "%f\0", gps.altitude.meters());
        snprintf(dados[3], 20, "%f\0", gps.speed.mps());
        snprintf(dados[4], 20, "%02d/%02d/%04d\0", gps.date.day(), gps.date.month(), gps.date.year());
        snprintf(dados[5], 20, "%02d:%02d:%02d\0", gps.time.hour(), gps.time.minute(), gps.time.second());

        return 1;
      }
    }
  }

  return 0;
}

void salvarDadosCSV(char dados[QTD_COLUNAS][20], String nomeArquivo) {
    File arquivo = SD.open(nomeArquivo.c_str(), FILE_APPEND);
    if (arquivo) {
        for (int i = 0; i < QTD_COLUNAS; ++i) {
            arquivo.print(dados[i]);
            if (i < QTD_COLUNAS - 1) {
                arquivo.print(",");
            }
        }
        arquivo.println();
    }
    arquivo.close();
}
