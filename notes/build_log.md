What worked:
- I2C scanner confirmed both devices at expected addresses before writing full sketch
- Auto-ranging display functions cleanly handle mV↔V and mA↔A boundaries
- UDP logging reliably streams to Python at 500ms intervals

What broke / debugging notes:
- ESP32 upload failed at default baud rate — fixed by lowering to 115200
- INA219 showed 0V initially — traced to breadboard power rail not reaching VCC (jumper not fully seated)
- ~860mV ghost reading on display traced to bus voltage (VIN- to GND) being measured rather than load voltage (VIN+ to VIN-)
- Floating VIN+ reads 8-12mV consistently — confirmed as internal bias current behavior, not a fault

Design decisions:
- set __Calibration_16V_400mA()__ chosen over default for better resolution at low currents
- Auto-ranging capped at mV/V and mA/A boundaries only — INA219 hardware limits make kV/µA ranges meaningless here
- 2 decimal places for voltage display (4mV resolution makes 3 decimals false precision)

Reused downstream:
- The ESP32 + INA219 + SSD1306 platform and WiFi CSV logging pipeline built here are directly reused in Projects 3, 4, 8, 9, 10, and 11.

References:\
Adafruit INA219 guide (https://learn.adafruit.com/adafruit-ina219-current-sensor-breakpack) \
TI INA219 datasheet (https://www.ti.com/product/INA219) \
Adafruit SSD1306 library (https://github.com/adafruit/Adafruit_SSD1306)
