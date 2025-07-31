#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // Ensure disconnected from any network
  delay(100);
  Serial.println("Starting WiFi scan...");
}

void loop() {
  Serial.println("\nScanning for WiFi networks...");
  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("No networks found");
  } else {
    Serial.printf("%d networks found:\n", n);
    for (int i = 0; i < n; ++i) {
      Serial.printf("%d: %s (RSSI: %d)\n", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
    }
  }
  WiFi.scanDelete(); // Free memory
  delay(5000);       // Wait 5 seconds before next scan
}
