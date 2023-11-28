#ifndef __PERSISTENCIA_H__
#define __PERSISTENCIA_H__
#include "gps.h"

/**
 * Conta quantos arquivos tem no diretório raíz.
 * @returns A quantidade de arquivos que existe no diretório raíz.
 */
int countFiles();

/**
 * Salva os dados num arquivo csv.
 * @params DadosInstantaneos &dadosInstantaneos - Os dados a serem salvos.
 * @params String nomeArquivo - Nome do arquivo no qual serão guardados os dados.
 */
void salvarDadosCSV(DadosInstantaneos &dadosInstantaneos, String nomeArquivo);
#endif
