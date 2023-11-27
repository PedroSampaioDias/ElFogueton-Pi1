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
 * @params DadosInstantaneos &dadosInstaneos - A struct em questão.
 */
void setDadosInstantaneos(DadosInstantaneos &dadosInstantaneos);

/**
 * Formatar a data e salvar numa struct.
 * @params DadosInstantaneos &dadosInstaneos - A struct em questão.
 */
void setData(DadosInstantaneos &dadosInstantaneos);

/**
 * Formatar a hora e salvar numa struct.
 * @params DadosInstantaneos &dadosInstaneos - A struct em questão.
 */
void setHora(DadosInstantaneos &dadosInstantaneos);

/**
 * Printar os dados instantâneos do gps.
 * @params DadosInstantaneos &dadosInstaneos - A struct em questão.
 */
void debugDadosInstantaneos(DadosInstantaneos &dadosInstantaneos);
#endif
