#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Definições de pinos
#define DHTPIN 15
#define DHTTYPE DHT22
#define LDR_PIN 32
#define RAINPIN 34
#define WIND_PIN 33

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Wi-Fi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ThingSpeak
const char* apiKey = "97AWEIZ0IGRIIJKN";
const char* server = "http://api.thingspeak.com";

DHT dht(DHTPIN, DHTTYPE);

// Função para classificar categoria de furacão
int classificarFuracao(int velocidade) {
  if (velocidade < 119) return 0;
  else if (velocidade < 154) return 1;
  else if (velocidade < 178) return 2;
  else if (velocidade < 209) return 3;
  else if (velocidade < 252) return 4;
  else return 5;
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(LDR_PIN, INPUT);
  pinMode(RAINPIN, INPUT);
  pinMode(WIND_PIN, INPUT);
  analogReadResolution(12);

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("SafeZone");
  lcd.setCursor(4, 1);
  lcd.print("System");
  delay(2000);
  lcd.clear();

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void loop() {
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  int chuva = analogRead(RAINPIN);
  int luz = analogRead(LDR_PIN);
  int ventoRaw = analogRead(WIND_PIN);
  int ventoKmh = map(ventoRaw, 0, 4095, 0, 300);
  int categoriaFuracao = classificarFuracao(ventoKmh);

  chuva = map(chuva, 0, 4095, 100, 0); // 100 = molhado, 0 = seco

  Serial.printf("Vento raw: %d\n", ventoRaw);
  Serial.printf("Temp: %.1f C, Umidade: %.1f, Vento: %d km/h, Chuva: %d, Luz: %d, Categoria: %d\n",
                temperatura, umidade, ventoKmh, chuva, luz, categoriaFuracao);

  lcd.setCursor(0, 0);
  lcd.printf("T:%.1fC H:%.0f", temperatura, umidade);

  lcd.setCursor(0, 1);
  lcd.print("                "); // limpa linha
  lcd.setCursor(0, 1);

  if (categoriaFuracao > 0) {
    lcd.printf("Furacao Cat:%d", categoriaFuracao);
  } else {
    lcd.printf("R:%d L:%s", chuva, luz > 2000 ? "Alta" : "Baixa");
  }

  // Enviar para ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(server) + "/update?api_key=" + apiKey +
                 "&field1=" + String(temperatura) +
                 "&field2=" + String(umidade) +
                 "&field3=" + String(ventoKmh) +
                 "&field4=" + String(luz) +
                 "&field5=" + String(categoriaFuracao) +
                 "&field6=" + String(chuva);
    http.begin(url);
    int httpResponseCode = http.GET();
    Serial.print("HTTP Response: ");
    Serial.println(httpResponseCode);
    http.end();
  } else {
    Serial.println("WiFi desconectado.");
  }

  delay(10000); // 10 segundos entre atualizações
}
