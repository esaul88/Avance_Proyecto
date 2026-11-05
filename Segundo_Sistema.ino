#include <IRremote.hpp>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEClient.h>

// --- TUS PINES EXACTOS ---
const int IR_SEND_PIN = 14;    
const int IR_RECEIVE_PIN = 32; 

// --- LA MAC EXACTA DE TU ROVER ---
String targetMacAddress = "48:87:2D:12:13:C5"; 

// --- UUIDs SERIALES DEL HC-08 ---
static BLEUUID serviceUUID((uint16_t)0xFFE0);
static BLEUUID charUUID((uint16_t)0xFFE2);

boolean connected = false;
BLEClient* pClient;
BLERemoteCharacteristic* pRemoteCharacteristic;

bool connectToRover() {
    Serial.print("Intentando conectar directamente a la MAC: ");
    Serial.println(targetMacAddress.c_str());
    
    // Crear el cliente BLE
    pClient = BLEDevice::createClient();
    
    // Conexión directa por dirección MAC (B Anula la necesidad de escanear)
    if (!pClient->connect(BLEAddress(targetMacAddress.c_str()))) {
      Serial.println(" -> [FALLO] El HC-08 rechazó la conexión o está fuera de rango.");
      delete pClient;
      return false;
    }
    Serial.println(" -> [OK] ¡Conexión física establecida con el rover!");

    // Buscar el servicio serial 0xFFE0
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.println(" -> [FALLO] No se encontró el servicio UUID 0xFFE0.");
      pClient->disconnect();
      delete pClient;
      return false;
    }
    Serial.println(" -> [OK] Servicio 0xFFE0 encontrado.");

    // Buscar la característica 0xFFE1 (para enviar datos)
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.println(" -> [FALLO] No se encontró la característica UUID 0xFFE1.");
      pClient->disconnect();
      delete pClient;
      return false;
    }
    
    Serial.println("¡ENLACE BLE COMPLETAMENTE LISTO!");
    return true;
}

void setup() {
  Serial.begin(115200); 
  delay(1000);
  Serial.println("\n--- Sistema Iniciado: Conexión Directa por MAC ---");

  // Iniciar periféricos IR
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  IrSender.begin(IR_SEND_PIN);

  // Inicializar antena Bluetooth
  BLEDevice::init("ESP32_Maestro");
}

void loop() {
  // Si no estamos conectados, intentar la conexión de forma continua con pausas
  if (!connected) {
    if (connectToRover()) {
      connected = true;
    } else {
      Serial.println("Reintentando conexión en 3 segundos...\n");
      delay(3000);
    }
  }

  // Si ya hay enlace activo, escuchar el control remoto IR y mandarlo al rover
  if (connected) {
    // Verificar si el cliente sigue conectado físicamente
    if (!pClient->isConnected()) {
      Serial.println("\n[ALERTA] Se perdió la conexión con el rover. Reiniciando bucle de enlace...");
      connected = false;
      return;
    }

    if (IrReceiver.decode()) {
      uint32_t command = IrReceiver.decodedIRData.command;
      String dataToSend = "";

     // Agregamos \n para obligar al HC-08 a vaciar su buffer
      if (command == 0x66) dataToSend = "f\n";      // Adelante
      else if (command == 0x62) dataToSend = "b\n"; // Atrás
      else if (command == 0x6C) dataToSend = "l\n"; // Izquierda
      else if (command == 0x72) dataToSend = "r\n"; // Derecha
      else dataToSend = "s\n";                      // Detener

      Serial.print("IR detectado (0x");
      Serial.print(command, HEX);
      Serial.print(") -> Enviando por Bluetooth: ");
      Serial.println(dataToSend);

      // Enviar datos al HC-08
      if (pRemoteCharacteristic != nullptr) {
        pRemoteCharacteristic->writeValue((uint8_t*)dataToSend.c_str(), dataToSend.length(), false);
      }
      
      IrReceiver.resume(); 
    }
  }
}