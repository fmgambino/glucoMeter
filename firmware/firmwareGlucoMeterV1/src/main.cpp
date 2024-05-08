#include <Wire.h>
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
    Serial.print("No se encontró la placa PN53x");
    while (1); // Detiene el programa si no se detecta el PN532
  }

  nfc.SAMConfig();

  Serial.println("Esperando una tarjeta NFC...");
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  // Intentamos leer una tarjeta NFC
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  // Verificamos si se ha encontrado una tarjeta
  if (success) {
    Serial.println("¡Se encontró una tarjeta NFC!");

    Serial.print("Longitud del UID: ");
    Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("Valor del UID: ");
    nfc.PrintHex(uid, uidLength);

    delay(1000); // Espera un poco antes de volver a escanear
  }
}
