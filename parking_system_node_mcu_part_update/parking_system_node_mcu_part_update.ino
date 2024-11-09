#include <ESP8266WiFi.h>

const char* ssid = "YoMada";        // Your WiFi SSID
const char* password = "12345678"; // Your WiFi password

WiFiServer server(80); // Start a web server on port 80

void setup() {
    Serial.begin(115200);  // Start the Serial communication with the PC
    connectToWiFi();
    sendIPAddress();
    server.begin();
}

void loop() {
    WiFiClient client = server.available(); // Check for incoming clients
    if (client) {
        handleClient(client);
    }
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
}

void sendIPAddress() {
    String phpUrl = "http://192.168.4.4/procedural/parking_system/lib/php/bridge/iot_connection.php?ip=";
    phpUrl += WiFi.localIP().toString();
    WiFiClient client;

    if (client.connect("192.168.4.4", 80)) {
        client.print(String("GET ") + phpUrl + " HTTP/1.1\r\n" +
                     "Host: 192.168.4.4\r\n" +
                     "Connection: close\r\n\r\n");
        Serial.println("IP address sent to PHP server.");
    }
}

void handleClient(WiFiClient client) {
    String currentLine = "";
    while (client.connected()) {
        if (client.available()) {
            char c = client.read();
            currentLine += c;

            if (c == '\n') {
                if (currentLine.startsWith("GET")) {
                    int start = currentLine.indexOf("data=") + 5;
                    int end = currentLine.indexOf(" ", start);
                    String data = currentLine.substring(start, end);
                    Serial.println(data); // Print data to Serial Monitor and Pass the received data to Arduino


                }
                break; // Break out of the while loop
            }
        }
    }
    client.stop(); // Close the connection
}
