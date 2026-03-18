# Traffic Signal Rule Violation Detection with Telegram Alert

This project detects vehicles that cross a traffic signal during a red light using an ultrasonic sensor, captures a photo of the violating vehicle using an ESP32-CAM, and sends the image to a Telegram chat for remote monitoring. It combines an Arduino-based traffic light controller with an ESP32-CAM that acts as a smart camera bot.

## 📌 Overview

The system consists of two main parts:

1. **Traffic Light Controller (Arduino)**: Simulates a traffic light (Green → Yellow → Red) and monitors for violations during the red phase using an HC-SR04 ultrasonic sensor. When a vehicle passes within a set distance (e.g., 28 cm) while the light is red, it sends a trigger signal to the camera module.

2. **Camera Module (ESP32-CAM)**: Listens for the trigger signal (either via a digital pin or serial communication), captures a photo, and sends it to a predefined Telegram chat using a Telegram Bot. It also supports manual commands (`/photo`, `/flash`, `/readings`) and can optionally be triggered by a PIR motion sensor.

The two modules work together to provide an automated traffic enforcement prototype.

## 🧰 Components Required

| Component                | Quantity | Purpose                               |
|--------------------------|----------|---------------------------------------|
| Arduino Uno (or similar) | 1        | Traffic light control & violation detection |
| ESP32-CAM module         | 1        | Camera & Telegram bot                  |
| HC-SR04 Ultrasonic Sensor| 1        | Measure vehicle distance               |
| LEDs (Red, Yellow, Green)| 3        | Traffic light indicators               |
| Resistors (220Ω)         | 3        | Current limiting for LEDs              |
| White LED (optional)     | 1        | Visual violation indicator             |
| PIR Motion Sensor (optional) | 1    | Motion-triggered photo (ESP32-CAM)    |
| BME280 Sensor (optional) | 1        | Temperature & humidity readings        |
| Jumper wires             | many     | Connections                            |
| Breadboard               | 1        | Prototyping                            |
| Power supply             | -        | 5V for Arduino, 5V for ESP32-CAM       |

## 🔌 Hardware Connections

### Traffic Light Controller (Arduino)

| Arduino Pin | Component          |
|-------------|--------------------|
| 8           | Green LED (anode)  |
| 9           | Red LED (anode)    |
| 10          | Yellow LED (anode) |
| 2           | HC-SR04 TRIG       |
| 3           | HC-SR04 ECHO       |
| 5           | +5V (optional output) |
| GND         | Common ground      |

**LED wiring**: Connect cathode of each LED to GND via a 220Ω resistor.

### Camera Module (ESP32-CAM)

The ESP32-CAM uses the AI-THINKER pinout. Connect the following:

| ESP32-CAM Pin | Connection               |
|---------------|--------------------------|
| VCC           | 5V                       |
| GND           | GND                      |
| GPIO 4        | Onboard flash LED        |
| GPIO 13       | PIR motion sensor (optional) |
| GPIO 14       | I2C SDA (BME280)         |
| GPIO 15       | I2C SCL (BME280)         |
| U0R (RX)      | (Optional) Arduino TX    |
| U0T (TX)      | (Optional) Arduino RX    |

To program the ESP32-CAM, connect it to a USB-to-serial adapter (e.g., FTDI) with GPIO0 pulled to GND during upload.

### Connecting Arduino and ESP32-CAM (Trigger Signal)

Choose one of the following methods:

#### Option A: Digital Pin Trigger
- Connect an Arduino digital pin (e.g., D4) to an ESP32-CAM GPIO pin (e.g., GPIO12).
- In Arduino code, set the pin HIGH for a short pulse when a violation occurs.
- In ESP32-CAM code, configure that GPIO as input and monitor its state to trigger `sendPhoto = true`.

#### Option B: Serial Communication
- Connect Arduino TX (pin 1) to ESP32-CAM RX (U0R) and Arduino RX to ESP32-CAM TX (use a voltage divider on Arduino RX if necessary, as ESP32 is 3.3V).
- In Arduino, when violation occurs, send a simple character (e.g., 'V') over Serial.
- In ESP32-CAM, read the Serial port and set `sendPhoto` when 'V' is received.

## 💻 Software Setup

### 1. Install Arduino IDE
Download from [arduino.cc](https://www.arduino.cc/en/software).

### 2. Add ESP32 Board Support
- Open Arduino IDE → File → Preferences.
- Add this URL to "Additional Board Manager URLs":  
  `https://dl.espressif.com/dl/package_esp32_index.json`
- Go to Tools → Board → Board Manager, search for "ESP32" and install.

### 3. Install Required Libraries
- **For ESP32-CAM code** (via Library Manager):
  - `UniversalTelegramBot` by Brian Lough
  - `ArduinoJson` (required by Telegram bot)
  - `SparkFun BME280` (if using sensor)
- **For Arduino traffic light code**:
  - No extra libraries needed.

### 4. Telegram Bot Setup
1. Open Telegram and search for **@BotFather**.
2. Send `/newbot` and follow instructions to create a new bot.
3. Copy the **API token** (BOTtoken) provided.
4. Find your **Chat ID**:
   - Send a message to your bot.
   - Visit `https://api.telegram.org/bot<YOUR_BOT_TOKEN>/getUpdates`
   - Look for the `chat` object and copy the `id` value.

### 5. Code Configuration

#### ESP32-CAM Code (`Picture_Capture_and_Send_to_Telegram_Code.ino`)
- Replace the following placeholders:
  - `ssid` and `password` with your WiFi credentials.
  - `BOTtoken` with your Telegram bot token.
  - `chatId` with your Telegram chat ID.
- (Optional) Adjust camera settings (frame size, quality) as needed.
- If using external trigger, modify the `loop()` to monitor a GPIO or Serial and set `sendPhoto = true`.

#### Traffic Light Code (`Traffic_LED_Signal_Sequence_Code.ino`)
- Adjust `thresholdDistance` if needed (default 28 cm).
- The code pulses the white LED (pin 11) and prints "captureSignal" to Serial when a violation occurs. You can modify this section to also send a trigger signal to the ESP32-CAM.

### 6. Upload Code
- For Arduino: Select correct board and port, upload.
- For ESP32-CAM: Set board to "AI Thinker ESP32-CAM", put module in flashing mode (GPIO0 to GND), then upload.

## 📱 Usage

1. Power up both modules.
2. The traffic light will cycle: Green (2s) → Yellow (2s) → Red (2s).
3. During the red phase, the ultrasonic sensor continuously checks for objects closer than the threshold.
4. When a violation is detected, the Arduino triggers the camera (via chosen method).
5. The ESP32-CAM captures a photo and sends it to your Telegram chat.
6. You can also interact with the ESP32-CAM via Telegram commands:
   - `/start` – Welcome message and instructions
   - `/photo` – Manually request a photo
   - `/flash` – Toggle the onboard flash LED
   - `/readings` – Get temperature/humidity (if BME280 connected)

## 📁 Repository Files

- `Picture_Capture_and_Send_to_Telegram_Code.ino` – ESP32-CAM firmware with Telegram bot.
- `Traffic_LED_Signal_Sequence_Code.ino` – Arduino traffic light and violation detection.
- `README.md` – This file.

## 🔧 Troubleshooting

| Problem                          | Possible Solution |
|----------------------------------|-------------------|
| ESP32-CAM not connecting to WiFi | Check SSID/password, ensure 2.4GHz network. |
| Telegram messages not received    | Verify bot token and chat ID; ensure bot was started. |
| Camera capture fails              | Check power supply (ESP32-CAM needs 5V/1A); reduce frame size. |
| Ultrasonic sensor not detecting   | Check wiring; ensure 5V supply; adjust threshold. |
| No trigger signal between boards  | Verify connections; use a logic analyzer to check pulses. |

