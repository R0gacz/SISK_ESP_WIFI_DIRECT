#include <WiFi.h>

WiFiClient client;

const char* ssid = "ESP32_P2P_SERVER"; // Server SSID
const char* host = "192.168.4.1";      // Default AP IP address for ESP32
const int port = 1234;                 // Same port as the server

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Wi-Fi Direct (P2P) Client...");
  
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid);
  
  Serial.print("Connecting to Server...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");

  Serial.print("Client IP Address: ");
  Serial.println(WiFi.localIP());
  
  if (client.connect(host, port)) {
    Serial.println("Connected to Server!");
    client.println("Hello from Client");
  } else {
    Serial.println("Connection failed.");
  }
}

void loop() {
  if (client.connected()) {
    while (client.available()) {
      String received = client.readStringUntil('\n');
      Serial.print("Received: ");
      Serial.println(received);
    }

    // Send a message periodically
    static unsigned long lastSendTime = 0;
    if (millis() - lastSendTime > 2000) {
      lastSendTime = millis();
      client.println("Message from Client: " + String(millis()));
    }
  } else {
    Serial.println("Disconnected from server. Reconnecting...");
    if (client.connect(host, port)) {
      Serial.println("Reconnected to Server!");
      client.println("Hello again from Client");
    }
  }
}