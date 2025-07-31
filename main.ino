#include <ESP8266WiFi.h>

const char* targetSSID = "Exit_signal";
const char* wifiPassword = "12345678";
const int ledPin = LED_BUILTIN;

// Timer variables
bool checkoutTimerStarted = false;
bool checkoutCompleted = false;
unsigned long checkoutStartTime = 0;
const unsigned long checkoutDuration = 30000; // 30 seconds

// Exit effect flag
bool exitEffectPlayed = false;

// Wi-Fi status tracking
wl_status_t lastWiFiStatus = WL_IDLE_STATUS;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // LED OFF

  WiFi.mode(WIFI_STA);
  WiFi.begin(targetSSID, wifiPassword);

  Serial.println("Trying to connect to Exit_signal...");

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to Exit_signal!");
  } else {
    Serial.println("\nFailed to connect to Exit_signal.");
    while (true) {
      digitalWrite(ledPin, HIGH); // LED OFF
      delay(1000);
    }
  }

  lastWiFiStatus = WiFi.status(); // Set initial status
}

void loop() {
  // Check and report Wi-Fi status only when it changes
  wl_status_t currentStatus = WiFi.status();
  if (currentStatus != lastWiFiStatus) {
    lastWiFiStatus = currentStatus;
    if (currentStatus == WL_CONNECTED) {
      Serial.println("WiFi connected");
    } else {
      Serial.println("WiFi disconnected, attempting reconnect...");
    }
  }

  // Proceed only if connected
  if (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    return;
  }

  int rssi = WiFi.RSSI();
  Serial.print("Current RSSI: ");
  Serial.println(rssi);

  if (rssi > -50) {
    // === EXIT ZONE ===
    Serial.println("EXIT ZONE");

    if (checkoutCompleted) {
      if (!exitEffectPlayed) {
        Serial.println("Checkout done. Blinking FAST for 5 seconds...");
        // Blink fast for 5 seconds
        for (int i = 0; i < 25; i++) {
          digitalWrite(ledPin, LOW);
          delay(100);
          digitalWrite(ledPin, HIGH);
          delay(100);
        }
        exitEffectPlayed = true;
        Serial.println("Gate unlocked. LED OFF.");
        digitalWrite(ledPin, HIGH); // LED OFF
      } else {
        digitalWrite(ledPin, HIGH); // Keep OFF after effect
      }
    } else {
      Serial.println("Checkout NOT completed. LED ON to block exit.");
      digitalWrite(ledPin, LOW); // LED ON solid
    }

  } else if (rssi > -70) {
    // === CHECKOUT ZONE ===
    Serial.println("CHECKOUT ZONE");
    exitEffectPlayed = false; // Reset exit effect if cart returns to checkout

    if (!checkoutTimerStarted) {
      Serial.println("Starting 30-second checkout timer...");
      checkoutStartTime = millis();
      checkoutTimerStarted = true;
      checkoutCompleted = false;
    }

    if (!checkoutCompleted && millis() - checkoutStartTime >= checkoutDuration) {
      checkoutCompleted = true;
      Serial.println("Checkout complete!");
    }

    if (!checkoutCompleted) {
      Serial.print("Checkout time left: ");
      Serial.print((checkoutDuration - (millis() - checkoutStartTime)) / 1000);
      Serial.println("s");
    }

    blinkLED(500); // Moderate blink during checkout

  } else {
    // === SHOPPING ZONE ===
    Serial.println("SHOPPING ZONE");

    checkoutTimerStarted = false;
    checkoutCompleted = false;
    exitEffectPlayed = false;

    blinkLED(1000); // Slow blink while shopping
  }
}

void blinkLED(int delayMs) {
  digitalWrite(ledPin, LOW);  // LED ON
  delay(delayMs);
  digitalWrite(ledPin, HIGH); // LED OFF
  delay(delayMs);
}
