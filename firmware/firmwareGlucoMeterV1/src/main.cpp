#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define SDA_PIN A4
#define SCL_PIN A5

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

void setup(void) {
  Serial.begin(115200);
  Serial.println("¡Hola!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("No se encontró la placa PN53x");
    while (1); // Hacer un bucle infinito si no encuentra el módulo
  }

  // Configura el PN532 para leer tags NFC
  nfc.SAMConfig();
  Serial.println("Esperando por una etiqueta NFC...");
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    Serial.println("¡Se encontró una etiqueta NFC!");

    Serial.print("Longitud UID: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("Valor UID: ");
    for (uint8_t i=0; i < uidLength; i++) {
      Serial.print(" 0x");Serial.print(uid[i], HEX);
    }
    Serial.println("");

    delay(1000); // Espera un segundo antes de leer la siguiente etiqueta
  }
}
