// Mengimpor library ESP8266WiFi untuk menghubungkan perangkat ke jaringan WiFi
#include <ESP8266WiFi.h>

// Mengimpor library WiFiClientSecure untuk menghandle koneksi aman (HTTPS) ke server
#include <WiFiClientSecure.h>

// Mengimpor library UniversalTelegramBot untuk berinteraksi dengan Bot Telegram
#include <UniversalTelegramBot.h>

// Konfigurasi WiFi
const char* ssid = "Galaxy A05"; // Nama jaringan WiFi yang akan digunakan
const char* password = "12345678"; // Password WiFi

// Konfigurasi Bot Telegram
#define BOT_TOKEN "7615701598:AAGqTOqqfxaJkAlAtvSFegM60HMWK8aoLEM" // Token bot Telegram yang digunakan untuk berkomunikasi dengan bot

// Membuat objek WiFiClientSecure untuk menghandle koneksi aman (HTTPS)
WiFiClientSecure client;

// Membuat objek UniversalTelegramBot dengan token bot dan client yang telah didefinisikan
UniversalTelegramBot bot(BOT_TOKEN, client);

// Pin LED
const int led1Pin = D1; // Pin D1 pada ESP8266 untuk LED 1
const int led2Pin = D2; // Pin D2 pada ESP8266 untuk LED 2
bool blinkMode = false; // Variabel untuk mengontrol mode berkedip
bool led1State = false; // Variabel untuk menyimpan status LED 1 (ON/OFF)
bool led2State = false; // Variabel untuk menyimpan status LED 2 (ON/OFF)

void setup() {
  // Memulai komunikasi serial dengan baud rate 115200 untuk debugging
  Serial.begin(115200);
  
  // Mengatur pin LED sebagai output dan memastikan LED dalam keadaan mati saat awal
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
  
  // Koneksi ke WiFi
  Serial.println("Connecting to WiFi...");

  // Memulai koneksi ke jaringan WiFi dengan SSID dan password yang telah didefinisikan
  WiFi.begin(ssid, password);

  // Menunggu hingga ESP8266 terhubung ke WiFi
  // Loop akan terus berjalan sampai status WiFi terhubung (WL_CONNECTED)
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  
  // Menghindari masalah sertifikat SSL dengan mengabaikan verifikasi sertifikat
  client.setInsecure();
}

void loop() {
  // Mengambil jumlah pesan baru yang diterima oleh bot
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  // Jika ada pesan baru, maka akan diproses dalam loop while
  while (numNewMessages) {
    // Memproses pesan baru dengan memanggil fungsi handleNewMessages
    handleNewMessages(numNewMessages);
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }

  // Mode berkedip
  if (blinkMode) {
    // Jika mode berkedip aktif, LED akan berkedip dengan interval 200ms
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, HIGH);
    delay(200);
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
    delay(200);
  } else {
    // Jika mode berkedip tidak aktif, LED akan mengikuti status yang telah ditentukan (led1State dan led2State)
    if (led1State) {
      digitalWrite(led1Pin, HIGH);
    } else {
      digitalWrite(led1Pin, LOW);
    }

    if (led2State) {
      digitalWrite(led2Pin, HIGH);
    } else {
      digitalWrite(led2Pin, LOW);
    }
  }
}

// Fungsi untuk menangani pesan dari Telegram
void handleNewMessages(int numNewMessages) {
  // Loop untuk memproses setiap pesan baru yang diterima
  for (int i = 0; i < numNewMessages; i++) {
    // Mengambil chat_id dari pengirim pesan
    String chat_id = bot.messages[i].chat_id;

    // Mengambil teks pesan yang dikirim oleh pengguna
    String text = bot.messages[i].text;
    
    Serial.println("Pesan diterima dari chat_id " + chat_id + ": " + text);

    // Menangani perintah yang diterima dari pengguna
    if (text == "/start") {
      bot.sendMessage(chat_id, "Selamat Datang di Bot Kontrol LED!\n"
                               "Gunakan perintah berikut:\n"
                               "/led1_on - Untuk menghidupkan LED 1\n"
                               "/led1_off - Untuk mematikan LED 1\n"
                               "/led2_on - Untuk menghidupkan LED 2\n"
                               "/led2_off - Untuk mematikan LED 2\n"
                               "/ledall_on - Untuk menyalakan semua LED\n"
                               "/ledall_off - Untuk mematikan semua LED\n"
                               "/blink_on - Untuk mengaktifkan mode berkedip\n"
                               "/blink_off - Untuk mematikan mode berkedip", "");
    } 
    else if (text == "/led1_on") {
      blinkMode = false;
      led1State = true;
      bot.sendMessage(chat_id, "LED 1 dinyalakan.", "");
    } 
    else if (text == "/led1_off") {
      blinkMode = false;
      led1State = false;
      bot.sendMessage(chat_id, "LED 1 dimatikan.", "");
    } 
    else if (text == "/led2_on") {
      blinkMode = false;
      led2State = true;
      bot.sendMessage(chat_id, "LED 2 dinyalakan.", "");
    } 
    else if (text == "/led2_off") {
      blinkMode = false;
      led2State = false;
      bot.sendMessage(chat_id, "LED 2 dimatikan.", "");
    } 
    else if (text == "/ledall_on") {
      blinkMode = false;
      led1State = true;
      led2State = true;
      bot.sendMessage(chat_id, "Kedua LED dinyalakan.", "");
    } 
    else if (text == "/ledall_off") {
      blinkMode = false;
      led1State = false;
      led2State = false;
      bot.sendMessage(chat_id, "Kedua LED dimatikan.", "");
    } 
    else if (text == "/blink_on") {
      blinkMode = true;
      bot.sendMessage(chat_id, "Mode berkedip diaktifkan.", "");
    } 
    else if (text == "/blink_off") {
      blinkMode = false;
      led1State = false;
      led2State = false;
      bot.sendMessage(chat_id, "Mode berkedip dimatikan.", "");
    } 
    else {
      bot.sendMessage(chat_id, "Perintah tidak dikenali.\n"
                               "Gunakan:\n"
                               "/start untuk melihat daftar perintah", "");
    }
  }
}