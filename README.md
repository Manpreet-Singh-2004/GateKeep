# Gatekeep

Gatekeep is a Wi-Fi-based cart control system inspired by **Gatekeeper** by Loblaw. NODEMCU or ESP8266 here acts as the wheel. You can extend the range using Wi-Fi Access Points and repeaters. Also, you can use multiple access points to mark entry and exit points (Wi-Fi SSIDs) for more accurate use cases. It uses an ESP8266 (NodeMCU) to control cart movement by detecting proximity to a Wi-Fi hotspot and enforcing a checkout waiting period before allowing exit.

---

## Features

- Detects zones based on Wi-Fi signal strength (RSSI) from a hotspot named `"Exit_signal"`.
- Defines three zones with different behaviors:
  - **Shopping Zone** (Far)  
    - RSSI: `< -70 dBm`  
    - LED blinks **slowly** (1-second interval).
  - **Checkout Zone** (Moderate)  
    - RSSI: `-70 dBm to -50 dBm`  
    - LED blinks **moderately** (0.5-second interval).  
    - Starts a **30-second timer** simulating checkout.
  - **Exit Zone** (Close)  
    - RSSI: `> -50 dBm`  
    - If checkout timer is **completed**, LED blinks **fast** (0.1-second interval) for 5 seconds, then turns off (indicating gate unlocked).  
    - If checkout timer is **not completed**, LED stays solid **ON** (indicating exit denied).

- Uses the built-in LED on NodeMCU (GPIO2) for feedback; configurable for other boards.
- Simple, clean Arduino IDE code for easy modification.

---

## Hardware

- **NodeMCU (ESP8266)** — other ESP8266-based boards can be used (change `ledPin` if necessary).
- Connected via USB to a PC for serial monitoring.

---

## Software

- Developed in **Arduino IDE** using the `ESP8266WiFi` library.
- Additional helper sketch:  
  - `Wifi_scanning.ino` — scans all available Wi-Fi networks and shows their SSIDs and RSSI values to help calibrate and test your environment.

---

## Notes

- Default Wi-Fi SSID: Exit_signal

  Default password: 12345678
  
  You can change both in the code:

  
```cpp
  const char* ssid = "Exit_signal"; 
  const char* password = "12345678";
 ```

- RSSI thresholds (`-50 dBm`, `-70 dBm`) can be adjusted based on your store layout and desired zone distances.
- BLE could provide a stealthier proximity detection, but is **not implemented** here because ESP8266 lacks BLE support.
- The LED behavior simulates gate states and can be replaced or complemented by a buzzer, relay, or motor controller to physically lock/unlock gates.


---

## Usage

1. Upload the `Gatekeep.ino` sketch to your NodeMCU.
2. Set your phone or router hotspot SSID to `"Exit_signal"` and configure the password in the sketch.
3. Open Serial Monitor (115200 baud) to view connection status, zone detections, and timer countdown.
4. Observe the LED behavior as you move between shopping, checkout, and exit zones.

