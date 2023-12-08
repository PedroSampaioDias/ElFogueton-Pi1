#!/bin/bash

echo "Iniciando script de obtenção de dados"
# obter o nome do arquivo em que serão guardados os dados. Esse arquivo é populado ao chamar o script
# "inicar_gravacao.sh"
arquivoRetorno=$(head -n 1 filepointer)

echo "Escrevendo os dados no arquivo $arquivoRetorno"

# Loop até receber "EOF"
while true; do
    # Faz a requisição HTTP
    resposta=$(curl -s http://192.168.0.100:80/obter_dados)
    # Verifica se a resposta é "EOF\n"
    if [ "$resposta" == "EOF" ]; then
        echo "Recebeu a string 'EOF', encerrando o script."
        break
    # verificar se a ESP32 retornou alguma mensagem de erro e retorná-lo.
    elif [[ "$resposta" == *"Erro"* ]]; then
        echo "$resposta"
        break
    # popular o arquivo de resposta e printar no terminal
    else
        echo "$resposta" >> $arquivoRetorno
        echo "$resposta" >> retorno.csv
        echo "$resposta"
    fi
done

