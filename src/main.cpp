#include <Arduino.h>
#include <painlessMesh.h>
#include <SPI.h>
#include <SD.h>

// --- MicroSD Configuration (SPI) ---
#define SD_CS 5
const char* FILE_PATH = "/ruta.txt";

// --- Mesh Network Configuration (Must match the Dual node) ---
#define MESH_PREFIX       "NOMBRE_DE_LA_MALLA"
#define MESH_PASSWORD     "CONTRASENA_DE_LA_MALLA"
#define MESH_PORT         5555
#define MESH_CHANNEL      1

Scheduler userScheduler;
painlessMesh mesh;
bool sdReady = false;

bool saveRouteToSD(const String& newRoute) {
  if (!sdReady) {
    Serial.println("[ERROR] SD card not initialized.");
    return false;
  }

  if (SD.exists(FILE_PATH)) {
    SD.remove(FILE_PATH);
  }

  File file = SD.open(FILE_PATH, FILE_WRITE);
  if (!file) {
    Serial.println("-> [ERROR] Failed to open /ruta.txt for writing.");
    return false;
  }

  file.print(newRoute);
  file.close();
  Serial.println("-> [OK] /ruta.txt updated without rebooting.");

  // Verify written content
  file = SD.open(FILE_PATH, FILE_READ);
  if (file) {
    Serial.print("-> [SD VERIFIED]: ");
    while (file.available()) {
      Serial.write(file.read());
    }
    Serial.println();
    file.close();
  }
  return true;
}

void receivedCallback(uint32_t from, String &msg) {
  // Ignore acknowledgment messages from other nodes
  if (msg.startsWith("ACK:")) return;

  Serial.printf("\n[DUAL -> NODE] Route received from %u: %s\n", from, msg.c_str());

  if (saveRouteToSD(msg)) {
    mesh.sendSingle(from, "ACK: Saved /ruta.txt (" + String(msg.length()) + " bytes)");
  } else {
    mesh.sendSingle(from, "ACK: Received, but SD write failed");
  }
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> [CONNECTED] Node %u joined the mesh.\n", nodeId);
}

void droppedConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> [DISCONNECTED] Node %u dropped from the mesh.\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("--> [TOPOLOGY] Active mesh peers: %d\n", mesh.getNodeList().size());
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

  // Inform this node that a root bridge exists in the network
  mesh.setContainsRoot(true);

  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onDroppedConnection(&droppedConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);

  Serial.print("[READY] Normal Storage Node ID: ");
  Serial.println(mesh.getNodeId());
}

void loop() {
  mesh.update();
}
