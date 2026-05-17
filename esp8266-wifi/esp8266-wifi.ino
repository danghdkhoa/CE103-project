#include <ESP8266WiFi.h>              // Use <ESP8266WiFi.h> for NodeMCU
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "ssid";
const char* password = "password";

// Use @BotFather to get this
#define BOT_TOKEN "BOT_TOKEN"
// Use @IDBot to get this
#define CHAT_ID "CHAT_ID"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

void setup() {
  Serial.begin(9600);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  client.setInsecure(); // Skip certificate validation for simplicity

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");

}

void loop() {
  // Add logic here to send messages based on triggers (e.g., button press)
  if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    message.trim();
    if (message == "ALARM") {
      Serial.println("Message sent.!");
      bot.sendMessage(CHAT_ID, "CANH BAO CO NGUOI");
    }
  }
}
