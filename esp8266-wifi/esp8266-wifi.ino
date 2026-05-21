#include <ESP8266WiFi.h>              // Use <ESP8266WiFi.h> for NodeMCU
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "ssid";
const char* password = "password";

#define BOT_TOKEN "BOT_TOKEN"
#define CHAT_ID "CHAT_ID"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

const unsigned long BOT_MTBS = 1000;
unsigned long bot_lasttime;
int status = 1;

void handleNewMessages(int numNewMessages)
{
  // Serial.print("handleNewMessages ");
  // Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";

    if (text == "/PIRon")
    {
      status = 1;
      Serial.println("PIRon");
      bot.sendMessage(chat_id, "Đã bật PIR.", "");
    }

    if (text == "/PIRoff")
    {
      status = 0;
      Serial.println("PIRoff");
      bot.sendMessage(chat_id, "Đã tắt PIR.", "");
    }

    if (text == "/status")
    {
      if (status)
      {
        bot.sendMessage(chat_id, "PIR đang bật.", "");
      }
      else
      {
        bot.sendMessage(chat_id, "PIR đang tắt.", "");
      }
    }

    if (text == "/start")
    {
      String welcome = "Xin Chao, " + from_name + ".\n";
      welcome += "Chọn các lệnh dưới đây.\n";
      welcome += "/PIRon : Bật PIR.\n";
      welcome += "/PIRoff : Tắt PIR.\n";
      welcome += "/status : Trạng thái hiện tại của PIR.\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}


void setup() {
  Serial.begin(9600);
  
  configTime(7 * 3600, 0, "pool.ntp.org");      // get UTC time via NTP
  client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  //client.setInsecure(); // Skip certificate validation for simplicity

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
    if (status == 1) {
      if (message == "ALARM") {
        Serial.println("Message sent.!");
        bot.sendMessage(CHAT_ID, "CẢNH BÁO: CÓ NGƯỜI.");
      }
    }
  }

  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      // Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}