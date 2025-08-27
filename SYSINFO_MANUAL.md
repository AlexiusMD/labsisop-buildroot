# System Status HTTP Server

## Instruções para Execução do Programa


## Capturas de Tela das Respostas do Endpoint /status

Abaixo encontram-se capturas de tela contendo o output do programa em execução:

![Captura de tela contendo XXX](screenshots/example.png)


## Explicação de Como Cada Informação é Obtida de /proc e /sys

Abaixo, uma explicação breve de como cada campo do JSON de resposta é extraído dos arquivos em `/proc` e `/sys`. O código usa funções auxiliares para ler esses arquivos e processar os dados.

- **datetime**: Obtido de `/sys/class/rtc/rtc0/since_epoch`, que fornece o tempo em segundos desde o epoch. Convertido para formato ISO usando `datetime.fromtimestamp`.

- **uptime_seconds**: Lido de `/proc/uptime`, pegando o primeiro valor da linha (tempo de uptime em segundos).

- **cpu**: Extraído de `/proc/cpuinfo`. Campos como "model name" e "cpu MHz" são parseados diretamente. O "usage_percent" é calculado aplicando [uma fórmula](https://gist.github.com/pcolby/6558833) no output extraído de `/proc/stat`.

- **memory**: Vem de `/proc/meminfo`. Calcula "total_mb" e "used_mb" a partir de "MemTotal" e "MemFree", convertendo de KB para MB.

- **os_version**: Lido integralmente de `/proc/version`, que contém a versão do kernel e detalhes de compilação.

- **processes**: Lista processos varrendo diretórios numéricos em `/proc` (PIDs). Para cada PID, lê o nome do processo de `/proc/<PID>/comm`.

- **disks**: Parseado de `/proc/partitions`, ignorando as linhas de cabeçalho. Extrai o dispositivo e tamanho (em MB, convertido de KB) das colunas relevantes.

- **usb_devices**: Varre diretórios em `/sys/bus/usb/devices`. Para cada dispositivo válido, lê a descrição de `/sys/bus/usb/devices/<dir>/product` e associa à porta.

- **network_adapters**: Lido de `/proc/net/dev`, pulando cabeçalhos. Extrai nomes de interfaces; o "ip_address" está vazio (PENDENTE).