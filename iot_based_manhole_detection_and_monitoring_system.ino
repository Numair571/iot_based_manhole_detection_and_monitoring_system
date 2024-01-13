#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

 char *ssid = "numair";
 char *password = "123456789";
 int YourChannelNumber = 12134014;
 char *apiKey = "";
 char *botToken = "";
 char *telegramChatId = "1635045125";

 int trigPin = D5;
 int echoPin = D6;
 int ledPin = D2; 

WiFiClient client;
UniversalTelegramBot bot(botToken, client);

void setup()
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  connectToWiFi();
}

void loop()
{
  long duration, distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(6);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance < 20)
  {
    Serial.println("Manhole Open");
    sendDataToThingSpeak("Manhole detected");
    sendTelegramMessage("Manhole detected");
    digitalWrite(ledPin, HIGH); 
  }
  else
  {
    Serial.println("Manhole Closed");
    sendDataToThingSpeak("Manhole Closed");
    sendTelegramMessage("Manhole closed.");
    digitalWrite(ledPin, LOW); 
  }

  delay(5000);
}

void sendDataToThingSpeak(String data)
{
  ThingSpeak.begin(client);
  ThingSpeak.writeField(YourChannelNumber, 1, data, apiKey);
}

void sendTelegramMessage(String message)
{
  bot.sendMessage(telegramChatId, message, "");
}

void connectToWiFi()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}
