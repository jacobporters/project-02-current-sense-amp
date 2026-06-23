#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// ── Config ──────────────────────────────────────────
const char* SSID     = "";
const char* PASSWORD = "";
const char* LAPTOP_IP = "";  // laptop IP
const int   UDP_PORT  = 5005;
const int   LOG_INTERVAL_MS = 500;  // log every 500ms
// ────────────────────────────────────────────────────

Adafruit_INA219  ina219;
Adafruit_SSD1306 display(128, 64, &Wire, -1);
WiFiUDP          udp;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

 // INA219
  if (!ina219.begin()) {
    Serial.println("INA219 not found!");
    while (1) delay(10);
  }
  ina219.setCalibration_16V_400mA();  // better resolution for low currents

 // SSD1306
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 not found!");
    while (1) delay(10);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected. IP: " + WiFi.localIP().toString());
  udp.begin(UDP_PORT);
}

void displayCurrent(float current_mA) {
  display.setTextSize(2);
  display.setCursor(0, 42);

  if (abs(current_mA) >= 1000) {
    display.print(current_mA / 1000.0, 2);
    display.print("A");
  } else {
    display.print(current_mA, 1);
    display.print("mA");
  }
}

void displayVoltage(float voltage_V) {
  display.setTextSize(2);
  display.setCursor(0, 10);

  if (abs(voltage_V) >= 1) {
    display.print(voltage_V, 2);
    display.print("V");
  } else {
    display.print(voltage_V * 1000.0, 1);
    display.print("mV");
  }
}

void displayPower(float power_mW) {
  display.setTextSize(1);
  display.setCursor(80, 56);

  if(abs(power_mW) >= 1000) {
    display.print(power_mW / 1000.0, 1);
    display.print("W");
  } else {
    display.print(power_mW, 1);
    display.print("mW");
  }
}

unsigned long lastLog = 0;

void loop() {
  float busV = ina219.getBusVoltage_V();
  float current = ina219.getCurrent_mA();
  float power   = ina219.getPower_mW();
  float loadV   = busV + (ina219.getShuntVoltage_mV() / 1000.0);

  // ── OLED display ──
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Voltage:");
  displayVoltage(loadV);

  display.setTextSize(1);
  display.setCursor(0, 32);
  display.print("Current:");
  displayCurrent(current);

  displayPower(power);

  display.display();

  // ── WiFi CSV logging ──
  if (millis() - lastLog >= LOG_INTERVAL_MS) {
    String row = String(loadV, 3) + "," +
                 String(current, 2) + "," +
                 String(power, 2);

    udp.beginPacket(LAPTOP_IP, UDP_PORT);
    udp.print(row);
    udp.endPacket();

    lastLog = millis();
  }

  delay(100);
}
