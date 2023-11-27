#ifndef __GPS_H__
#define __GPS_H__

#define GPS_SERIAL Serial2
#define BAUD_RATE 9600

#define DEBUG 1

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
 */
void setDadosInstantaneos();

/**
 * Formatar a data e salvar numa struct.
 */
void setData();

/**
 * Formatar a hora e salvar numa struct.
 */
void setHora();

/**
 * Printar os dados instantâneos do gps.
 */
void debugDadosInstantaneos();
#endif
