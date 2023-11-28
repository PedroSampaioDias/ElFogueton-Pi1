#ifndef __GPS_H__
#define __GPS_H__
#include <TinyGPS++.h>

#define GPS_SERIAL Serial2

#ifndef DEBUG
#define DEBUG 0
#endif

typedef struct CoordenadasEspaciais {
    float latitude;
    float longitude;
    float altitude;
} CoordenadasEspaciais;

typedef struct DadosInstantaneos {
    CoordenadasEspaciais coordenadas;
    float velocidade;
    char data[11];
    char hora[9];
} DadosInstantaneos;

/**
 * Salvar os dados de interesse em uma struct.
 * @params DadosInstantaneos &dadosInstantaneos - A struct em questão.
 * @params TinyGPSPlus &gps - Objeto para conexão com o GPS.
 */
void setDadosInstantaneos(DadosInstantaneos &dadosInstantaneos, TinyGPSPlus &gps);

/**
 * Formatar a data e salvar numa struct.
 * @params DadosInstantaneos &dadosInstantaneos - A struct em questão.
 * @params TinyGPSPlus &gps - Objeto para conexão com o GPS.
 */
void setData(DadosInstantaneos &dadosInstantaneos, TinyGPSPlus &gps);

/**
 * Formatar a hora e salvar numa struct.
 * @params DadosInstantaneos &dadosInstantaneos - A struct em questão.
 * @params TinyGPSPlus &gps - Objeto para conexão com o GPS.
 */
void setHora(DadosInstantaneos &dadosInstantaneos, TinyGPSPlus &gps);

/**
 * Printar os dados instantâneos do gps.
 * @params DadosInstantaneos &dadosInstantaneos - A struct em questão.
 * @params TinyGPSPlus &gps - Objeto para conexão com o GPS.
 */
void debugDadosInstantaneos(DadosInstantaneos &dadosInstantaneos, TinyGPSPlus &gps);
#endif
