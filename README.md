# 🌪️ SafeZone - Monitor de Furacões com ESP32

Projeto acadêmico que simula um sistema de alerta de furacões usando a placa **ESP32** com sensores ambientais e envio de dados para o **ThingSpeak**. Também exibe as informações em um display LCD.

## 🧰 Componentes Utilizados

- ESP32 DevKit
- Sensor de temperatura/umidade **DHT22**
- Sensor de luz **LDR**
- Sensor de chuva (analógico)
- Potenciômetro (simula sensor de vento)
- Display LCD 16x2 com interface I2C
- Protoboard e jumpers

## ⚙️ Funcionalidades

- Leitura de **temperatura**, **umidade**, **chuva**, **luminosidade** e **vento**
- Classificação do furacão de acordo com a escala de Saffir-Simpson (de 0 a 5)
- Exibição das informações em **LCD I2C**
- Envio dos dados a cada 10 segundos para o **ThingSpeak**

## 📡 Wi-Fi

Conexão automática à rede:
```cpp
const char* ssid = "Wokwi-GUEST";
const char* password = "";
```

## 🌍 ThingSpeak

- Dados enviados para: [`ThingSpeak - Canal SafeZone`](https://thingspeak.mathworks.com/channels/2983253)
- Chave de API usada:
```cpp
const char* apiKey = "97AWEIZ0IGRIIJKN";
```

### Campos enviados:
| Campo       | Dados                  |
|-------------|------------------------|
| field1      | Temperatura (°C)       |
| field2      | Umidade (%)            |
| field3      | Velocidade do vento    |
| field4      | Luminosidade (LDR)     |
| field5      | Categoria do furacão   |
| field6      | Chuva (0 a 100)        |

## 📟 Interface LCD

- Linha 1: Temperatura e umidade
- Linha 2: Categoria do furacão ou estado da chuva e luz

Exemplo:
```
T:26.5C H:60
Furacao Cat:2
```

## 🌀 Classificação do Furacão

| Categoria | Velocidade (km/h) |
|-----------|-------------------|
| 0         | < 119             |
| 1         | 119–153           |
| 2         | 154–177           |
| 3         | 178–208           |
| 4         | 209–251           |
| 5         | ≥ 252             |


## 🚀 Como Rodar

1. Faça upload do código no ESP32 (usando Arduino IDE ou Wokwi).
2. O dispositivo se conecta ao Wi-Fi e começa a enviar dados ao ThingSpeak.
3. As informações são exibidas no display LCD.
4. Os dados podem ser monitorados remotamente no ThingSpeak:

👉 [`Acesso para o thingspeak`](https://thingspeak.mathworks.com/channels/2983253)

---
**Projeto:** SafeZone - Sistema de Alerta de Furacões com IoT
