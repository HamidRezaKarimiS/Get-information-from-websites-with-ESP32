#include <Arduino.h>
#include <WiFi.h>
#include <httpclient.h>
#include <ArduinoJson.h>
#include <wificlientsecure.h>

const char *ssid = "hamid";          // Replace with your WiFi network name
const char *password = "hamid12345"; // Replace with your WiFi network password

const char *host = "https://api.priceto.day/v1/latest/irr/usd";

int _price;
int _high;
int _low;
String _time;

uint8_t button = 0;

void connectToWiFi();
void get_data();
void show_data();

void setup()
{
  pinMode(button, INPUT_PULLUP);
  Serial.begin(115200);
  connectToWiFi();
}

void loop()
{
  if (digitalRead(button) == LOW)
  {
    get_data();
    delay(1000);
  }
}

void connectToWiFi()
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
void get_data()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFiClientSecure Client;
    Client.setInsecure();
    HTTPClient http;
    http.begin(Client, host);
    int httpCode = http.GET();
    if (httpCode > 0)
    {
      String payload = http.getString();
      JsonDocument doc;
      deserializeJson(doc, payload);
      _price = doc["price"].as<int>();
      _high = doc["high"].as<int>();
      _low = doc["low"].as<int>();
      _time = doc["time"].as<String>();
      show_data();
    }
    else
    {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
}
void show_data()
{
  Serial.println("--------------------------------------------");
  Serial.print("Last Time:");
  Serial.println(_time);
  Serial.println("--------------------------------------------");
  Serial.print("Price:");
  Serial.println(_price);
  Serial.print("High:");
  Serial.println(_high);
  Serial.print("Low:");
  Serial.println(_low);
}