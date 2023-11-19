#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <SD.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

//progmeg

#define GPS_SERIAL Serial2
#define BAUD_RATE 9600
#define CS_PIN 5

#define DEBUG 1

TinyGPSPlus gps;

char dadosInstantaneos[6][20]; // Usando arrays de caracteres em vez de String

unsigned int numeroArquivoAtual = 0;

bool dataLoggerAtivo = false;

AsyncWebServer server(80);

int countFiles();

void setup() {
  Serial.begin(BAUD_RATE);

  while (!Serial) { ; }

  GPS_SERIAL.begin(BAUD_RATE);

  while (!GPS_SERIAL) { ; }

  while (!SD.begin(CS_PIN)) { ; }

  server.on("/dataLogger", HTTP_GET, [](AsyncWebServerRequest *request){
    String ligarDesligar = request->getParam("comando")->value();
    if (ligarDesligar == "ligar") {
      iniciarDataLogger();
      request->send(200, "text/plain", "Iniciando o Data Logger");
    } else if (ligarDesligar == "desligar") {
      pararDataLogger();
      request->send(200, "text/plain", "Parando o Data Logger");
    } else {
      String status = (dataLoggerAtivo) ? "Ativo" : "Inativo";
      request->send(200, "text/plain", "Status do Data Logger: " + status);
    }
  });

  server.on("/dados", HTTP_GET, [](AsyncWebServerRequest *request){
    String consulta = request->getParam("consulta")->value();
    // Exemplo: "Consulta: todos"
    // Exemplo: "Consulta: último"
    // Exemplo: "Consulta: data=2023-01-01"
    //request->send(200, "text/plain", resposta);
  });

  server.begin();
}

void loop() {
  while (GPS_SERIAL.available() > 0) {
    gps.encode(GPS_SERIAL.read());
  }

  if (gps.location.isValid() && gps.altitude.isValid() && gps.speed.isValid() && gps.date.isValid() && gps.time.isValid()) {
    sprintf(dadosInstantaneos[0], "%f\0", gps.location.lat());
    sprintf(dadosInstantaneos[1], "%f\0", gps.location.lng());
    sprintf(dadosInstantaneos[2], "%f\0", gps.altitude.meters());
    sprintf(dadosInstantaneos[3], "%f\0", gps.speed.mps());
    sprintf(dadosInstantaneos[4], "%02d/%02d/%04d\0", gps.date.day(), gps.date.month(), gps.date.year());
    sprintf(dadosInstantaneos[5], "%02d:%02d:%02d\0", gps.time.hour(), gps.time.minute(), gps.time.second());

    String nomeArquivoAtual = "/Dados.txt";

    File arquivo = SD.open(nomeArquivoAtual.c_str(), FILE_APPEND);

    if (arquivo) {
      for (int i = 0; i < 6; ++i) {+
        arquivo.print(dadosInstantaneos[i]);
        if (i < 6 - 1) {
          arquivo.print(",");
        }
      }
      arquivo.println();
      arquivo.close();
    }
  }
}

int countFiles() {
  int count = 0;
  File root = SD.open("/");

  if (root) {
    while (true) {
      File entry = root.openNextFile();
      if (!entry) {
        // Não há mais arquivos
        break;
      }
      if (entry.isDirectory()) {
        // Pule diretórios
        entry.close();
        continue;
      }
      count++;
      entry.close();
    }
    root.close();
  }

  return count;
}

void iniciarDataLogger() {
  // Adicione o código para inicializar o data logger aqui
  dataLoggerAtivo = true;
}

void pararDataLogger() {
  // Adicione o código para parar o data logger aqui
  dataLoggerAtivo = false;
}
