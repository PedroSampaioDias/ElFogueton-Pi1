#ifndef __GPS_H__
#define __GPS_H__

#include <TinyGPS++.h>

#define GPS_SERIAL Serial2

#ifndef DEBUG
#define DEBUG 0
#endif

/**
 * Salvar os dados de interesse em uma matriz de caracteres.
 * @param dados char** - A matriz de caracteres para armazenar os dados instantâneos.
 * @param gps TinyGPSPlus& - Objeto para conexão com o GPS.
 */
void setDadosInstantaneos(char dados[6][20], TinyGPSPlus &gps);

#endif
