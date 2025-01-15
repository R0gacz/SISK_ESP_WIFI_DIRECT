#include <WiFi.h>
const int buttonPin = 0; 

WiFiClient client;

const char* ssid = NULL;//TODO 
const char* password = NULL;//TODO  
IPAddress hostIp = nullptr;//TODO
int port= NULL;//TODO

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

  Serial.println("\nConnected!");

  Serial.print("Client IP Address: ");
  Serial.println(WiFi.localIP());
  
  if (client.connect(hostIp, port)) {
    Serial.println("Connected to Server!");
    client.println("Hello from Client"); //sending message to sever
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

    //Click BOOT button to send message xD
    if (LOW == digitalRead(buttonPin)) {
      //TODO send your own message to server
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