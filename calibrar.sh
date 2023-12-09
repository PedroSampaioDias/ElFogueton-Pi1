bash iniciar_gravacao.sh -1
bash parar_gravacao.sh
bash apagar_dados.sh

arquivoRetorno=$(head -n 1 filepointer)
rm $arquivoRetorno

bash obter_quantidade_satelites.sh
