* **Web-Based Dashboard:** User-friendly interface hosted directly on the ESP32 for easy smartphone control.
* **Power Stability:** Optimized with PWM soft-start and a dual-battery system to prevent WiFi disconnections.
* **Mobile Optimized:** Custom CSS to ensure smooth touch-based control without text selection interference.

---

### 🛠️ Hardware Components
* **Microcontroller:** ESP32 (WiFi-enabled)
* **Motor Driver:** L298N Dual H-Bridge
* **Sensors:** HC-SR04 Ultrasonic Sensor
* **Actuators:** SG90 Servo Motor
* **Power Source:** 7.4V Lithium-ion battery pack (Series setup)

---

### 🔌 Pin Mapping (ESP32)
| Component | GPIO Pin |
| :--- | :--- |
| Motor Driver (IN1, IN2, IN3, IN4) | D12, D13, D14, D27 |
| Ultrasonic Sensor (Trig, Echo) | D5, D18 |
| Servo Motor (PWM) | D19 |

---

### 🚀 How to Run
1. Upload the `.ino` code to your ESP32 using the Arduino IDE.
2. Connect your smartphone to the WiFi Access Point: **"Ankit_Smart_Car"**.
3. Open your browser and navigate to `192.168.4.1`.
4. Start driving!
