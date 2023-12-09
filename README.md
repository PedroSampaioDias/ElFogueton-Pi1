# ElFogueton-Pi1


## Scripts shell
- `apagar_dados.sh`: chame-o para apagar os dados do arquivo que está sendo gravado no msd. Chame-o antes de cada lançamento.

- `iniciar_gravacao.sh`: chame-o para iniciar a gravação dos dados no microSD. Modo de uso: `bash iniciar_gravacao.sh <numero do lançamento>`. O <número do lançamento> deve ser um inteiro.

- `main.ino`: arquivo principal da esp32, que deve ser compilado via arduino IDE. 

- `obter_dados.sh`: obter os dados de um lançamento.

- `obter_quantidade_satelites.sh`: obtém a quantidade de satélites que o GPS está utilizando.

- `obter_tamanho_arquivo.sh`: obtém o tamanho do arquivo de dados de vôo.

- `parar_gravacao.sh`: parar a gravação de dados no MSD.

- `reiniciar_esp.sh`: 
status_gravacao.sh
