#include <WiFi.h>

WiFiClient client;

const char* ssid = "ESP32_P2P_SERVER"; // Server SSID
const char* password = "sisk2025"; //

IPAddress hostIp;
int port;

const int buttonPin = 0;  // GPIO pin where the button is connected

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.println("Starting Wi-Fi Direct (P2P) Client...");
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to Server...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  IPAddress hostIp = WiFi.gatewayIP();
  port = 1234;

  Serial.println("\nConnected!");

  Serial.print("Client IP Address:");
  Serial.println(WiFi.localIP());
  
  if (client.connect(hostIp, port)) {
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

    if (LOW == digitalRead(buttonPin)) {
      client.println("Ticks: " + String(millis()));
      delay(1000);
    }
  } else {
    Serial.println("Disconnected from server. Reconnecting...");
    if (client.connect(hostIp, port)) {
      Serial.println("Reconnected to Server!");
      client.println("Hello again from Client");
    }
  }
}