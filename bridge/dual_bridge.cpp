#include <Arduino.h>
#include <painlessMesh.h>
#include <WiFi.h>

// --- Mesh Network Configuration (Must match all ESP32s) ---
#define MESH_PREFIX       "NOMBRE_DE_LA_MALLA"
#define MESH_PASSWORD     "CONTRASENA_DE_LA_MALLA"
#define MESH_PORT         5555
#define MESH_CHANNEL      1

// --- External AP Configuration (Laptop / Raspberry Pi Hotspot) ---
// Keep 'false' while testing only via USB cable so the ESP32 doesn't lose sync scanning for a missing AP.
// Set to 'true' when the external Wi-Fi AP is active (ensure the AP uses the same channel as MESH_CHANNEL).
#define USE_EXTERNAL_AP   false
#define EXTERNAL_SSID     "AP_LAPTOP_O_RASPBERRY"
#define EXTERNAL_PASS     "CONTRASENA_DEL_AP"
#define BRIDGE_TCP_PORT   8888

Scheduler userScheduler;
painlessMesh mesh;
WiFiServer bridgeServer(BRIDGE_TCP_PORT);

void receivedCallback(uint32_t from, String &msg) {
  Serial.printf("[MESH -> DUAL] From Node %u: %s\n", from, msg.c_str());
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

void broadcastRoute(const String& routeData, const String& source) {
  mesh.sendBroadcast(routeData);
  Serial.println("[" + source + " -> MESH] Broadcasted: " + routeData);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  mesh.setDebugMsgTypes(ERROR | STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, MESH_CHANNEL);

  // Designate this Dual ESP32 as the official gateway/root of the mesh
  mesh.setRoot(true);
  mesh.setContainsRoot(true);

  if (USE_EXTERNAL_AP) {
    mesh.stationManual(EXTERNAL_SSID, EXTERNAL_PASS);
    mesh.setHostname("Astroyans-Dual-Bridge");
    bridgeServer.begin();
    Serial.printf("[BRIDGE] Connecting to external AP '%s' (TCP Port %d)...\n", EXTERNAL_SSID, BRIDGE_TCP_PORT);
  }

  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onDroppedConnection(&droppedConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);

  Serial.print("[READY] Dual Bridge Node ID: ");
  Serial.println(mesh.getNodeId());
  Serial.println("Type a route in the Serial Monitor and press Enter to send it to the mesh:");
}

void loop() {
  mesh.update();

  // 1. Bridge input from USB Serial (Laptop / Raspberry Pi wired)
  if (Serial.available()) {
    String serialRoute = Serial.readStringUntil('\n');
    serialRoute.trim();
    if (serialRoute.length() > 0) {
      broadcastRoute(serialRoute, "USB");
    }
  }

  // 2. Bridge input from External Wi-Fi AP (TCP Socket)
  if (USE_EXTERNAL_AP) {
    WiFiClient client = bridgeServer.available();
    if (client && client.connected() && client.available()) {
      String wifiRoute = client.readStringUntil('\n');
      wifiRoute.trim();
      if (wifiRoute.length() > 0) {
        broadcastRoute(wifiRoute, "WIFI");
        client.println("BRIDGE_ACK: Route injected into mesh");
      }
    }
  }
}
