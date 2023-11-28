#include <HardwareSerial.h>
#include <SD.h>
#include "gps.h"
#include "persistencia.h"

#ifndef DEBUG
#define DEBUG 0
#endif

#define QTD_COLUNAS 6

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

char dados[6][20]; // Usando arrays de caracteres em vez de String


void salvarDadosCSV(DadosInstantaneos &dadosInstantaneos, String nomeArquivo) {

    // formatar os dados
    sprintf(dados[0], "%f\0", dadosInstantaneos.coordenadas.latitude);
    sprintf(dados[1], "%f\0", dadosInstantaneos.coordenadas.longitude);
    sprintf(dados[2], "%f\0", dadosInstantaneos.coordenadas.altitude);
    sprintf(dados[3], "%f\0", dadosInstantaneos.velocidade);
    sprintf(dados[4], "%02d/%02d/%04d\0", dadosInstantaneos.data);
    sprintf(dados[5], "%02d:%02d:%02d\0", dadosInstantaneos.hora);

    File arquivo = SD.open(nomeArquivo.c_str(), FILE_APPEND);
    if (arquivo) {
        for (int i = 0; i < QTD_COLUNAS; ++i) {
            arquivo.print(dadosInstantaneos[i]);
            if (i < QTD_COLUNAS - 1) {
                arquivo.print(",");
            }
        }

        // \n no final da linha 
        arquivo.println();

    } else { Serial.println("ERRO ESCRITA");}

    arquivo.close();
}

void loop() {
  while(1) Serial.println(countFiles());
  while (GPS_SERIAL.available() > 0) {
    gps.encode(GPS_SERIAL.read());
  }

  if (gps.location.isValid() && gps.altitude.isValid() && gps.speed.isValid() && gps.date.isValid() && gps.time.isValid()) {
    Serial.println("VASCO");
    Serial.println("VASCO FIM");
  } else { Serial.println("ERRO GPS");}
}

