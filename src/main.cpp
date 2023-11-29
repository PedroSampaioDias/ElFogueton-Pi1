#include <WiFi.h>
#include <TinyGPS++.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>
#include "gps.h"
#include "persistencia.h"
#include "setup.h"

const char* WIFI_SSID = "ElFogueton";
const char* WIFI_PASSWORD = "elfogueton";
const int PORTA = 80;
const int PINO_MSD = 5;
const int BAUD_RATE = 9600;

IPAddress staticIP(192, 168, 0, 100);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

AsyncWebServer server(PORTA);
bool dataLoggerAtivo = false;
int posUltimoByteLido = 0;

TinyGPSPlus gps;
char dadosInstantaneos[6][20];

void setup() {
  setupSerial(BAUD_RATE);
  setupGPSSerial(BAUD_RATE);
  setupSDCard(PINO_MSD);
  setupWiFi(WIFI_SSID, WIFI_PASSWORD, staticIP, gateway, subnet);
  setupEndpoints(server, dataLoggerAtivo, posUltimoByteLido);
}

void loop() {
  if (dataLoggerAtivo) {
    setDadosInstantaneos(dadosInstantaneos, gps);
    salvarDadosCSV(dadosInstantaneos, "/Dados.txt");
  }
}
