#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

WiFiServer server(1234); // Port 1234 for communication

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Wi-Fi Direct (P2P) Server...");
  
  // Start Wi-Fi in P2P mode
  WiFi.mode(WIFI_MODE_AP);
  WiFi.softAP("ESP32_P2P_SERVER");

  Serial.print("Server IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
  Serial.println("Server started, waiting for clients...");
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("Client connected!");

    while (client.connected()) {
      if (client.available()) {
        String received = client.readStringUntil('\n');
        Serial.print("Received: ");
        Serial.println(received);
        
        client.print("Echo from Server: ");
        client.println(received);
      }
    }

    Serial.println("Client disconnected.");
    client.stop();
  }
}