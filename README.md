# Config-ESP32 — MicroSD Storage Node (MQTT)

**ESP32 DevKit V1** node designed to receive route/map updates via **MQTT** over a local network (Access Point) and store them on a **MicroSD** card (`/ruta.txt`) in real time without rebooting the board.

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
   * `PubSubClient` (by Nick O'Leary) — install via the Library Manager.
   * `WiFi.h`, `SPI.h`, and `SD.h` — included natively with the ESP32 core.

---

## Configuration & Usage

1. Before uploading the sketch, fill in your network credentials in the code *(never commit real Wi-Fi passwords to the repository)*:
   * `ssid`: Wi-Fi network name broadcast by the Access Point.
   * `password`: Wi-Fi network password.
   * `mqtt_server`: Static IP address of the MQTT Broker (Mosquitto).
   * `topic`: Subscription channel (e.g., `marte/ruta`).
2. Select the assigned `COM` port and upload the code (`Upload Speed: 115200`).
3. Open the **Serial Monitor** at **`115200 baud`** to verify SD initialization, MQTT connection status, and live writes to `/ruta.txt`.
