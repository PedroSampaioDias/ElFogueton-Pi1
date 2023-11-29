#ifndef SETUP_H
#define SETUP_H

#include <WiFi.h>
#include <SD.h>
#include <ESPAsyncWebServer.h>

/**
 * Configura a comunicação serial.
 */
void setupSerial(int baudRate);

/**
 * Configura a porta serial para comunicação com o GPS.
 */
void setupGPSSerial(int baudRate);

/**
 * Inicializa o cartão SD.
 */
void setupSDCard(int pinoSD);

/**
 * Configura a conexão Wi-Fi.
 */
void setupWiFi(const char* ssid, const char* password, const IPAddress& staticIP, const IPAddress& gateway, const IPAddress& subnet);

/**
 * Configura os endpoints do servidor web assíncrono.
 */
void setupEndpoints(AsyncWebServer& server, bool& dataLoggerAtivo, int& posUltimoByteLido);

/**
 * Manipulador para o endpoint "/iniciar_gravacao".
 * @param request Objeto de solicitação do servidor web.
 */
void handleIniciarGravacao(AsyncWebServerRequest* request, bool& dataLoggerAtivo, int& posUltimoByteLido);

/**
 * Manipulador para o endpoint "/parar_gravacao".
 * @param request Objeto de solicitação do servidor web.
 */
void handlePararGravacao(AsyncWebServerRequest* request, bool& dataLoggerAtivo, int& posUltimoByteLido);

/**
 * Manipulador para o endpoint "/status_gravacao".
 * @param request Objeto de solicitação do servidor web.
 */
void handleStatusGravacao(AsyncWebServerRequest* request, bool& dataLoggerAtivo);

/**
 * Manipulador para o endpoint "/obter_dados".
 * @param request Objeto de solicitação do servidor web.
 */
void handleObterDados(AsyncWebServerRequest* request, bool& dataLoggerAtivo, int& posUltimoByteLido);

#endif
