#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "YOUR WIFI NAME";
const char* password = "YOUR WIFI PASSWORD";

// Initialize Telegram BOT
#define BOTtoken "YOUR TELEGRAM TOKEN"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "YOUR CHAT ID"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int power_pin = 2;
const int reset_pin = 0;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;
    if (text == "/turnon")  
          {   
      digitalWrite(power_pin,LOW);
      delay(400);
      digitalWrite(power_pin,HIGH);
      bot.sendMessage(CHAT_ID , "Turning on computer...");   
    }   
    else if (text == "/reboot")  
          {   
      digitalWrite(reset_pin,LOW);
      delay(400);
      digitalWrite(reset_pin,HIGH); 
      bot.sendMessage(CHAT_ID, "Rebooting computer...");   
    }   
    else  
        {   
      bot.sendMessage(CHAT_ID, "Available commands: /turnon /reboot");   
    }
   
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    client.setInsecure();
  #endif

  pinMode(power_pin,OUTPUT);
  pinMode(reset_pin,OUTPUT);
  digitalWrite(power_pin,HIGH);
  digitalWrite(reset_pin,HIGH);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  
  //Your bot will send you a message in Telegram when it starts
  //bot.sendMessage(CHAT_ID, "Bot available");   

}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
