#!/bin/bash

# verificar se o valor passado por linha de comando é um número inteiro.
if [[ "$1" =~ ^[0-9]+$ || "$1" =~ ^[-][0-9]+$  ]]
then
    # indicar qual o nome do arquivo aonde os dados serão salvos.
    # O nome do arquivo se encontra na primeira linha do arquivo "filepointer"

    nomeArquivo="retorno_lancamento_$1.csv"
    echo "$nomeArquivo" > filepointer
    echo "Os dados serão salvos no arquivo $nomeArquivo."

    # fazer uma requisicao para o endpoint de início de gravação
    curl http://192.168.0.100:80/iniciar_gravacao
else
    echo "O argumento enviado não é um número inteiro. Finalizando script."
    echo "Forma correta de uso: bash inicar_gravacao.sh <numero da gravação>"
    echo "Exemplo: bash iniciar_gravacao.sh 1"
fi
