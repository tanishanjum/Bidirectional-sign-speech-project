
# Bidirectional Sign-to-Speech and Speech-to-Sign Converter

Final Year ECE Project (2025–2026)  
Ballari Institute of Technology and Management, Ballari  
Student: Tanish Anjum M.S 
  

---

##  Project Overview

This project presents a Bidirectional Sign-to-Speech and Speech-to-Sign Converter designed to enable real-time communication between hearing/speech-impaired individuals and normal users. The system converts hand gestures into text and speech, and spoken words into visual sign representation.

The project uses a smart glove integrated with flex sensors and an accelerometer, an ESP32 microcontroller for processing, Bluetooth for wireless communication, and OLED/LCD displays for output.

---

##  Objectives

- To design a smart glove using four flex sensors and an accelerometer
- To convert recognized hand gestures into text and speech
- To implement speech recognition using a mobile phone and transmit text to ESP32 via Bluetooth
- To display speech-to-sign output on an OLED screen
- To support real-time bidirectional communication
- To make the system portable, low-cost, and user friendly

---

## System Features

- Real-time sign-to-speech conversion  
- Real-time speech-to-sign conversion  
- Wireless Bluetooth communication  
- Wearable smart glove interface  
- OLED and LCD display output  
- Clear speech output using speaker module  
- Offline operation (no internet required)

---

## Hardware Requirements

- ESP32 Microcontroller  
- Flex Sensors (4 Nos.)  
- MPU6050 Accelerometer  
- OLED Display  
- LCD Display  
- Speaker / Voice Output Module  
- Bluetooth (In-built in ESP32)  
- Gloves  
- Connecting Wires and Breadboard  
- Power Supply / Battery  

---

## Software Requirements

- Arduino IDE  
- Embedded C / Arduino Programming Language  
- Mobile Application for Speech Recognition  
- Serial Monitor for Debugging  

---

## Working Principle

### 1. Sign-to-Speech Mode
The user performs a hand gesture using the smart glove. Flex sensors and the accelerometer capture finger bending and hand movement. The ESP32 processes the sensor data, recognizes the gesture, displays the corresponding text on the LCD, and produces speech output through the speaker.

### 2. Speech-to-Sign Mode
The normal user speaks into the mobile phone. The mobile application converts speech into text and transmits it to the ESP32 via Bluetooth. The received text is displayed on the OLED along with sign representation for the hearing-impaired user.

---

## Testing & Results

- Accurate detection of predefined gestures  
- Clear text display on LCD and OLED  
- Audible and clear speech output  
- Stable Bluetooth communication within 5–10 meters  
- Real-time response with minimal delay  
- Reliable operation under repeated test cycles  

---

## Future Scope

- Integration of Artificial Intelligence and Machine Learning  
- Expansion of gesture vocabulary  
- Multilingual speech output support  
- Cloud and IoT connectivity  
- Miniaturization using custom PCB  
- Development of a full-featured Android mobile app  
- Battery life optimization  

---

## Repository Contents

- `2WAYS.ino` – Main ESP32 source code  
- `ESP32_Code.pdf` – Full project source code (PDF version)  
- `README.md` – Project documentation  

---

## Author

**Tanish Anjum M.S**  
Final Year ECE Student  
Ballari Institute of Technology and Management  
Academic Year: 2025–2026  

---

## 📜 License

This project is developed for academic purposes as part of a final year engineering project.

