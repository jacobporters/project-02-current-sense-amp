# project-2-current-sense-amp
Build a reusable current/power measurement platform using the INA219 + ESP32 + SSD1306 with auto-ranging OLED display and WiFi CSV logging.

COMPLETE ✅ 

IMAGES

Goal: Build a current sense amp displaying live current, voltage, and power readings to an auto-ranging OLED display over I2C. Utilize an ESP32 to stream timestamped CSV rows over UDP to a python logger on a laptop. 

I learned:
- How to use I2C with multiple devices on one bus.
- Navigating an INA219, i.e. why getBusVoltage_V() + shunt correction gives true load voltage rather than using bus voltage alone, and why busV * currentMA is more accurate than getPower_mW() for low currents, due to the internal 2mW quantization on the hardware power register.
- Auto-ranging display functions and dealing with false precision.
- Troubleshooting ESP32 upload quirks and baud rates for different board variants.

How to flash:
- Board: ESP32 Dev Module
- Upload Speed: 115200 baud
- Hold BOOT button if upload fails at "Connecting..."
- Libraries required: Adafruit INA219, Adafruit SSD1306, Adafruit GFX, Adafruit BusIO (all via Library Manager)
- Edit SSID, PASSWORD, and LAPTOP_IP in the config block at the top of the sketch before uploading

How to run logger:
- Requires Python 3, standard library only (no pip installs)
- Run __python3 log_current.py__ before powering the ESP32
- Output writes to __current_log.csv__ in the same directory
- Stop with Ctrl+C

BOM:
| Part | Value / Part # | Qty | Notes |
|------|-----------------|-----|-------|
| Current sense amp | INA219 | 1 | Adafruit |
| Microcontroller | ESP32-D0WD-V3 dev board | 1 | - |
| Display | SSD1306 0.96" bi-color OLED (I2C) | 1 | - |
| Test load | 100Ω resistor | 1 | - |
| Misc | Breadboard + jumper wires | 1 | - |
