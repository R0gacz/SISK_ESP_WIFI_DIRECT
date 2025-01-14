#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <vector>

// Dane sieci Wi-Fi
const char* ssid = "Realme(Jakub)";
const char* password = "tempSisk";

// Adres i port głównego routera/serwera
const char* remote_host = "192.168.128.31"; // Adres serwera docelowego
const int remote_port = 5678;             // Port serwera docelowego

WiFiServer server(1234); // Lokalny serwer
WiFiClient forwardClient; // Klient do komunikacji z routerem
std::vector<WiFiClient> clients; // Lista lokalnych klientów

void setup() {
  Serial.begin(115200);

  // Tryb AP + STA
  WiFi.mode(WIFI_MODE_APSTA);
  WiFi.softAP("ESP32_P2P_SERVER");
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
}

void loop() {
  // Sprawdź, czy nowy klient próbuje się połączyć
  WiFiClient newClient = server.available();
  if (newClient) {
    Serial.println("New client connected!");
    clients.push_back(newClient); // Dodaj klienta do listy
  }

  // Iteruj przez listę aktywnych klientów
  for (size_t i = 0; i < clients.size(); i++) {
    WiFiClient client = clients[i];
    if (client.connected()) {
      if (client.available()) {
        String received = client.readStringUntil('\n');
        Serial.print("Received from client ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(received);

        // Echo do klienta
        client.print("Echo from Server: ");
        client.println(received);

        // Wysyłanie danych do głównego routera
        if (!forwardClient.connected()) {
          Serial.println("Connecting to remote server...");
          if (forwardClient.connect(remote_host, remote_port)) {
            Serial.println("Connected to remote server.");
          } else {
            Serial.println("Failed to connect to remote server.");
          }
        }
        if (forwardClient.connected()) {
          forwardClient.println(received);
          Serial.println("Data forwarded to remote server.");
        }
      }
    } else {
      // Usuń rozłączonych klientów
      Serial.print("Client ");
      Serial.print(i);
      Serial.println(" disconnected.");
      clients.erase(clients.begin() + i);
      i--; // Dostosuj indeks po usunięciu klienta
    }
  }
}


// #include <WiFi.h>
// #include <WiFiClient.h>
// #include <WiFiAP.h>
// #include <vector>

// WiFiServer server(1234); // Port 1234 for communication
// std::vector<WiFiClient> clients;

// void setup() {
//   Serial.begin(115200);
//   Serial.println("Starting Wi-Fi Direct (P2P) Server...");
  
//   WiFi.mode(WIFI_MODE_AP);
//   WiFi.softAP("ESP32_P2P_SERVER");

//   Serial.print("Server IP Address: ");
//   Serial.println(WiFi.softAPIP());

//   server.begin();
//   Serial.println("Server started, waiting for clients...");
// }

// void loop() {
//   // Sprawdź, czy nowy klient próbuje się połączyć
//   WiFiClient newClient = server.available();
//   if (newClient) {
//     Serial.println("New client connected!");
//     clients.push_back(newClient); // Dodaj klienta do listy
//   }

//   // Iteruj przez listę aktywnych klientów
//   for (size_t i = 0; i < clients.size(); i++) {
//     WiFiClient client = clients[i];
//     if (client.connected()) {
//       if (client.available()) {
//         String received = client.readStringUntil('\n');
//         Serial.print("Received from client ");
//         Serial.print(client.remoteIP());
//         Serial.print(": ");
//         Serial.println(received);

//         client.print("Echo from Server: ");
//         client.println(received);
//       }
//     } else {
//       // Usuń rozłączonych klientów
//       Serial.print("Client ");
//       Serial.print(i);
//       Serial.println(" disconnected.");
//       clients.erase(clients.begin() + i);
//       i--; // Dostosuj indeks po usunięciu klienta
//     }
//   }
// }