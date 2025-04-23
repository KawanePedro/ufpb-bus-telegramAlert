# Sistema de Notificação de Ônibus Circular UFPB

Este projeto implementa um sistema de notificação que avisa quando o ônibus circular da UFPB está se aproximando do Centro de Informática (CI). O sistema usa comunicação sem fio através de módulos nRF24L01+ e envia notificações via Telegram.

## Componentes do Sistema

O sistema é composto por três partes principais:

1. **Emissor (Arduino)**: Instalado no ônibus, envia periodicamente o sinal identificador "CIRCULAR_CI".
2. **Receptor (ESP8266/NodeMCU)**: Instalado próximo ao CI, recebe o sinal do ônibus, pisca um LED, ativa um buzzer e envia uma notificação via Telegram.
3. **Bot Telegram**: Interface para os usuários receberem as notificações em seus smartphones.

## Estrutura de Arquivos

Todos os arquivos estão na raiz do projeto:

- `emissor.ino` - Código para o Arduino que envia o sinal do ônibus
- `receptor.ino` - Código para o ESP8266 que recebe o sinal e envia notificações
- `telegram_bot.py` - Implementação do bot Telegram usando Python
- `requirements.txt` - Dependências Python para o bot Telegram
- `.env` - Arquivo para armazenar variáveis de ambiente (não incluído, deve ser criado)

## Requisitos de Hardware

### Emissor (Arduino)
- Arduino Uno/Nano/Pro Mini
- Módulo nRF24L01+

### Receptor (ESP8266/NodeMCU)
- NodeMCU ou ESP8266
- Módulo nRF24L01+
- LED (embutido no NodeMCU D4/GPIO2)
- Buzzer HXD (conectado ao pino D1/GPIO5)
- Fonte de alimentação

## Conexões de Hardware

### Emissor (Arduino)
```
nRF24L01+ -> Arduino
-----------------
VCC -> 3.3V
GND -> GND
CE  -> Pino 9
CSN -> Pino 10
SCK -> Pino 13 (SCK)
MOSI -> Pino 11 (MOSI)
MISO -> Pino 12 (MISO)
```

### Receptor (ESP8266/NodeMCU)
```
nRF24L01+ -> ESP8266/NodeMCU
-----------------
VCC -> 3.3V
GND -> GND
CE  -> GPIO4 (D2)
CSN -> GPIO15 (D8)
SCK -> GPIO14 (D5)
MOSI -> GPIO13 (D7)
MISO -> GPIO12 (D6)

Buzzer HXD -> ESP8266/NodeMCU
-----------------
+ -> GPIO5 (D1) (com resistor 100-220Ω opcional)
- -> GND
```

## Configuração

### Emissor (Arduino)
1. Carregue o código `emissor.ino` no Arduino usando a IDE Arduino
2. Verifique se as conexões dos pinos estão corretas
3. O emissor começará a enviar o sinal "CIRCULAR_CI" automaticamente a cada 5 segundos

### Receptor (ESP8266/NodeMCU)
1. Edite o arquivo `receptor.ino` para incluir:
   - Credenciais de WiFi (`ssid` e `password`)
   - Token do bot Telegram (`token`)
   - ID do chat Telegram (`chat_id`)
2. Carregue o código no ESP8266/NodeMCU usando a IDE Arduino (certifique-se de ter as bibliotecas ESP8266 instaladas)
3. O receptor começará a aguardar sinais do emissor

### Bot Telegram
1. Crie um arquivo `.env` na raiz do projeto com o seguinte conteúdo:
   ```
   BOT_TOKEN=seu_token_aqui
   ```
2. Instale as dependências:
   ```
   pip install -r requirements.txt
   ```
3. Execute o bot:
   ```
   python telegram_bot.py
   ```
4. Use o comando `/chatid` no chat do Telegram para obter o ID do chat necessário para configurar o receptor

## Como Funciona

1. O módulo emissor instalado no ônibus envia constantemente o sinal "CIRCULAR_CI"
2. O receptor próximo ao CI detecta esse sinal quando o ônibus se aproxima
3. Quando o sinal é detectado:
   - O LED pisca 5 vezes (500ms ligado, 500ms desligado) 
   - O buzzer toca por 0,5 segundos (respeitando um intervalo mínimo de 30 segundos entre toques)
   - Uma notificação é enviada via Telegram (respeitando um intervalo mínimo de 5 minutos entre notificações)
4. Os usuários do bot Telegram podem usar os seguintes comandos:
   - `/start` - Inicia o bot e mostra uma mensagem de boas-vindas
   - `/ajuda` - Exibe ajuda sobre os comandos disponíveis
   - `/info` - Mostra informações sobre o bot
   - `/horarios` - Exibe os horários do ônibus circular
   - `/chatid` - Obtém o ID do chat para configuração do receptor

## Solução de Problemas

### Emissor
- Verifique se o LED de alimentação do módulo nRF24L01+ está aceso
- Verifique se as mensagens "Sinal enviado com sucesso!" aparecem no monitor serial
- Verifique a sintaxe do código, especialmente a instrução `delay(5000)` que precisa terminar com ponto e vírgula

### Receptor
- Verifique se o ESP8266 está conectado ao WiFi (mensagem "Conectado ao WiFi")
- Teste a conexão com o Telegram usando uma mensagem de teste
- Verifique se o módulo nRF24L01+ está alimentado corretamente
- Se ocorrer o erro "Failed to connect to ESP8266: Timed out waiting for packet header" durante o upload:
  - Coloque o ESP8266 em modo de programação manualmente (pressione o botão FLASH/GPIO0 enquanto pressiona e solta o botão RESET)
  - Tente um cabo USB diferente ou outra porta USB
  - Verifique as permissões da porta serial (em Linux: `sudo chmod a+rw /dev/ttyUSB0`)

### Bot Telegram
- Certifique-se de que o bot foi iniciado corretamente
- Verifique logs para qualquer erro de comunicação com a API do Telegram
- Confirme se o arquivo `.env` contém o token correto

## Personalizações

- Ajuste o intervalo de envio do emissor alterando o valor do `delay` (atualmente 5000 ms)
- Ajuste o intervalo mínimo entre notificações alterando `notificationInterval` no receptor (atualmente 300000 ms = 5 minutos)
- Ajuste o intervalo mínimo entre toques do buzzer alterando `buzzerInterval` (atualmente 30000 ms = 30 segundos)
- Ajuste a duração do toque do buzzer alterando `buzzerDuration` (atualmente 500 ms = 0,5 segundos)
- Modifique a mensagem de notificação conforme necessário

## Limitações

- O alcance do módulo nRF24L01+ é limitado (aproximadamente 100 metros em campo aberto)
- O receptor requer conexão WiFi estável para enviar notificações
- O uso de `delay()` no loop do receptor pode afetar a responsividade do sistema