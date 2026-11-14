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

To compile and flash the firmware using **Arduino IDE**:

1. **USB-to-Serial Driver:**
   * This board uses the **CP2102** USB bridge and requires the **CP210x Universal Windows Driver** (by Silicon Labs) to assign a `COM` port on Windows *(if using a CH340 board variant, install the `CH341SER` driver instead)*.
2. **Arduino IDE Board Support:**
   * Install the **esp32** board package (by Espressif Systems) via the Boards Manager.
   * Select board: `DOIT ESP32 DEVKIT V1`.
3. **Required Libraries:**
   * `SPI.h` and `SD.h` — included natively with the ESP32 core.

---

## How to Run & Verify (Current Stage)

1. Connect the ESP32 to your PC using a **data-capable USB cable** and select the assigned `COM` port under **Tools > Port**.
2. Upload the sketch (`Upload Speed: 115200`).
3. Open the **Serial Monitor** (`Ctrl + Shift + M`) and set the baud rate to **`115200 baud`**.
4. Press the physical **`EN`** (reset) button on the ESP32 to verify SD card initialization.
5. Type a test route string in the Serial Monitor input bar (e.g., `X:12.4, Y:45.8`) and press **Enter** to confirm `/ruta.txt` is overwritten and read back without rebooting.
