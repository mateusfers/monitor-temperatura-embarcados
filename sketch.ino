#include "DHT.h"

// Definição de pinos do circuito no Wokwi
#define DHTPIN 4        // Pino de dados do DHT22
#define DHTTYPE DHT22   // Tipo do sensor

// Pinos dos LEDs indicadores (Estado Normal, Alerta e Falha)
#define LED_VERDE 12    
#define LED_VERMELHO 14 
#define LED_AZUL 27     

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Inicialização da comunicação serial
  Serial.begin(115200);
  Serial.println("Iniciando Sistema de Monitoramento de Temperatura...");

  // Inicialização do sensor
  dht.begin();

  // Configuração dos pinos dos LEDs como saída de hardware
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
}

void loop() {
  // Estrutura base inicial (as próximas lógicas virão nos próximos commits)
  delay(2000);
}