#include <WiFi.h>
#include <PubSubClient.h>

// ================= WIFI WOKWI =================
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ================= MQTT =================
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

const char* topic_via1 = "semaforo/via1/fluxo";
const char* topic_via2 = "semaforo/via2/fluxo";
const char* topic_status = "semaforo/status";
const char* topic_tempo_via1 = "semaforo/via1/tempo";
const char* topic_tempo_via2 = "semaforo/via2/tempo";
const char* topic_eventos = "semaforo/eventos";

WiFiClient espClient;
PubSubClient client(espClient);

// ================= SENSORES =================
#define TRIG1 5
#define ECHO1 18

#define TRIG2 17
#define ECHO2 16

// ================= LEDS VIA 1 =================
#define V1_VERDE 23
#define V1_AMARELO 22
#define V1_VERMELHO 21

// ================= LEDS VIA 2 =================
#define V2_VERDE 19
#define V2_AMARELO 4
#define V2_VERMELHO 2

// ================= TEMPOS =================
const int TEMPO_AMARELO = 2000;
const int TEMPO_BAIXO = 4000;
const int TEMPO_MEDIO = 7000;
const int TEMPO_ALTO = 10000;

const int LIMITE_VEICULO_CM = 50;

// ================= WIFI =================
void setup_wifi() {
  Serial.println("Conectando ao WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// ================= MQTT =================
void reconnect_mqtt() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT... ");

    String clientId = "ESP32-Semaforo-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("conectado!");
      client.publish(topic_status, "ESP32 conectado ao MQTT");
      client.publish(topic_eventos, "Sistema iniciado e conectado ao MQTT");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 3 segundos");
      delay(3000);
    }
  }
}

// ================= PUBLICAR EVENTOS =================
void publicarEvento(String mensagem) {
  Serial.println(mensagem);
  client.publish(topic_eventos, mensagem.c_str());
}

// ================= SENSOR =================
long medirDistanciaCM(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH, 30000);

  if (duracao == 0) {
    return 999;
  }

  long distancia = duracao * 0.034 / 2;
  return distancia;
}

// ================= LEDS =================
void apagarTodos() {
  digitalWrite(V1_VERDE, LOW);
  digitalWrite(V1_AMARELO, LOW);
  digitalWrite(V1_VERMELHO, LOW);

  digitalWrite(V2_VERDE, LOW);
  digitalWrite(V2_AMARELO, LOW);
  digitalWrite(V2_VERMELHO, LOW);
}

void via1Verde() {
  apagarTodos();
  digitalWrite(V1_VERDE, HIGH);
  digitalWrite(V2_VERMELHO, HIGH);

  client.publish(topic_status, "Via 1 verde / Via 2 vermelho");
  publicarEvento("Atualização: Via 1 ficou VERDE e Via 2 ficou VERMELHA");
}

void via1Amarelo() {
  apagarTodos();
  digitalWrite(V1_AMARELO, HIGH);
  digitalWrite(V2_VERMELHO, HIGH);

  client.publish(topic_status, "Via 1 amarelo / Via 2 vermelho");
  publicarEvento("Atualização: Via 1 ficou AMARELA e Via 2 continuou VERMELHA");
}

void via2Verde() {
  apagarTodos();
  digitalWrite(V1_VERMELHO, HIGH);
  digitalWrite(V2_VERDE, HIGH);

  client.publish(topic_status, "Via 1 vermelho / Via 2 verde");
  publicarEvento("Atualização: Via 2 ficou VERDE e Via 1 ficou VERMELHA");
}

void via2Amarelo() {
  apagarTodos();
  digitalWrite(V1_VERMELHO, HIGH);
  digitalWrite(V2_AMARELO, HIGH);

  client.publish(topic_status, "Via 1 vermelho / Via 2 amarelo");
  publicarEvento("Atualização: Via 2 ficou AMARELA e Via 1 continuou VERMELHA");
}

// ================= LÓGICA ADAPTATIVA =================
int calcularTempoVerde(long distancia) {
  if (distancia <= 15) {
    return TEMPO_ALTO;
  } else if (distancia <= 35) {
    return TEMPO_MEDIO;
  } else if (distancia <= LIMITE_VEICULO_CM) {
    return TEMPO_BAIXO;
  } else {
    return TEMPO_BAIXO;
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);

  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  pinMode(V1_VERDE, OUTPUT);
  pinMode(V1_AMARELO, OUTPUT);
  pinMode(V1_VERMELHO, OUTPUT);

  pinMode(V2_VERDE, OUTPUT);
  pinMode(V2_AMARELO, OUTPUT);
  pinMode(V2_VERMELHO, OUTPUT);

  apagarTodos();

  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);

  randomSeed(micros());
}

// ================= LOOP =================
void loop() {
  if (!client.connected()) {
    reconnect_mqtt();
  }

  client.loop();

  long distanciaVia1 = medirDistanciaCM(TRIG1, ECHO1);
  long distanciaVia2 = medirDistanciaCM(TRIG2, ECHO2);

  String msgVia1 = String(distanciaVia1) + " cm";
  String msgVia2 = String(distanciaVia2) + " cm";

  client.publish(topic_via1, msgVia1.c_str());
  client.publish(topic_via2, msgVia2.c_str());

  Serial.print("Via 1: ");
  Serial.print(msgVia1);
  Serial.print(" | Via 2: ");
  Serial.println(msgVia2);

  int tempoVia1 = calcularTempoVerde(distanciaVia1);
  int tempoVia2 = calcularTempoVerde(distanciaVia2);

  String msgTempoVia1 = String(tempoVia1 / 1000) + " segundos";
  String msgTempoVia2 = String(tempoVia2 / 1000) + " segundos";

  Serial.print("Novo tempo Via 1: ");
  Serial.println(msgTempoVia1);

  Serial.print("Novo tempo Via 2: ");
  Serial.println(msgTempoVia2);

  client.publish(topic_tempo_via1, msgTempoVia1.c_str());
  client.publish(topic_tempo_via2, msgTempoVia2.c_str());

  if (distanciaVia1 < distanciaVia2) {
    publicarEvento("Decisão: Via 1 recebeu prioridade por apresentar maior fluxo");

    via1Verde();
    delay(tempoVia1);

    via1Amarelo();
    delay(TEMPO_AMARELO);

    via2Verde();
    delay(TEMPO_BAIXO);

    via2Amarelo();
    delay(TEMPO_AMARELO);
  } else if (distanciaVia2 < distanciaVia1) {
    publicarEvento("Decisão: Via 2 recebeu prioridade por apresentar maior fluxo");

    via2Verde();
    delay(tempoVia2);

    via2Amarelo();
    delay(TEMPO_AMARELO);

    via1Verde();
    delay(TEMPO_BAIXO);

    via1Amarelo();
    delay(TEMPO_AMARELO);
  } else {
    publicarEvento("Decisão: fluxo equilibrado entre as vias");

    via1Verde();
    delay(TEMPO_BAIXO);

    via1Amarelo();
    delay(TEMPO_AMARELO);

    via2Verde();
    delay(TEMPO_BAIXO);

    via2Amarelo();
    delay(TEMPO_AMARELO);
  }
}
