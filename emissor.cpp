#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Configuração dos pinos para Arduino
#define CE_PIN    9
#define CSN_PIN   10

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  
  // Inicializa o módulo de rádio
  if (!radio.begin()) {
    Serial.println("Falha ao iniciar o módulo nRF24L01+!");
    while(1);
  }

  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();  // Coloca o rádio no modo de transmissão

  Serial.println("Transmissor pronto. Enviando sinal 'CIRCULARCI'...");
}

void loop() {
  const char message[] = "CIRCULAR_CI";
  
  bool success = radio.write(&message, sizeof(message));
  
  if (success) {
    Serial.println("Sinal enviado com sucesso!");
    delay(5000)
  } else {
    Serial.println("Falha ao enviar o sinal!");
  }
  
  delay(5000);  // Espera 5 segundos antes de enviar novamente
}