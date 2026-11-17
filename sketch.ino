#include "DHT.h"

// Definição de pinos do circuito no Wokwi
#define DHTPIN 4        // Pino de dados do DHT22
#define DHTTYPE DHT22   // Tipo do sensor

// Pinos dos LEDs indicadores
#define LED_VERDE 12    // Estado Normal
#define LED_VERMELHO 14 // Estado Alerta
#define LED_AZUL 27     // Estado Falha (guardado para o próximo commit)

DHT dht(DHTPIN, DHTTYPE);

// RF01: Controle de tempo para aquisição periódica
unsigned long tempoAnterior = 0;
const unsigned long INTERVALO_LEITURA = 2000; // Intervalo de 2 segundos (2000 ms)

// RF03: Limite de temperatura definido no sistema
const float LIMITE_TEMP = 30.0;

// RF04: Estados do sistema
enum EstadoSistema {
  ESTADO_NORMAL,
  ESTADO_ALERTA
};

EstadoSistema estadoAtual = ESTADO_NORMAL;

// RF02: Função para apresentar o valor da leitura e o estado do sistema (Serial + LEDs)
void atualizarSaida(float temperatura, EstadoSistema estado) {
  if (estado == ESTADO_ALERTA) {
    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AZUL, LOW);

    Serial.print("Temp: ");
    Serial.print(temperatura, 1);
    Serial.print(" °C | Limite: ");
    Serial.print(LIMITE_TEMP, 1);
    Serial.println(" °C | Estado: ALERTA");
  } else {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(LED_AZUL, LOW);

    Serial.print("Temp: ");
    Serial.print(temperatura, 1);
    Serial.print(" °C | Limite: ");
    Serial.print(LIMITE_TEMP, 1);
    Serial.println(" °C | Estado: NORMAL");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("=== Sistema de Monitoramento de Temperatura ===");
  Serial.print("Intervalo de aquisicao: ");
  Serial.print(INTERVALO_LEITURA);
  Serial.println(" ms");

  dht.begin();

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
}

void loop() {
  unsigned long tempoAtual = millis();

  // RF01: Leitura periódica
  if (tempoAtual - tempoAnterior >= INTERVALO_LEITURA) {
    tempoAnterior = tempoAtual;

    float temperatura = dht.readTemperature();

    // RF03 e RF04: Avaliação de limite e transição de estados
    if (temperatura > LIMITE_TEMP) {
      estadoAtual = ESTADO_ALERTA;
    } else {
      estadoAtual = ESTADO_NORMAL;
    }

    // RF02: Apresentação na saída apropriada
    atualizarSaida(temperatura, estadoAtual);
  }
}