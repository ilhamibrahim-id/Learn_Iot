#include <Arduino.h>
#include <SimpleDHT.h>
#define sensorLDR A0
#define pinDHT 7
#define RED_LED D5   // led warna merah
#define GREEN_LED D6 // led warna hijau
#define BLUE_LED D7  // led warnah biru // SD3 pin signal sensor DHT

byte temperature = 0;
byte humidity = 0;

SimpleDHT11 dht11(D1);
int nilaiSensor;

void setup()
{
  Serial.begin(115200);
  Serial.println("Contoh Penggunaan Sensor LDR");
  delay(3000);
  pinMode(RED_LED, OUTPUT); //atur pin-pin digital sebagai output
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
}

void loop()
{
  nilaiSensor = analogRead(sensorLDR);
  int err = SimpleDHTErrSuccess;
  Serial.print("Nilai Sensor : ");
  Serial.println(nilaiSensor);
  Serial.print("CEK SUHU: ");
  Serial.print((int)temperature);
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
  {
    Serial.print("Pembacaan DHT11 gagal, err=");
    Serial.println(err);
    delay(1000);
    return;
  }
  Serial.print(" *C, ");
  Serial.print((double)temperature + 273.15);

  Serial.print(" *K, ");
  Serial.print((int)temperature * 0.8);

  Serial.print(" *R, ");
  Serial.print((int)humidity);
  Serial.println(" H");
  delay(1000);
  if ((int)nilaiSensor > 512 && (int)temperature < 24) {
    digitalWrite(BLUE_LED, HIGH);
    digitalWrite(BLUE_LED, LOW);
    delay(100);
  } else if ((int)nilaiSensor < 512  && (int)temperature < 32) {
     digitalWrite(RED_LED, HIGH);
  } else if((int)temperature >= 24 && (int)nilaiSensor < 512) {
    digitalWrite(RED_LED, HIGH);
  } else {
    digitalWrite(RED_LED, LOW);
  }
}