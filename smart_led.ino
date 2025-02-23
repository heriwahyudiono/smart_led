#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

const char* ssid = "Galaxy A05";
const char* password = "12345678";

#define BOT_TOKEN "7615701598:AAGqTOqqfxaJkAlAtvSFegM60HMWK8aoLEM"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

const int ledPin = D1;
bool blinkMode = false;
bool ledState = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  client.setInsecure();
}

void loop() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  while (numNewMessages) {
    handleNewMessages(numNewMessages);
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }

  if (blinkMode) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  } else {
    digitalWrite(ledPin, ledState ? HIGH : LOW);
  }
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    Serial.println("Pesan diterima dari chat_id " + chat_id + ": " + text);

    if (text == "/start") {
      bot.sendMessage(chat_id, "Selamat Datang di Bot Kontrol LED!\n"
                               "Gunakan perintah berikut:\n"
                               "/led_on - Untuk menghidupkan LED\n"
                               "/led_off - Untuk mematikan LED\n"
                               "/led_blink - Untuk menghidupkan lampu kelap-kelip", "");
    } 
    else if (text == "/led_on") {
      blinkMode = false;
      ledState = true;
      bot.sendMessage(chat_id, "LED dinyalakan", "");
    } 
    else if (text == "/led_off") {
      blinkMode = false;
      ledState = false;
      bot.sendMessage(chat_id, "LED dimatikan", "");
    } 
    else if (text == "/led_blink") {
      blinkMode = true;
      bot.sendMessage(chat_id, "Lampu kelap-kelip di aktifkan", "");
    } 
    else {
      bot.sendMessage(chat_id, "Perintah tidak dikenali.\n"
                               "Gunakan:\n"
                               "/start untuk melihat daftar perintah", "");
    }
  }
}
