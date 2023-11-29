#include <SD.h>
#include "persistencia.h"

#define QTD_COLUNAS 6

void salvarDadosCSV(char dados[6][20], String nomeArquivo) {
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
