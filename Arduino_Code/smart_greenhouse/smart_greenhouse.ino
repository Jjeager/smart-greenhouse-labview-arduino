#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int ldrPin = A2;
const int suSeviyePin = A3;
const int toprakNemPin = A4;

const int fanPin = 8;
const int pompaPin = 9;
const int ledPin = 10;

unsigned long sonDHTOkuma = 0;
float sicaklik = 0.0;
float nem = 0.0;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10); 
  
  dht.begin();
  
  pinMode(fanPin, OUTPUT);
  pinMode(pompaPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  digitalWrite(fanPin, LOW);
  digitalWrite(pompaPin, LOW);
  digitalWrite(ledPin, LOW);
}

void loop() {
  if (millis() - sonDHTOkuma > 2000) {
    sicaklik = dht.readTemperature();
    nem = dht.readHumidity();
    sonDHTOkuma = millis();
  }
  
  int ldrDegeri = analogRead(ldrPin);
  int suDegeri = analogRead(suSeviyePin);
  int toprakDegeri = analogRead(toprakNemPin);
  
  int toprakYuzde = map(toprakDegeri, 1023, 300, 0, 100);
  if (toprakYuzde < 0) toprakYuzde = 0;
  if (toprakYuzde > 100) toprakYuzde = 100;

  Serial.print(sicaklik);
  Serial.print(",");
  Serial.print(nem);
  Serial.print(",");
  Serial.print(ldrDegeri);
  Serial.print(",");
  Serial.print(suDegeri);
  Serial.print(",");
  Serial.println(toprakYuzde);

  if (Serial.available() >= 3) {
    char fanEmri = Serial.read();
    char pompaEmri = Serial.read();
    char isikEmri = Serial.read();
    
    while(Serial.available() > 0 && (Serial.peek() == '\n' || Serial.peek() == '\r')) {
      Serial.read();
    }

    digitalWrite(fanPin, (fanEmri == '1') ? HIGH : LOW);
    digitalWrite(pompaPin, (pompaEmri == '1') ? HIGH : LOW);
    digitalWrite(ledPin, (isikEmri == '1') ? HIGH : LOW);
  }

  delay(200); 
}