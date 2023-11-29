#ifndef __PERSISTENCIA_H__
#define __PERSISTENCIA_H__

/**
 * Salva os dados em um arquivo CSV.
 * @param dados char[][20] - Os dados a serem salvos.
 * @param nomeArquivo String - Nome do arquivo no qual os dados serão armazenados.
 */
void salvarDadosCSV(char dados[6][20], String nomeArquivo);

#endif
