#include <SPI.h>
#include <SD.h>

#define PIN_CS_SD 5
String ultimaRutaGuardada = "";

void guardarYVerificarSD(String nuevaRuta) {
  // 1. Sobrescribe el archivo en la SD
  File archivo = SD.open("/ruta.txt", FILE_WRITE);
  if (archivo) {
    archivo.print(nuevaRuta);
    archivo.close();
    ultimaRutaGuardada = nuevaRuta;
    Serial.println("-> [OK] Archivo actualizado en la SD sin reiniciar.");
  } else {
    Serial.println("-> [ERROR] No se pudo escribir en la SD.");
    return;
  }

  // 2. Lee el archivo de vuelta para confirmar que quedó guardado
  archivo = SD.open("/ruta.txt", FILE_READ);
  if (archivo) {
    Serial.print("-> Contenido actual en SD: ");
    while (archivo.available()) {
      Serial.write(archivo.read());
    }
    Serial.println("\n-----------------------------------");
    archivo.close();
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Iniciando módulo MicroSD...");
  if (!SD.begin(PIN_CS_SD)) {
    Serial.println("[FALLO] No se detecta la SD. Revisa los cables o el formato (debe ser FAT32).");
    while (true); // Se detiene aquí si hay error de cableado
  }
  Serial.println("[LISTO] Tarjeta SD detectada. Escribe una ruta arriba y presiona Enter:");
}

void loop() {
  // Simula recibir una actualización desde la red
  if (Serial.available()) {
    String datoEntrante = Serial.readStringUntil('\n');
    datoEntrante.trim(); // Limpia saltos de línea invisibles

    if (datoEntrante.length() > 0) {
      if (datoEntrante != ultimaRutaGuardada) {
        Serial.println("Cambio detectado: " + datoEntrante);
        guardarYVerificarSD(datoEntrante);
      } else {
        Serial.println("El dato es idéntico al actual. No se hace nada.");
      }
    }
  }
}