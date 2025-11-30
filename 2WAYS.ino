#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
Adafruit_MPU6050 mpu;

// ---- FLEX SENSOR CONFIG ----
const int FLEX_PINS[4] = {34, 35, 32, 33}; // ESP32 ADC pins
float flexRef[4];        // reference average values
float flexVal[4];        // current readings
const float flexThreshold = 400.0; // difference to trigger (adjust)

float accX = 0, accY = 0, accZ = 0;

unsigned long lastFlexRead = 0;
const int flexReadInterval = 50; // ms between checks


#define W 128
#define H 64
Adafruit_SSD1306 display(W, H, &Wire, -1);

String direction = "UP";
int fingerDown = 0;

// UNIVERSAL PALM SIZE (centered)
int palmW = 40;
int palmH = 23;

// CENTER PALM
int palmX = (W - palmW) / 2;
int palmY = (H - palmH) / 2;

// FINGER DIMENSIONS (auto-scaled)
int fW = 5;   // fixed width
int fL = 18;  // max length
int gap = 3;  // spacing
int checkFlexTrigger() {
  for (int i = 0; i < 4; i++) {
    flexVal[i] = analogRead(FLEX_PINS[i]);
    float diff = abs(flexVal[i] - flexRef[i]);
    if (diff > flexThreshold) {
      return i; // flex index 0–3 triggered
    }
    delay(100);
  }
  return -1; // no trigger
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    delay(200);
  }

   SerialBT.begin("ESP32_Flex_MPU6050");
  SerialBT.println("Bluetooth Ready...");

  // ----- Initialize MPU6050 -----
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found! Check wiring.");
    delay(10);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("MPU6050 initialized.\n");

  // ----- FLEX SENSOR CALIBRATION -----
  Serial.println("Calibrating flex sensors (10 samples each)...");
  for (int i = 0; i < 4; i++) {
    float sum = 0;
    for (int j = 0; j < 10; j++) {
      int val = analogRead(FLEX_PINS[i]);
      sum += val;
      delay(100);
    }
    flexRef[i] = sum / 10.0;
    Serial.print("Flex "); Serial.print(i + 1);
    Serial.print(" ref = "); Serial.println(flexRef[i]);
  }
  Serial.println("Calibration complete.\n");

  showWelcome();
}

void showWelcome() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(16, 20);
  display.print("WELCOME");
  display.display();
}

// ✅ Draw full hand fitting in screen
void drawPalm(String dir, int fd) {
  display.clearDisplay();

  // Draw palm
  display.fillRoundRect(palmX, palmY, palmW, palmH, 6, SSD1306_WHITE);

  // Thumb small bump
  display.fillCircle(palmX + palmW - 6, palmY + palmH - 6, 5, SSD1306_WHITE);

  // ===========================
  // FINGER AUTO-SCALING
  // ===========================

  // ******** UP ********
  if (dir == "UP") {
    int topY_up = palmY - fL;
    int topY_down = palmY - 6;

    for (int i = 0; i < 4; i++) {
      int fx = palmX + 4 + i * (fW + gap);
      int fy = (i == fd ? topY_down : topY_up);
      int len = palmY - fy;
      display.fillRect(fx, fy, fW, len, SSD1306_WHITE);
    }
  }

  // ******** DOWN ********
  else if (dir == "DOWN") {
    int base = palmY + palmH;
    int maxY = base + fL;
    if (maxY > H) fL = H - base - 2; // auto-fit

    for (int i = 0; i < 4; i++) {
      int fx = palmX + 4 + i * (fW + gap);
      int fy = base;
      int len = (i == fd ? fL / 2 : fL);
      display.fillRect(fx, fy, fW, len, SSD1306_WHITE);
    }
  }

  // ******** RIGHT ********
  else if (dir == "RIGHT") {
    int base = palmX + palmW;
    int maxX = base + fL;
    if (maxX > W) fL = W - base - 2;  // auto-fit

    for (int i = 0; i < 4; i++) {
      int fy = palmY + 4 + i * (fW + gap);
      int len = (i == fd ? fL / 2 : fL);
      display.fillRect(base, fy, len, fW, SSD1306_WHITE);
    }
  }

  // ******** LEFT ********
  else if (dir == "LEFT") {
    int base = palmX;
    int minX = base - fL;
    if (minX < 0) fL = base - 2;  // auto-fit

    for (int i = 0; i < 4; i++) {
      int fy = palmY + 4 + i * (fW + gap);
      int len = (i == fd ? fL / 2 : fL);
      display.fillRect(base - len, fy, len, fW, SSD1306_WHITE);
    }
  }

  display.display();
}


void loop() {




   sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  accX = a.acceleration.x;
  accY = a.acceleration.y;
  accZ = a.acceleration.z;

  // Periodically check flex sensors
  if (millis() - lastFlexRead >= flexReadInterval) {
    lastFlexRead = millis();

    int flexIndex = checkFlexTrigger();
    if (flexIndex != -1) {
      char sendChar = 0;

      // X-axis control
      if (accX < -5)      {sendChar = '1' + flexIndex;delay(3000); }      // 1–4
      else if (accX > 5)  {sendChar = '5' + flexIndex;delay(3000); }      // 5–8
      // Y-axis control
      else if (accY < -5) {sendChar = 'A' + flexIndex;delay(3000); }     // A–D
      else if (accY > 5)  {sendChar = 'E' + flexIndex;delay(3000); }      // E–H

      if (sendChar != 0) {
        Serial.print("Send: "); Serial.println(sendChar);
        SerialBT.write(sendChar);
      }
    }

    // Optional debug info
    Serial.print("AccX: "); Serial.print(accX, 2);
    Serial.print("  AccY: "); Serial.print(accY, 2);
    Serial.print("  | Flex: ");
    for (int i = 0; i < 4; i++) {
      Serial.print(flexVal[i]); Serial.print(" ");
    }
    Serial.println();
  }

  delay(10);

  

  if (SerialBT.available()) {
    char c = toupper(SerialBT.read());

    // Welcome
    if (c == 'H') {
      showWelcome();
      return;
    }

    // UP 1–4
    if (c >= '1' && c <= '4') {
      palmW=40;
      palmH=23;
      fingerDown = c - '1';
      drawPalm("UP", fingerDown);
      return;
    }

    // LEFT 5–8
    if (c >= '5' && c <= '8') {
       palmH=40;
      palmW=23;
      fingerDown = c - '5';
      drawPalm("LEFT", fingerDown);
      return;
    }

    // RIGHT 9,A,B,C
    if (c == '9' || c == 'A' || c == 'B' || c == 'C') {
      palmH=40;
      palmW=23;
      if (c == '9') fingerDown = 0;
      else fingerDown = c - 'A' + 1;
      drawPalm("RIGHT", fingerDown);
      return;
    }

    // DOWN D,E,F,G
    if (c == 'D' || c == 'E' || c == 'F' || c == 'G') {
       palmW=40;
      palmH=23;
      fingerDown = c - 'D';
      drawPalm("DOWN", fingerDown);
      return;
    }
  }
}
