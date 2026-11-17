#include "DHT.h"

// Definição de pinos do circuito no Wokwi
#define DHTPIN 4        // Pino de dados do DHT22
#define DHTTYPE DHT22   // Tipo do sensor

// Pinos dos LEDs indicadores (Estado Normal, Alerta e Falha)
#define LED_VERDE 12    
#define LED_VERMELHO 14 
#define LED_AZUL 27     

DHT dht(DHTPIN, DHTTYPE);

// Variáveis para controle de tempo com millis 
unsigned long tempoAnterior = 0;
const long intervaloLeitura = 2000; // Intervalo de 2 segundos (2000 ms)

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
  unsigned long tempoAtual = millis();

  // Verifica se já passou o intervalo de tempo definido 
  if (tempoAtual - tempoAnterior >= intervaloLeitura) {
    tempoAnterior = tempoAtual;

    // Leitura da temperatura em Celsius
    float temperatura = dht.readTemperature();

    // Exibição básica da leitura periódica no Monitor Serial 
    Serial.print("Leitura periodica - Temp: ");
    Serial.print(temperatura);
    Serial.println(" °C");
  }
}