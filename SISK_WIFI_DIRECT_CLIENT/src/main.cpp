#include <WiFi.h>

WiFiClient client;

const char* ssid = //TODO 
const char* host = //TODO     // Default AP IP address for ESP32
const int port = 1234;                 // Same port as the server

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Wi-Fi Direct (P2P) Client...");
  
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid);
  
  Serial.print("Connecting to Server...");
  while (/*TODO check if wifi status != connected*/) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");

  Serial.print("Client IP Address: ");
  //TODO print local IP to serial
  
  if (client.connect(host, port)) {
    Serial.println("Connected to Server!");
    client.println("Hello from Client");
  } else {
    Serial.println("Connection failed.");
  }
}

void loop() {
  if (/*TODO chceck if client is connected*/) {
    while (client.available()) {
      String received = client.readStringUntil('\n');
      Serial.print("Received: ");
      Serial.println(received);
    }

    // Send a message periodically
    static unsigned long lastSendTime = 0;
    if (millis() - lastSendTime > 2000) {
      lastSendTime = millis();
      //TODO send message to WIFI server
    }
  } else {
    Serial.println("Disconnected from server. Reconnecting...");
    if (client.connect(host, port)) {
      Serial.println("Reconnected to Server!");
      client.println("Hello again from Client");
    }
  }
}