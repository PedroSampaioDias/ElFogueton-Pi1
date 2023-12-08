#!/bin/bash

if [[ "$1" =~ ^[0-9]+$ || "$number" =~ ^[-][0-9]+$  ]]
then
    requisicao="curl http://192.168.0.100:80/iniciar_gravacao?idLancamento=$1"

    echo "fazendo a seguinte requisicao: $requisicao"
    $requisicao
else
    echo "O argumento enviado não é um número inteiro. Finalizando script."
fi
