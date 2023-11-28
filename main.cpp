#include <WiFi.h>
#include <TinyGPS++.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>
#include "gps.h"
#include "persistencia.h"

#define PORTA 80 // porta utilizada para conexão via protocolo HTTP
#define PINO_MSD 5
#define BAUD_RATE 9600
#define GPS_SERIAL Serial2

// definição de constantes para abrir wifi
const char *ssid = "ElFogueton";
const char *password = "elfogueton";

// configuração de IP
IPAddress staticIP(192, 168, 0, 100);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// abertura do servidor 
AsyncWebServer server(PORTA);

// mutex que só permite que um arquivo seja enviado se não estiver sendo escrito
bool dataLoggerAtivo = false;

// ponteiro para a última posição lida no arquivo de texto
int posUltimoByteLido = 0;

// conexão com o GPS
TinyGPSPlus gps;

// struct para persistência de dados vindos do gps em um determinado instante
DadosInstantaneos dadosInstantaneos = { {0.0, 0.0, 0.0}, 0.0, "00/00/0000", "00:00:00" };

void setup() {
  Serial.begin(BAUD_RATE);

  // polling para abrir porta serial
  while(!Serial) {
    Serial.println("Erro ao abrir porta serial!");
    Serial.println("Tentando novamente...");
  }

  GPS_SERIAL.begin(BAUD_RATE);

  // polling para abrir conexão com GPS
  while(!GPS_SERIAL){
    Serial.println("Erro ao acessar GPS!");
    Serial.println("Tentando novamente...");
  }

  // polling para abrir conexão com sd card 
  while (!SD.begin(PINO_MSD)) {
    Serial.println("Erro ao abrir SD Card!");
    Serial.println("Tentando novamente...");
  }

  // definir conexão wifi
  WiFi.softAPConfig(staticIP, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.print("Endereço IP do Ponto de Acesso: ");
  Serial.println(WiFi.softAPIP());

  // endpoints
  server.on("/iniciar_gravacao", HTTP_GET, [](AsyncWebServerRequest *request){
    dataLoggerAtivo = true;
    posUltimoByteLido = 0; // Reset the read position when starting recording
    request->send(200, "text/plain", "Iniciando o Data Logger\n");
  });

  server.on("/parar_gravacao", HTTP_GET, [](AsyncWebServerRequest *request){
    dataLoggerAtivo = false;
    posUltimoByteLido = 0; // Reset the read position when stopping recording
    request->send(200, "text/plain", "Parando o Data Logger\n");
  });

  server.on("/status_gravacao", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", dataLoggerAtivo ? "Ativo\n" : "Inativo\n");
  });

  server.on("/obter_dados", HTTP_GET, [](AsyncWebServerRequest *request){
    if(dataLoggerAtivo) {
      request->send(404, "text/plain", "Erro: arquivo está sendo escrito!\n");
      return;
    }
    File arquivo = SD.open("/Dados.txt", "r");
    if(!arquivo) {
      request->send(404, "text/plain", "Erro: arquivo não está disponível!\n");
      return;
    }

    // ler o arquivo a partir do ultimo byte lido 
    arquivo.seek(posUltimoByteLido);

    // ler 50 linhas e concatenar numa string
    String concatenatedLines = "";
    for (int i = 0; i < 50 && arquivo.available(); i++) {
      String linhas = arquivo.readStringUntil('\n');
      linhasConcatenadas += line + "\n";
    }

    if (linhasConcatenadas.length() > 0) {

      // retornar as 50 linhas para o usuário
      request->send(200, "text/plain", linhasConcatenadas);

      // atualizar a posição do último byte lido
      posUltimoByteLido = arquivo.position();
    } else {

      // se chegar no EOF, retornar isso para o cliente.
      request->send(200, "text/plain", "EOF\n");
      posUltimoByteLido = 0;
    }

    arquivo.close();
  });

  server.begin();
}

void loop() {
    if(dataLoggerAtivo) {
        setDadosInstantaneos(dadosInstantaneos, gps);
        salvarDadosCSV(dadosInstantaneos, "/Dados.txt");
    }
}
