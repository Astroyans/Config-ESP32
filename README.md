# Config-ESP32 — Local MicroSD Storage Node

**ESP32 DevKit V1** setup for writing, overwriting, and verifying route/map data on a **MicroSD** card (`/ruta.txt`) in real time via Serial communication without rebooting the board.

---

## Hardware & Wiring (SPI Bus)

* **Microcontroller:** ESP32 DevKit V1
* **Storage:** SPI MicroSD Card Reader Module + MicroSD card formatted as **FAT32** (≤ 32 GB recommended)

| ESP32 Pin | MicroSD Module Pin | Function |
| :--- | :--- | :--- |
| **VIN** (or **3V3***) | **VCC** | Power Supply |
| **GND** | **GND** | Ground |
| **D23** (GPIO 23) | **MOSI** | Data to SD (Master Out Slave In) |
| **D19** (GPIO 19) | **MISO** | Data from SD (Master In Slave Out) |
| **D18** (GPIO 18) | **SCK** | SPI Clock |
| **D5** (GPIO 5) | **CS** | Chip Select |

> *Note: Most generic MicroSD modules include an onboard 5V-to-3.3V voltage regulator, so they generally work more reliably when powered from the **VIN** (5V) pin.*

---

## Software & Driver Requirements

To compile and flash the firmware using **PlatformIO**:

1. **USB-to-Serial Driver:**
   * This board uses the **CP2102** USB bridge and requires the **CP210x Universal Windows Driver** (by Silicon Labs) to assign a `COM` port on Windows *(if using a CH340 board variant, install the `CH341SER` driver instead)*.
   * On **Linux**, no driver is needed, but your user must belong to the `uucp` (Arch) or `dialout` (Debian/Ubuntu) group to access the serial port, e.g. `sudo usermod -aG uucp $USER` (then log out and back in).
2. **PlatformIO Core:**
   * Install with `pipx install platformio`, or use the **PlatformIO IDE** extension for VS Code.
   * The `espressif32` platform and Arduino framework are downloaded automatically on the first build (board: `esp32doit-devkit-v1`).
3. **Required Libraries:**
   * `SPI.h` and `SD.h` — included natively with the ESP32 core. No `lib_deps` needed.

---

## How to Run & Verify (Current Stage)

1. Connect the ESP32 to your PC using a **data-capable USB cable**.
2. Build the firmware:
   ```bash
   pio run
   ```
3. Flash the board (upload speed is set to 115200 in `platformio.ini`):
   ```bash
   pio run -t upload
   ```
4. Open the serial monitor at **115200 baud**:
   ```bash
   pio device monitor
   ```
5. Press the physical **`EN`** (reset) button on the ESP32 to verify SD card initialization.
6. Type a test route string in the serial monitor (e.g., `X:12.4, Y:45.8`) and press **Enter** to confirm `/ruta.txt` is overwritten and read back without rebooting.

---

## Project Layout

```
Config-ESP32/
├── platformio.ini   # PlatformIO environment (esp32doit-devkit-v1, Arduino framework)
├── src/
│   └── main.cpp     # Firmware: SD write/read-back test over Serial
├── AGENTS.md        # Guidance for AI coding agents
├── ROADMAP.md       # Phased plan and task tracking
├── README.md
└── .gitignore
```
