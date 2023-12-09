# ElFogueton-Pi1

Repositório com o código de Hardware do foguete da equipe ElFogueton, em PI1. 

O servidor é uma ESP32 conectada com um GPS e um MicroSD. O servidor responde a um cliente. As requisições podem ser feitas via os scripts shell que estão explicados abaixo.




## Scripts shell
- `apagar_dados.sh`: chame-o para apagar os dados do arquivo que está sendo gravado no msd. Chame-o antes de cada lançamento.
- `iniciar_gravacao.sh`: chame-o para iniciar a gravação dos dados no microSD. Modo de uso: `bash iniciar_gravacao.sh <numero do lançamento>`. O <número do lançamento> deve ser um inteiro.
- `obter_dados.sh`: obter os dados de um lançamento.
- `obter_quantidade_satelites.sh`: obtém a quantidade de satélites que o GPS está utilizando.
- `obter_tamanho_arquivo.sh`: obtém o tamanho do arquivo de dados de vôo.
- `parar_gravacao.sh`: parar a gravação de dados no MSD.
- `reiniciar_esp.sh`: reiniciar a esp32.
- `status_gravacao.sh`: obtém o status da gravação, isto é, se a esp32 está gravando ou não os arquivos no microSD.



## Workflow

1. Calibre o gps. Use `bash calibrar.sh`.
2. Inicie a gravação de dados. Use `bash iniciar_gravacao.sh <número do lançamento>`, onde o número do lançamento (inteiro) indica qual lançamento será gravado. Exemplo: `bash iniciar_gravacao.sh 1`.
3. Pare a gravação dos dados. Use `bash parar_gravacao.sh`.
4. Obtenha os dados. Use `bash obter_dados.sh`. Esse script popula um arquivo chamado `retorno_lancamento_<número do lançamento>.csv`. O número do lançamento é o mesmo que foi dado no passo 2.
5. Apague o arquivo que está no SD card. Use `bash apagar_dados.sh`.
6. Repita os passos anteriores para o próximo lançamento.
