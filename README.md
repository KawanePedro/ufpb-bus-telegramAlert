# Sistema de Notificação de Ônibus Circular UFPB

Este projeto implementa um sistema de notificação que avisa quando o ônibus circular da UFPB está se aproximando do Centro de Informática (CI). O sistema usa comunicação sem fio através de módulos nRF24L01+ e envia notificações via Telegram.

## Componentes do Sistema

O sistema é composto por três partes principais:

1. **Emissor (Arduino)**: Instalado no ônibus, envia periodicamente um sinal identificador.
2. **Receptor (ESP8266/NodeMCU)**: Instalado próximo ao CI, recebe o sinal do ônibus e envia uma notificação via Telegram.
3. **Bot Telegram**: Interface para os usuários receberem as notificações em seus smartphones.

## Estrutura de Arquivos

Todos os arquivos estão na raiz do projeto:

- `emissor.cpp` - Código para o Arduino que envia o sinal do ônibus
- `receptor.cpp` - Código para o ESP8266 que recebe o sinal e envia notificações
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
```

## Configuração

### Emissor (Arduino)
1. Carregue o código `emissor.cpp` no Arduino usando a IDE Arduino
2. Verifique se as conexões dos pinos estão corretas
3. O emissor começará a enviar sinais automaticamente a cada 5 segundos

### Receptor (ESP8266/NodeMCU)
1. Edite o arquivo `receptor.cpp` para incluir:
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

1. O módulo emissor instalado no ônibus envia constantemente o sinal "CIRCULARCI"
2. O receptor próximo ao CI detecta esse sinal quando o ônibus se aproxima
3. O receptor envia uma notificação via Telegram para o chat configurado
4. Para evitar múltiplas notificações, há um intervalo mínimo de 5 minutos entre notificações

## Solução de Problemas

### Emissor
- Verifique se o LED de alimentação do módulo nRF24L01+ está aceso
- Verifique se as mensagens "Sinal enviado com sucesso!" aparecem no monitor serial

### Receptor
- Verifique se o ESP8266 está conectado ao WiFi (mensagem "Conectado ao WiFi")
- Teste a conexão com o Telegram usando uma mensagem de teste
- Verifique se o módulo nRF24L01+ está alimentado corretamente

### Bot Telegram
- Certifique-se de que o bot foi iniciado corretamente
- Verifique logs para qualquer erro de comunicação com a API do Telegram

## Personalizações

- Ajuste o intervalo de envio do emissor alterando o valor do `delay` (em milissegundos)
- Ajuste o intervalo mínimo entre notificações alterando `notificationInterval` no receptor
- Modifique a mensagem de notificação conforme necessário

## Limitações

- O alcance do módulo nRF24L01+ é limitado (aproximadamente 100 metros em campo aberto)
- O receptor requer conexão WiFi estável para enviar notificações