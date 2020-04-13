#include <WiFi.h>
#include <Wire.h>
#include <ArduinoJson.h> 
 
const char* ssid     = "SSID"; // логин
const char* password = "pass"; // пароль
 
const char * hostDomain = "api.openweathermap.org"; // домен
const int hostPort = 80; // порт
 
String line; 
 
WiFiClient client;
 
 
void setup()
{
    Serial.begin(9600);
    delay(100);
    
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
 
    WiFi.begin(ssid, password); // подключение к точке Wi-Fi
 
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
 
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
 
      
 
}
 
 
void requestURL(const char * host, uint8_t port) // запрос
 
{
  Serial.println("Connecting to domain: " + String(host));
 
  if (!client.connect(host, port))
  {
    Serial.println("connection failed");
    return;
  }
  Serial.println("Connected!");
 
  client.println("GET /data/2.5/weather?id=536203&lang=ru&APPID=XXXXXXXXXXXXX...XXX&units=metric HTTP/1.1");
  client.print("Host: api.openweathermap.org\r\n");
  client.print("Connection: close\r\n\r\n");
 
 
  unsigned long timeout = millis();
  while (client.available() == 0) 
  {
    if (millis() - timeout > 1000) 
    {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  while (client.available()) 
  {
    line = client.readStringUntil('\r');
  
  }
 
  Serial.println();
  Serial.println("closing connection");
  client.stop();
 
}
 
 
void parseJSON(String weather) // парсинг данных
{
  StaticJsonBuffer<3000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(weather);
  if (!root.success())
  {
    Serial.print("ParseObject() failed");
  }
 
  String name = root["name"];      // название города
  Serial.print("name: ");
  Serial.println(name); 
 
  float temp = root["main"]["temp"];  // температура
  Serial.print("temp: ");
  Serial.print(temp);               
  Serial.println(" C");
 
  float temp_min = root["main"]["temp_min"]; // мин. температура
  Serial.print("temp_min: ");
  Serial.print(temp_min);               
  Serial.println(" C");
 
  float temp_max = root["main"]["temp_max"]; // макс. температура
  Serial.print("temp_max: ");
  Serial.print(temp_max);               
  Serial.println(" C");
 
  
  float pressure = root["main"]["pressure"]; // давление
  Serial.print("pressure: ");
  Serial.print(pressure/1.33);  // гПа в мм рт.ст.             
  Serial.println(" mmHg");
 
  
  float humidity = root["main"]["humidity"]; // влажность 
  Serial.print("humidity: ");
  Serial.print(humidity);               
  Serial.println(" %");
}
 
 
 
 
 
void loop()
{
 
    Serial.println();
   
    requestURL(hostDomain, hostPort);
 
    Serial.print(line);
    Serial.println();
    Serial.println("-------------");
    Serial.println();
 
    parseJSON(line);
    delay(30000); // смотрим погоду каждые 30 секунд
    
}
