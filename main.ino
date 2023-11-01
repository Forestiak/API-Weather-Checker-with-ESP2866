#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "";  //wifi ssid
const char* password = "";  //wifi password
const char* apiKey = "";  //can be found on openweathermap.org after signing up
const int buzzerPin = D1;
const float thresholdTemp = 15.0;
String url = "http://api.openweathermap.org/data/2.5/weather?id=  CITY_ID_YOU_WANT_TO_CHECK  &appid=" + String(apiKey) + "&units=metric";

void setup() {
  Serial.begin(9600);
  //pinMode(buzzerPin, OUTPUT);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;
    http.begin(client, url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      float temperature = parseTemperature(payload);

      if (temperature < thresholdTemp) {
        digitalWrite(buzzerPin, HIGH);
        Serial.println(temperature);
        delay(1000);  // Buzzer ON for 1 second
        digitalWrite(buzzerPin, LOW);
      }
      else{
        Serial.println("valid");
      }
    }
    http.end();
  }
  delay(60000);  // Check every minute
}

float parseTemperature(String payload) {
  StaticJsonDocument<512> doc;
  deserializeJson(doc, payload);
  float temperature = doc["main"]["temp"];
  return temperature;
}
