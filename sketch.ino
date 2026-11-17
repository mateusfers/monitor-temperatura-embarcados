#include "DHT.h"

// Definição de pinos do circuito no Wokwi
#define DHTPIN 4        // Pino de dados do DHT22
#define DHTTYPE DHT22   // Tipo do sensor

// Pinos dos LEDs indicadores
#define LED_VERDE 12    // Estado Normal
#define LED_VERMELHO 14 // Estado Alerta
#define LED_AZUL 27     // Estado Falha

DHT dht(DHTPIN, DHTTYPE);

// RF01: Controle de tempo para aquisição periódica
unsigned long tempoAnterior = 0;
const unsigned long INTERVALO_LEITURA = 2000; // Intervalo de 2000 ms (2 segundos)

// RF03: Limite de temperatura definido no sistema
const float LIMITE_TEMP = 30.0;

// Faixa física operacional do sensor DHT22 para validação adicional
const float TEMP_MIN_VALIDA = -40.0;
const float TEMP_MAX_VALIDA = 80.0;

// RF04 e RF05: Máquina de estados completa do sistema
enum EstadoSistema {
  ESTADO_NORMAL,
  ESTADO_ALERTA,
  ESTADO_FALHA
};

EstadoSistema estadoAtual = ESTADO_NORMAL;

// RF05: Função que verifica se a leitura do sensor é válida
bool leituraValida(float temperatura) {
  if (isnan(temperatura)) {
    return false;
  }
  if (temperatura < TEMP_MIN_VALIDA || temperatura > TEMP_MAX_VALIDA) {
    return false;
  }
  return true;
}

// RF02: Função para apresentar o valor da leitura e o estado do sistema (Serial + LEDs)
void atualizarSaida(float temperatura, EstadoSistema estado) {
  if (estado == ESTADO_FALHA) {
    digitalWrite(LED_AZUL, HIGH);
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, LOW);

    Serial.println("Temp: INVALIDA | Estado: FALHA (Erro na leitura do sensor)");
  } else if (estado == ESTADO_ALERTA) {
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

    // RF03, RF04 e RF05: Avaliação de falha, limite e transição de estados
    if (!leituraValida(temperatura)) {
      estadoAtual = ESTADO_FALHA;
    } else if (temperatura > LIMITE_TEMP) {
      estadoAtual = ESTADO_ALERTA;
    } else {
      estadoAtual = ESTADO_NORMAL;
    }

    // RF02: Apresentação na saída apropriada
    atualizarSaida(temperatura, estadoAtual);
  }
}