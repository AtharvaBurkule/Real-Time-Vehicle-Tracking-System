
# Real-time Vehicle Tracking System

## Overview

The Real-time Vehicle Tracking System is an Arduino-based project designed to monitor and report vehicle location and detect accidents or falls. It uses GPS for location tracking and various sensors for detecting falls and vibrations. The data is displayed on an LCD and sent to a Firebase database for real-time monitoring. In case of an accident or fall, an SMS notification is sent.

## Components

- **Microcontroller**: Arduino
- **GPS Module**: For location tracking
- **LCD Display**: 16x2 LiquidCrystal I2C for showing real-time information
- **Vibration Sensor**: To detect vibrations and potential accidents
- **Accelerometer**: For detecting falls
- **Wi-Fi Module**: ESP32 or ESP8266 for network connectivity
- **Firebase**: For real-time database updates and notifications
- **GSM Module**: For sending SMS notifications

## Libraries

- `TinyGPSPlus`: To parse GPS data
- `LiquidCrystal_I2C`: To control the LCD
- `Firebase_ESP_Client`: For Firebase integration
- `WiFi.h` or `ESP8266WiFi.h`: For connecting to Wi-Fi

## Setup

1. **Install Libraries**:
   Make sure to install the necessary libraries (`TinyGPSPlus`, `LiquidCrystal_I2C`, `Firebase_ESP_Client`, `WiFi.h` or `ESP8266WiFi.h`) in the Arduino IDE.

2. **Network and Firebase Configuration**:
   - Replace the placeholders for Wi-Fi credentials (`WIFI_SSID`, `WIFI_PASSWORD`) with your actual network details.
   - Replace the `API_KEY` with your Firebase project's API key.
   - Update `DATABASE_URL` with your Firebase Realtime Database URL.

3. **Connections**:
   - Connect the GPS module to `Serial2` of the Arduino.
   - Connect the LCD to the I2C pins (usually `0x27` address).
   - Connect the accelerometer and vibration sensor to the specified analog and digital pins.

## Code Explanation

- **Setup**:
  - Initializes the LCD and Wi-Fi connection.
  - Configures Firebase with the API key and database URL.
  - Signs up and connects to Firebase.

- **Loop**:
  - Reads GPS data and updates the LCD and Firebase with the location.
  - Monitors accelerometer and vibration sensor data for fall detection and vibration.
  - Sends an SMS notification if an accident or fall is detected.

- **Functions**:
  - `SendMessage()`: Sends an SMS notification with the accident details.
  - `displayInfo()`: Displays GPS location on the LCD and updates Firebase.
  - `updateSerial()`: Forwards serial data (commented out in the provided code).

## Usage

1. Upload the code to your Arduino.
2. Ensure all hardware connections are properly made.
3. Open the Serial Monitor to view real-time status and debugging information.
4. The system will start monitoring and display data on the LCD. It will also send SMS notifications if an accident or fall is detected.

## Troubleshooting

- Ensure the GPS module is properly connected and has a clear view of the sky for accurate positioning.
- Verify that your Firebase credentials and database URL are correctly configured.
- Check the Serial Monitor for any error messages or connection issues.



