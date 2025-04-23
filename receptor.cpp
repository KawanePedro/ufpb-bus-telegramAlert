#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

// Configuração dos pinos para NodeMCU
#define CE_PIN   4    // GPIO4
#define CSN_PIN  15   // GPIO15
#define LED_PIN  2    // GPIO2 (D4 no NodeMCU) - LED embutido
#define BUZZER_PIN 5  // GPIO5 (D1 no NodeMCU) - Buzzer

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

// Configurações WiFi
const char* ssid = ""; //nome da rede wifi
const char* password = ""; //senha da rede wifi

// Configurações do Telegram
const String token = ""; // token do bot
const String chat_id = ""; // id do bot

// Variáveis para controle
unsigned long lastNotification = 0;
const long notificationInterval = 300000; // 5 minutos

// Variáveis para controle do buzzer
unsigned long lastBuzzerTime = 0;
const long buzzerInterval = 30000;   // Tempo mínimo entre toques (30 segundos)
const long buzzerDuration = 500;    // Duração do toque (meio segundo)

void setup() {
  Serial.begin(115200);
  
  // Configura os pinos de saída
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Inicializa com o LED e buzzer desligados
  digitalWrite(LED_PIN, HIGH);  // LED embutido é ativo em LOW
  digitalWrite(BUZZER_PIN, LOW);
  
  // Inicializa WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao WiFi");
  
  // Inicializa o módulo de rádio
  if (!radio.begin()) {
    Serial.println("Falha ao iniciar o módulo nRF24L01+!");
    while(1);
  }

  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();

  Serial.println("Receptor pronto. Aguardando sinal do ônibus...");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Verifica se há mensagem disponível
  if (radio.available()) {
    char receivedMessage[32] = "";
    radio.read(&receivedMessage, sizeof(receivedMessage));
    
    Serial.print("Mensagem recebida: ");
    Serial.println(receivedMessage);
    
    // Verifica se é a mensagem do ônibus
    if (String(receivedMessage) == "CIRCULAR_CI") {
      // Pisca o LED quando recebe a mensagem específica
      blinkLED();
      
      // Verifica se já passou tempo suficiente desde a última notificação
      if (currentMillis - lastNotification > notificationInterval) {
        sendTelegramNotification("🚌 Ônibus Circular está se aproximando do CI!");
        lastNotification = currentMillis;
      }
      
      // Ativa o buzzer se já passou tempo suficiente desde o último toque
      if (currentMillis - lastBuzzerTime > buzzerInterval) {
        digitalWrite(BUZZER_PIN, HIGH);
        Serial.println("Buzzer ligado");
        lastBuzzerTime = currentMillis;
        
        // Programa o desligamento do buzzer
        delay(buzzerDuration);
        digitalWrite(BUZZER_PIN, LOW);
        Serial.println("Buzzer desligado");
      }
    }
  }
  
  delay(10);
}

void sendTelegramNotification(String message) {
  WiFiClientSecure client;
  client.setInsecure(); // Para simplificar, mas considere adicionar certificado
  
  if (!client.connect("api.telegram.org", 443)) {
    Serial.println("Falha na conexão com o Telegram");
    return;
  }
  
  String url = "/bot" + token + "/sendMessage?chat_id=" + chat_id + "&text=" + message;
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: api.telegram.org\r\n" +
               "Connection: close\r\n\r\n");
  
  Serial.println("Notificação enviada para o Telegram");
  
  // Aguarda resposta
  delay(500);
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
}

// Função para piscar o LED quando recebe mensagem específica
void blinkLED() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, LOW);  // Liga o LED (ativo em LOW)
    delay(500);
    digitalWrite(LED_PIN, HIGH); // Desliga o LED
    delay(500);
  }
}