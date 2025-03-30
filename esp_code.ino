#include <WiFi.h>
#include <HTTPClient.h>
#include <BluetoothSerial.h>

const char* ssid = "YOUR_SSID";  // Change to your WiFi SSID
const char* password = "YOUR_PASSWORD";  // Change to your WiFi password
const char* serverUrl = "http://SERVER_IP:5000/update";  // Change to your Python server IP

BluetoothSerial SerialBT;

void setup() {
    Serial.begin(115200);
    SerialBT.begin("ESP32_BT_Scanner");

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
}

void loop() {
    int wifiCount = WiFi.scanNetworks();
    int btCount = scanBluetoothDevices();

    Serial.printf("WiFi Devices: %d, Bluetooth Devices: %d\n", wifiCount, btCount);

    sendDeviceCount(wifiCount, btCount);
    delay(10000); // Scan every 10 seconds
}

// Function to scan for Bluetooth Classic devices
int scanBluetoothDevices() {
    Serial.println("Scanning for Bluetooth devices...");
    int btCount = 0;

    if (SerialBT.hasClient()) {
        btCount = 1; // If a device is connected, count at least one.
    }

    // Note: The ESP32 Arduino framework does not support active scanning for classic Bluetooth devices.
    return btCount;
}

void sendDeviceCount(int wifi, int bt) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverUrl);
        http.addHeader("Content-Type", "application/json");

        String payload = "{\"wifi\":" + String(wifi) + ",\"bluetooth\":" + String(bt) + "}";
        Serial.println("Sending payload: " + payload);

        int httpResponseCode = http.POST(payload);
        String response = http.getString(); // Read server response

        Serial.printf("HTTP Response code: %d\n", httpResponseCode);
        Serial.println("Server response: " + response);

        http.end();
    } else {
        Serial.println("WiFi not connected");
    }
}