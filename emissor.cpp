#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

// Configuração dos pinos para NodeMCU
#define CE_PIN    D2  // GPIO4
#define CSN_PIN   D8  // GPIO15

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
const long notificationInterval = 300000; 

void setup() {
 Serial.begin(115200);
  
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
   if (radio.available()) {
    char receivedMessage[32] = "";
    radio.read(&receivedMessage, sizeof(receivedMessage));
    
    Serial.print("Mensagem recebida: ");
    Serial.println(receivedMessage);
    
    // Verifica se é a mensagem do ônibus
    if (String(receivedMessage) == "CIRCULARCI") {
      // Verifica se já passou tempo suficiente desde a última notificação
      if (millis() - lastNotification > notificationInterval) {
        sendTelegramNotification("🚌 Ônibus Circular está se aproximando do CI!");
        lastNotification = millis();
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