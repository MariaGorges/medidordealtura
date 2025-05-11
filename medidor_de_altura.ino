#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

float Altura = 0;
float Distancia = 0;

float getDistance(int trig, int echo) {
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  pinMode(echo, INPUT);
  return pulseIn(echo, HIGH, 30000) / 58.0;
}

float getAverageDistance(int trig, int echo, int samples = 5) {
  float soma = 0;
  for (int i = 0; i < samples; i++) {
    float leitura = getDistance(trig, echo);
    if (leitura > 0 && leitura < 400) {
      soma += leitura;
    } else {
      i--;
    }
    delay(50);
  }
  return soma / samples;
}

void imprimir_dados() {
  display.clearDisplay();
  display.drawRect(0, 0, 128, 64, WHITE);
  display.setTextSize(2);
  display.setCursor(3, 3);
  display.print("Altura:");
  display.setCursor(3, 37);
  display.print(Altura, 2);
  display.println("m");
  display.display();
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.display();
}

void loop() {
  Distancia = getAverageDistance(2, 3); // em cm

  if (Distancia < 250.0) {
    Altura =  1.80 - ((Distancia - 0.3) / 100.0); // compensação de 0.3 cm (tábua)
    imprimir_dados();
  } else {
    display.clearDisplay();
    display.setCursor(3, 3);
    display.setTextSize(1);
    display.print("Fora de alcance");
    display.display();
  }

  delay(1000);
}
