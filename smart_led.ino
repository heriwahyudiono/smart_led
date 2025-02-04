#include <ESP8266WiFi.h> // Library untuk mengontrol modul ESP8266 dengan WiFi

const char* ssid = "Galaxy A05";    
const char* password = "12345678";  

WiFiServer server(80);  // Membuat server pada port 80 (HTTP)

const int led1Pin = D1;  // Pin untuk LED 1
const int led2Pin = D2;  // Pin untuk LED 2

bool blinkMode = false;   // Variabel untuk mengontrol mode berkedip
bool led1State = false;   // Status LED 1
bool led2State = false;   // Status LED 2

void setup() {
  Serial.begin(115200); 

  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available(); 
  if (client) {
    while (!client.available()) {
      delay(1);
    }

    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Mengontrol LED berdasarkan permintaan klien
    if (request.indexOf("/LED1=ON") != -1) {
      blinkMode = false;
      led1State = true; 
    } 
    if (request.indexOf("/LED1=OFF") != -1) {
      blinkMode = false;
      led1State = false;
    }

    if (request.indexOf("/LED2=ON") != -1) {
      blinkMode = false;
      led2State = true;
    } 
    if (request.indexOf("/LED2=OFF") != -1) {
      blinkMode = false;
      led2State = false;
    }

    if (request.indexOf("/LEDALL=ON") != -1) {
      blinkMode = false;
      led1State = true;
      led2State = true;
    } 
    if (request.indexOf("/LEDALL=OFF") != -1) {
      blinkMode = false;
      led1State = false;
      led2State = false;
    }

    if (request.indexOf("/LEDBLINK=ON") != -1) {
      blinkMode = true;
    } 
    if (request.indexOf("/LEDBLINK=OFF") != -1) {
      blinkMode = false;
      led1State = false;
      led2State = false;
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
    client.println("<a href=\"/LEDBLINK=ON\">Nyalakan Mode Berkedip</a>");
    client.println("<a href=\"/LEDBLINK=OFF\">Matikan Mode Berkedip</a>");
    client.println("</body>");
    client.println("</html>");
  }

  // Mode berkedip: LED berkedip bersamaan menggunakan delay(500)
  if (blinkMode) {
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, HIGH);
    delay(500);
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
    delay(500);
  } else {
    // Mengatur status LED menggunakan if-else
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
