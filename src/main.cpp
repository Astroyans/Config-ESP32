#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define PIN_CS_SD 5
String lastSavedRoute = "";

void saveAndVerifySD(String newRoute) {
  // 1. Overwrite the file on the SD card
  File file = SD.open("/ruta.txt", FILE_WRITE);
  if (file) {
    file.print(newRoute);
    file.close();
    lastSavedRoute = newRoute;
    Serial.println("-> [OK] File updated on the SD card without rebooting.");
  } else {
    Serial.println("-> [ERROR] Could not write to the SD card.");
    return;
  }

  // 2. Read the file back to confirm it was saved
  file = SD.open("/ruta.txt", FILE_READ);
  if (file) {
    Serial.print("-> Current content on SD: ");
    while (file.available()) {
      Serial.write(file.read());
    }
    Serial.println("\n-----------------------------------");
    file.close();
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting MicroSD module...");
  if (!SD.begin(PIN_CS_SD)) {
    Serial.println("[FAIL] SD card not detected. Check the wiring or the format (must be FAT32).");
    while (true); // Halt here on wiring error
  }
  Serial.println("[READY] SD card detected. Type a route above and press Enter:");
}

void loop() {
  // Simulates receiving an update from the network
  if (Serial.available()) {
    String incomingData = Serial.readStringUntil('\n');
    incomingData.trim(); // Strip invisible line breaks

    if (incomingData.length() > 0) {
      if (incomingData != lastSavedRoute) {
        Serial.println("Change detected: " + incomingData);
        saveAndVerifySD(incomingData);
      } else {
        Serial.println("Data is identical to the current one. Nothing to do.");
      }
    }
  }
}
