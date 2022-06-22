#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SimpleDHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
#define RED_LED D5 //led warna merah
#define GREEN_LED D6 //led warna hijau
#define BLUE_LED D8 //led warnah biru
const char *ssid = "POCO X3 Pro";//silakan disesuaikan sendiri
const char *password = "berkahibu";//silakan disesuaikan sendiri
const char *mqtt_server = "broker.hivemq.com";
WiFiClient espClient;
PubSubClient client(espClient);
SimpleDHT11 dht11(D7);
long now = millis();
long lastMeasure = 0;
void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}
void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client"))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void setup()
{
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.clear();
  lcd.home();
  Serial.begin(115200);
  Serial.println("Mqtt Node-RED");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(RED_LED, OUTPUT); //atur pin-pin digital sebagai output
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
}
void loop()
{
  now = millis();
  if (now - lastMeasure > 5000)
  {
    lastMeasure = now;
    int err = SimpleDHTErrSuccess;
    byte temperature = 0;
    byte humidity = 0;
    if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
    {
      Serial.print("Pembacaan DHT11 gagal, err=");
      Serial.println(err);
      delay(1000);
      return;
    }
    static char temperatureTemp[7];
    dtostrf(temperature, 4, 2, temperatureTemp);
    lcd.home();
    lcd.setCursor(0, 0);
    lcd.print("-----");
    lcd.print(temperatureTemp);
    lcd.print("C-----");
    if  ((int)temperature > 30) {
      lcd.home();
      lcd.setCursor(0, 1);
      lcd.println("--Burned House--");
      digitalWrite(BLUE_LED, HIGH);
      delay(50);
      digitalWrite(BLUE_LED, LOW);
      delay(50);
      digitalWrite(BLUE_LED, HIGH);
      delay(50);
      digitalWrite(BLUE_LED, LOW);
      delay(50);
      digitalWrite(BLUE_LED, HIGH);
      delay(50);
      digitalWrite(BLUE_LED, LOW);
      delay(50);
      digitalWrite(BLUE_LED, HIGH);
      delay(50);
      digitalWrite(BLUE_LED, LOW);
      delay(50);
      digitalWrite(BLUE_LED, HIGH);
      delay(50);
      digitalWrite(BLUE_LED, LOW);
      delay(50);
    } else {
      lcd.home();
      lcd.setCursor(0, 1);
      lcd.println("---SuhuNormal----");
    }
    if (!client.connected())
    {
      reconnect();
    }
    if (!client.loop())
    {
      client.connect("ESP8266Client");
    }
    client.publish("1941720180/ilhamibrahim", temperatureTemp);
  }
}
