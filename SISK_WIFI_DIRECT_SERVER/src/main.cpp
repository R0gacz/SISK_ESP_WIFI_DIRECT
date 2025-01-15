#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <vector>

// Dane sieci Wi-Fi
// Adres i port głównego routera/serwera
const char* ssid = "TempNetwork";
const char* password = "sisk2025";
const int udpPort = 5678;                  // Port UDP serwera

// Konfiguracja servera Wifi P2P
IPAddress apIP(10, 0, 0, 1);
IPAddress apGateway(10, 0, 0, 1);
IPAddress apSubnet(255, 255, 255, 0);
WiFiUDP udp;
WiFiServer server(1234); // Lokalny serwer
std::vector<WiFiClient> clients; // Lista lokalnych klientów

void setup() {
  Serial.begin(115200);

  // Tryb AP + STA
  WiFi.mode(WIFI_MODE_APSTA);
  WiFi.softAPConfig(apIP, apGateway, apSubnet);
  WiFi.softAP("ESP32_P2P_SERVER", password, 1, 1, 8, false);

  WiFi.begin(ssid, password);

  Serial.println("Starting Wi-Fi Direct (P2P) Server...");
  Serial.print("Server IP Address (AP): ");
  Serial.println(WiFi.softAPIP());

  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi.");
  Serial.print("STA IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Local server started.");
  udp.beginPacket("192.168.0.255", udpPort);
  udp.println("Local server started.");
  udp.endPacket();
}

void loop() {
  // Sprawdź, czy nowy klient próbuje się połączyć
  WiFiClient newClient = server.available();
  if (newClient) {
    Serial.println("New client connected!");
    clients.push_back(newClient); // Dodaj klienta do listy
  }


  for (size_t i = 0; i < clients.size(); i++) {
    WiFiClient client = clients[i];
    if (client.connected()) {
      if (client.available()) {
        String received = client.readStringUntil('\n');
        Serial.print("Received from client ");
        Serial.print(client.remoteIP());
        Serial.print(": ");
        Serial.println(received);

        // Echo do klienta
        client.print("Echo from Server: ");
        client.println(received);



        Serial.println("Data forwarded to remote server.");
        udp.beginPacket("192.168.0.255", udpPort);
        udp.print("Recieved data from ");  // Wysłanie wiadomości jako ciąg znaków
        udp.print(client.remoteIP());  // Wysłanie wiadomości jako ciąg znaków
        udp.print(": ");  // Wysłanie wiadomości jako ciąg znaków
        udp.println(received);  // Wysłanie wiadomości jako ciąg znaków
        udp.endPacket();

      }
    } else {
      // Usuń rozłączonych klientów
        udp.beginPacket("192.168.0.255", udpPort);
        udp.print("Client ");
        udp.print(client.remoteIP());
        udp.println(" disconnected.");
        udp.endPacket();

        clients.erase(clients.begin() + i);
        i--; // Dostosuj indeks po usunięciu klienta
    }
  }
}