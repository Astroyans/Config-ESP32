#include <Arduino.h>
#include <painlessMesh.h>
#include <SPI.h>
#include <SD.h>

// --- MicroSD Configuration (SPI) ---
#define SD_CS 5
const char* FILE_PATH = "/ruta.txt";

// --- Mesh Network Configuration (Must match all ESP32 nodes) ---
#define MESH_PREFIX     "MESH_NAME"
#define MESH_PASSWORD   "MESH_PASSWORD"
#define MESH_PORT       5555
#define MESH_CHANNEL    1

Scheduler userScheduler;
painlessMesh mesh;
bool sdReady = false;

// Overwrite /ruta.txt and read back to verify integrity without rebooting
void saveRouteToSD(const String& newRoute) {
  if (!sdReady) {
    Serial.println("[ERROR] SD card not initialized. Cannot save route.");
    return;
  }

  if (SD.exists(FILE_PATH)) {
    SD.remove(FILE_PATH);
  }

  File file = SD.open(FILE_PATH, FILE_WRITE);
  if (!file) {
    Serial.println("-> [ERROR] Failed to open /ruta.txt for writing.");
    return;
  }

  file.print(newRoute);
  file.close();
  Serial.println("-> [OK] /ruta.txt updated in MicroSD without rebooting.");

  file = SD.open(FILE_PATH, FILE_READ);
  if (file) {
    Serial.print("-> [SD VERIFIED]: ");
    while (file.available()) {
      Serial.write(file.read());
    }
    Serial.println();
    file.close();
  }
}

void receivedCallback(uint32_t from, String &msg) {
  Serial.printf("\n[MESH RX from %u] %s\n", from, msg.c_str());
  saveRouteToSD(msg);
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> [CONNECTED] Node %u joined the mesh.\n", nodeId);
}

void droppedConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> [DISCONNECTED] Node %u dropped from the mesh.\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("--> [TOPOLOGY UPDATE] Active nodes: %d\n", mesh.getNodeList().size());
}

void setup() {
  Serial.begin(115200);
  delay(500);

  if (!SD.begin(SD_CS)) {
    Serial.println("[FAIL] SD card not detected. Check wiring or FAT32 format.");
    sdReady = false;
  } else {
    Serial.println("[READY] MicroSD card initialized.");
    sdReady = true;
  }

  mesh.setDebugMsgTypes(ERROR | STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, MESH_CHANNEL);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onDroppedConnection(&droppedConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);

  Serial.print("[READY] Mesh Storage Node ID: ");
  Serial.println(mesh.getNodeId());
}

void loop() {
  mesh.update();

  // Allows local manual testing via Serial Monitor
  if (Serial.available()) {
    String manualInput = Serial.readStringUntil('\n');
    manualInput.trim();
    if (manualInput.length() > 0) {
      Serial.println("\n[SERIAL TEST] Writing manual input to SD...");
      saveRouteToSD(manualInput);
    }
  }
}
