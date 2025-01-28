#include <ESP8266WiFi.h> // Library untuk mengontrol modul ESP8266 dengan WiFi

// Nama SSID dan password WiFi yang akan digunakan untuk koneksi
const char* ssid = "Galaxy A05";    
const char* password = "12345678";  

WiFiServer server(80);  // Membuat server pada port 80 (HTTP)

const int led1Pin = D1;  // Pin untuk LED 1
const int led2Pin = D2;  // Pin untuk LED 2

bool flashMode = false;  // Variabel untuk mengontrol mode kelap-kelip

/**
 * Fungsi setup dijalankan sekali saat perangkat dinyalakan.
 * Digunakan untuk inisialisasi pin, koneksi WiFi, dan server.
 */
void setup() {
  Serial.begin(115200); // Memulai komunikasi serial untuk debugging

  // Inisialisasi pin LED sebagai output
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  digitalWrite(led1Pin, LOW); // Memastikan LED 1 mati saat mulai
  digitalWrite(led2Pin, LOW); // Memastikan LED 2 mati saat mulai

  // Menyambungkan perangkat ke jaringan WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  // Menunggu hingga perangkat berhasil terhubung ke WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Menampilkan informasi koneksi WiFi
  Serial.println("\nWiFi connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin(); // Memulai server
}

/**
 * Fungsi loop dijalankan terus-menerus untuk menangani permintaan HTTP.
 */
void loop() {
  WiFiClient client = server.available(); // Menunggu koneksi dari klien
  if (!client) return; // Jika tidak ada klien, keluar dari fungsi

  // Menunggu hingga klien mengirim data
  while (!client.available()) {
    delay(1);
  }

  // Membaca permintaan dari klien
  String request = client.readStringUntil('\r');
  Serial.println(request); // Menampilkan permintaan di Serial Monitor
  client.flush(); // Menghapus buffer klien

  // Mengontrol LED berdasarkan permintaan klien
  if (request.indexOf("/LED1=ON") != -1) {
    flashMode = false; // Mematikan mode kelap-kelip
    digitalWrite(led1Pin, HIGH); // Menyalakan LED 1
    delay(1000); 
    digitalWrite(led1Pin, LOW); // Mematikan LED 1 setelah 1 detik
  } else if (request.indexOf("/LED1=OFF") != -1) {
    flashMode = false; // Mematikan mode kelap-kelip
    digitalWrite(led1Pin, LOW); // Mematikan LED 1
  }

  if (request.indexOf("/LED2=ON") != -1) {
    flashMode = false; // Mematikan mode kelap-kelip
    digitalWrite(led2Pin, HIGH); // Menyalakan LED 2
    delay(1000); 
    digitalWrite(led2Pin, LOW); // Mematikan LED 2 setelah 1 detik
  } else if (request.indexOf("/LED2=OFF") != -1) {
    flashMode = false; // Mematikan mode kelap-kelip
    digitalWrite(led2Pin, LOW); // Mematikan LED 2
  }

  if (request.indexOf("/LEDALL=ON") != -1) {
    flashMode = false; // Mematikan mode kelap-kelip
    digitalWrite(led1Pin, HIGH); // Menyalakan LED 1
    digitalWrite(led2Pin, HIGH); // Menyalakan LED 2
  } else if (request.indexOf("/LEDALL=OFF") != -1) {
    flashMode = false; // Mematikan mode kelap-kelip
    digitalWrite(led1Pin, LOW); // Mematikan LED 1
    digitalWrite(led2Pin, LOW); // Mematikan LED 2
  }

  if (request.indexOf("/LEDFLASH=ON") != -1) {
    flashMode = true; // Mengaktifkan mode kelap-kelip
  } else if (request.indexOf("/LEDFLASH=OFF") != -1) {
    flashMode = false; // Mematikan mode kelap-kelip
    digitalWrite(led1Pin, LOW); // Memastikan LED 1 mati
    digitalWrite(led2Pin, LOW); // Memastikan LED 2 mati
  }

  // Mode kelap-kelip: LED bergantian menyala dan mati
  if (flashMode) {
    digitalWrite(led1Pin, HIGH); 
    digitalWrite(led2Pin, LOW); 
    delay(500);
    digitalWrite(led1Pin, LOW); 
    digitalWrite(led2Pin, HIGH); 
    delay(500);
  }

  // Mengirimkan respon HTML ke klien
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<style>");
  client.println("body { font-family: Arial; text-align: center; margin-top: 50px; }");
  client.println("a { text-decoration: none; display: inline-block; margin: 10px; padding: 10px 20px; color: white; background-color: blue; border-radius: 5px; }");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<h1>Kontrol LED</h1>");
  client.println("<a href=\"/LED1=ON\">Hidupkan LED 1</a>");
  client.println("<a href=\"/LED1=OFF\">Matikan LED 1</a>");
  client.println("<br>");
  client.println("<a href=\"/LED2=ON\">Hidupkan LED 2</a>");
  client.println("<a href=\"/LED2=OFF\">Matikan LED 2</a>");
  client.println("<br>");
  client.println("<a href=\"/LEDALL=ON\">Hidupkan Kedua LED</a>");
  client.println("<a href=\"/LEDALL=OFF\">Matikan Kedua LED</a>");
  client.println("<br>");
  client.println("<a href=\"/LEDFLASH=ON\">Nyalakan Mode Kelap-Kelip</a>");
  client.println("<a href=\"/LEDFLASH=OFF\">Matikan Mode Kelap-Kelip</a>");
  client.println("</body>");
  client.println("</html>");
}
