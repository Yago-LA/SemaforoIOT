# 🚦 Semáforo Inteligente com Fluxo de Trânsito

## 📖 Descrição do Projeto

Este projeto apresenta um sistema de **Semáforo Inteligente baseado em Internet das Coisas (IoT)**, desenvolvido com o objetivo de otimizar o fluxo de veículos em cruzamentos urbanos. O sistema utiliza sensores ultrassônicos para detectar a presença e a proximidade de veículos em duas vias distintas, ajustando automaticamente o tempo de abertura dos semáforos conforme o fluxo identificado.

A solução foi desenvolvida utilizando o microcontrolador ESP32, comunicação MQTT para transmissão de dados em tempo real e a plataforma Node-RED para monitoramento e visualização das informações.

O projeto está alinhado ao **Objetivo de Desenvolvimento Sustentável (ODS) 11 – Cidades e Comunidades Sustentáveis**, contribuindo para o desenvolvimento de soluções voltadas à mobilidade urbana inteligente.

---

# 🎯 Objetivos

- Monitorar o fluxo de veículos em duas vias utilizando sensores ultrassônicos;
- Adaptar automaticamente o tempo dos semáforos conforme o fluxo detectado;
- Implementar comunicação IoT utilizando protocolo MQTT;
- Permitir monitoramento remoto em tempo real através do Node-RED;
- Demonstrar a aplicação prática de sistemas inteligentes de transporte (ITS).

---

# ⚙️ Funcionamento do Sistema

O sistema realiza leituras contínuas dos sensores ultrassônicos HC-SR04 instalados em cada via.

A partir das distâncias detectadas, o ESP32 calcula o fluxo de veículos e determina qual via necessita de maior tempo de abertura do sinal verde.

Quando uma via apresenta maior fluxo, o sistema aumenta automaticamente o tempo do semáforo verde correspondente.

Todas as informações são transmitidas via MQTT para monitoramento em tempo real.

Fluxo de funcionamento:

```text
Sensores HC-SR04
        ↓
      ESP32
        ↓
Broker MQTT (Mosquitto)
        ↓
     Node-RED
        ↓
 Dashboard e Monitoramento
```

---

# 💻 Software Desenvolvido

O software foi desenvolvido utilizando:

- Arduino IDE
- Linguagem C/C++
- Biblioteca WiFi.h
- Biblioteca PubSubClient

Principais funcionalidades:

- Conexão Wi-Fi automática;
- Comunicação MQTT;
- Leitura dos sensores ultrassônicos;
- Controle dos LEDs do semáforo;
- Lógica adaptativa baseada no fluxo de veículos;
- Publicação de eventos em tempo real;
- Integração com Node-RED.

---

# 🛠️ Hardware Utilizado

## Plataforma de Desenvolvimento

### ESP32

Microcontrolador responsável pelo processamento dos dados dos sensores, acionamento dos atuadores e comunicação com a internet.

---

## Sensores

### HC-SR04 (2 unidades)

Sensores ultrassônicos responsáveis pela detecção da distância dos veículos em cada via.

Funções:

- Medir distância;
- Detectar presença de veículos;
- Determinar o fluxo de trânsito.

---

## Atuadores

### LEDs (6 unidades)

Representam os estados dos semáforos.

#### Via 1
- LED Verde
- LED Amarelo
- LED Vermelho

#### Via 2
- LED Verde
- LED Amarelo
- LED Vermelho

---

## Componentes Auxiliares

- Protoboard
- Resistores de 220 Ω
- Jumpers
- Cabos de conexão

---

# 🌐 Comunicação e Protocolos

## Protocolo MQTT

O sistema utiliza o protocolo MQTT (Message Queuing Telemetry Transport) para comunicação entre os dispositivos.

Características:

- Baixo consumo de banda;
- Comunicação em tempo real;
- Arquitetura Publisher/Subscriber;
- Amplamente utilizado em aplicações IoT.

---

## Broker MQTT

Broker utilizado:

Eclipse Mosquitto

Função:

- Receber mensagens do ESP32;
- Gerenciar tópicos MQTT;
- Distribuir informações aos clientes conectados.

---

## Tópicos MQTT

| Tópico | Função |
|----------|----------|
| semaforo/via1/fluxo | Fluxo detectado na Via 1 |
| semaforo/via2/fluxo | Fluxo detectado na Via 2 |
| semaforo/via1/tempo | Tempo calculado para a Via 1 |
| semaforo/via2/tempo | Tempo calculado para a Via 2 |
| semaforo/status | Estado atual dos semáforos |
| semaforo/eventos | Eventos e decisões do sistema |

---

# 📊 Monitoramento com Node-RED

O Node-RED é utilizado para:

- Receber dados MQTT;
- Exibir informações em tempo real;
- Mostrar eventos do sistema;
- Gerar gráficos;
- Criar dashboards para monitoramento.

O dashboard permite acompanhar:

- Fluxo das vias;
- Tempo dos semáforos;
- Mudanças de estado;
- Eventos gerados pelo sistema.

---

# 🔧 Como Executar o Projeto

## 1. Simulação

Abrir o projeto na plataforma Wokwi.

## 2. Programação

Carregar o código-fonte no ESP32.

## 3. MQTT

Configurar o broker MQTT:

```text
Broker: broker.hivemq.com
Porta: 1883
```

## 4. Node-RED

Criar um nó MQTT configurado para:

```text
semaforo/#
```

Conectar os dados aos componentes:

- Debug
- Gauge
- Text
- Chart

---

# 📈 Resultados Obtidos

O sistema foi capaz de:

- Detectar o fluxo de veículos;
- Adaptar automaticamente o tempo dos semáforos;
- Publicar informações em tempo real via MQTT;
- Exibir os dados em dashboards Node-RED;
- Demonstrar a viabilidade da aplicação de IoT em sistemas de mobilidade urbana.

---

# 🚀 Melhorias Futuras

- Integração com câmeras de visão computacional;
- Aplicação de Inteligência Artificial;
- Integração entre múltiplos cruzamentos;
- Priorização de veículos de emergência;
- Desenvolvimento de protótipo físico em escala real.

---

# 👨‍💻 Autor

Yago Linos dos Anjos

Projeto desenvolvido para a disciplina de Objetos Inteligentes Conectados.

---

# 📚 Referências

- MQTT. Disponível em: http://mqtt.org
- Eclipse Mosquitto. Disponível em: https://mosquitto.org
- Node-RED. Disponível em: https://nodered.org
- Wokwi Simulator. Disponível em: https://wokwi.com
- Arduino. Disponível em: https://www.arduino.cc
