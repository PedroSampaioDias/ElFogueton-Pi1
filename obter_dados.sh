#!/bin/bash

echo "Iniciando script de obtenção de dados"

# Loop até receber "EOF"
while true; do
    # Faz a requisição HTTP
    resposta=$(curl -s http://192.168.0.100:80/obter_dados)
    
    # Verifica se a resposta é "EOF\n"
    if [ "$resposta" == "EOF" ]; then
        echo "Recebeu a string 'EOF', encerrando o script."
        break
    elif [[ "$resposta" == *"Erro"* ]]; then
        echo "$resposta"
        break
    else
        echo "$resposta" >> retorno.csv
    fi
    
    # Aguarda um intervalo de tempo antes da próxima requisição (opcional)
done

