[circleci-image]: https://img.shields.io/circleci/build/github/nestjs/nest/master?token=abc123def456
[circleci-url]: https://circleci.com/gh/nestjs/nest

![MI-iPhone](https://electronicagambino.com/wp-content/uploads/elementor/thumbs/cropped-Electronica-Gambino-e1684335474114-q6losum0uq8caxhait9doqxx83gv53yq2d8g8oiv7o.png)

![version](https://img.shields.io/badge/version-1.0.0-blue.svg?longCache=true&style=flat-square)
![license](https://img.shields.io/badge/license-MIT-green.svg?longCache=true&style=flat-square)

# Dispositivo Hacking para Parche FreeStyle - NFC

Este proyecto tiene como objetivo la lectura de datos de los parches FreeStyle utilizando lectores RFID RC522, PN532 y PN532 (Modelo Rojo). La implementación se realiza utilizando el framework Arduino en VSCode con PlatformIO.

## Requisitos

### Hardware
- Lector RFID RC522
- Lector RFID PN532 (Modelo Rojo)
- Arduino Nano
- Cables de conexión

### Software
- VSCode con PlatformIO
- Librerías para los lectores RFID

## Instalación

### Configurar VSCode con PlatformIO
1. Instalar VSCode desde [Visual Studio Code](https://code.visualstudio.com/).
2. Instalar la extensión PlatformIO desde el Marketplace de VSCode.

### Configurar el proyecto en PlatformIO
1. Crear un nuevo proyecto en PlatformIO y seleccionar la placa Arduino Nano.
2. Añadir las librerías necesarias para los lectores RFID en el archivo `platformio.ini` del proyecto.

```ini
[env:arduino nano]
platform = atmelavr
board = nano
```

# Proyecto de Conexión de Hardware RFID y Monitoreo de Glucosa

## Conexión de Hardware

### Lector RC522

Conectar el lector RC522 al Arduino Nano utilizando el protocolo ICSP:

- SDA -> Pin 10
- SCK -> Pin 13
- MOSI -> Pin 11
- MISO -> Pin 12
- IRQ -> No conectado
- GND -> GND
- RST -> Pin 9
- 3.3V -> 3.3V

## 📋 Esquema de Conexión ARDUINO NANO & LECTOR RC522:

![Esquema de Conexión ARDUINO NANO & LECTOR RC522](https://i.ibb.co/C0sS1SP/esquema-Nano-RC522-icsp.jpg)

### Lector PN532 (Modelo Rojo)

Conectar el lector PN532 al Arduino Nano utilizando el protocolo I2C:

- SDA -> A4
- SCL -> A5
- VCC -> 3.3V
- GND -> GND

## 📋 Esquema de Conexión ARDUINO NANO & LECTOR RC522:

![Esquema de Conexión ARDUINO NANO & LECTOR RC522](https://i.ibb.co/89rtvd4/esquema-Nano-RC522-i2c.jpg)

## Información sobre el Parche FreeStyle

El parche FreeStyle Libre es un dispositivo de monitoreo continuo de glucosa (CGM) que mide los niveles de glucosa en el líquido intersticial. Los datos se pueden obtener escaneando el parche con un lector NFC compatible, lo que permite a los usuarios monitorear sus niveles de glucosa en tiempo real.

## Código Ejemplo (Firmware)

```cpp
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Crear instancia MFRC522

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

void setup() {
    Serial.begin(9600); // Iniciar comunicación serial
    while (!Serial);

    SPI.begin(); // Iniciar bus SPI
    mfrc522.PCD_Init(); // Iniciar MFRC522

    Wire.begin(); // Iniciar bus I2C
    nfc.begin(); // Iniciar PN532
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        Serial.print("No se encontró la placa PN53x");
        while (1); // Detener ejecución
    }
    nfc.SAMConfig(); // Configurar seguridad en PN532
}

void loop() {
    // Lector RC522
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        Serial.print("UID de la tarjeta :");
        for (byte i = 0; i < mfrc522.uid.size; i++) {
            Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
            Serial.print(mfrc522.uid.uidByte[i], HEX);
        }
        Serial.println();
        mfrc522.PICC_HaltA(); // Detener lectura de la tarjeta
    }

    // Lector PN532
    uint8_t success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
    uint8_t uidLength;

    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
    if (success) {
        Serial.print("UID de la tarjeta :");
        for (uint8_t i = 0; i < uidLength; i++) {
            Serial.print(" ");
            Serial.print(uid[i], HEX);
        }
        Serial.println();
    }
}
```

# LibreMonitor - Monitoriza tu Parche FreeStyle

LibreMonitor es un pequeño dispositivo DIY que utiliza comunicación de campo cercano (NFC) para leer datos de un sensor FreeStyle Libre y transmitirlos mediante Bluetooth de baja energía a una aplicación de iPhone. LibreMonitor escanea el sensor cada dos minutos. Transfiere los últimos 32 valores históricos de las últimas ocho horas y los 16 valores de tendencia para el tiempo actual y los últimos 15 minutos, mostrándolos en un gráfico y una tabla.

**Importante:** Solo se utilizan los llamados datos en bruto y debes elegir la pendiente y la intersección por ti mismo para que la aplicación calcule valores útiles de glucosa. Valores que funcionan bien para mis sensores son 0.13 para la pendiente y -20 para el desplazamiento. Cualquier otra información interna, como de los sensores de temperatura, aún no se comprende completamente y se descarta.

Este código se publica para que otros puedan contribuir y ayudar a mejorarlo o utilizarlo para mejorar sus propios dispositivos.

LibreMonitor no tiene afiliación de ningún tipo con Abbott. Este es un proyecto DIY con fines de investigación. El código proporcionado aquí podría arrojar resultados incorrectos. Debes construir tu propio dispositivo y eres responsable de los resultados. Úsalo bajo tu propio riesgo.

## Qué necesitas para construir un LibreMonitor

### 1. Compra el hardware necesario

Los componentes necesarios para un LibreMonitor son:

- [Módulo NFC BM019](http://www.solutions-cubed.com/bm019/) capaz de comandos ISO/IEC 15693.
- [Simblee](https://www.simblee.com) o [RFDuino](http://www.rfduino.com) y un Escudo de Programación USB correspondiente. Se recomienda obtener un kit de inicio.
- Batería Lipo, por ejemplo, [esta](http://www.exp-tech.de/polymer-lithium-ion-battery-110mah-5695) (100 mAh es suficiente para un día completo).
- Cargador Lipo (opcional), por ejemplo, [este](https://www.adafruit.com/product/1304) o [este](https://www.adafruit.com/products/1904) de adafruit.
- Interruptor (opcional pero útil si montas un cargador Lipo).

### 2. Haz el cableado

Conecta las piezas según se muestra en el siguiente diagrama (cortesía de [libxMike](https://github.com/libxmike?tab=following)):

![Diagrama de Cableado](https://cloud.githubusercontent.com/assets/10375483/19703622/c866a0d0-9b04-11e6-9471-8056324664b5.jpg)

Se sugiere montar y probar todo en una protoboard antes de soldar el dispositivo final. A continuación, hay imágenes de otro dispositivo LibreMonitor sin cargador Lipo y con cargador Lipo para referencia.

### 3. Programa tu Simblee

Simblee es IoT para conectar a todos y a todo (IoT4EE). Incorpora conectividad móvil, Bluetooth® Smart, malla, nube y otras formas de conexión inalámbrica.

El software para programar Simblee es código estándar de Arduino. Consiste en `LibreMonitor.ino` y la biblioteca contenida en `LibreMonitorArduinoLibrary.zip`. Consulta la [guía de inicio rápido de Simblee](https://www.simblee.com/Simblee_Quickstart_Guide_v1.0.pdf) en el [sitio web de Simblee](https://www.simblee.com) para obtener instrucciones sobre cómo programar Simblee. Si cableaste tu LibreMonitor como se describe anteriormente, no olvides reconfigurar los pines SPI del Simblee en el archivo `variant.h` (consulta la información de cableado en `LibreMonitor.ino` para más detalles).

### 4. Construye y ejecuta la aplicación LibreMonitor para iOS

La aplicación iOS requiere Xcode 8, Swift 3.0 e iOS 10. Descarga el proyecto de Xcode. Ejecuta [Cocoapods](https://cocoapods.org) para instalar la biblioteca [Charts](https://github.com/danielgindi/Charts), necesaria para el gráfico de niveles de glucosa. Compila la aplicación, ejecútala en el teléfono y ábrela. Si deseas recibir notificaciones para valores altos o bajos de glucosa y ver un icono de insignia, permite la configuración correspondiente cuando se te solicite. Una vez que la aplicación esté en funcionamiento, configura los valores de pendiente y desplazamiento (por ejemplo, 0.13 y -20) y conéctate a tu Simblee presionando "connect". Una vez que se detecte y conecte el Simblee, el estado de "Simblee" debería cambiar a "Notifying" y aparecer en verde. Coloca el dispositivo LibreMonitor sobre tu FreeStyle Libre y en no más de dos minutos los datos deberían mostrarse o actualizarse. Consulta las capturas de pantalla para más detalles.

## Algunas explicaciones

La fila "Glucosa" muestra el valor actual de glucosa y dos "valores delta" que indican cómo se desarrollará la glucosa (extrapolación lineal para los próximos quince minutos). El primer valor delta es la diferencia entre el valor actual y el más antiguo, y el valor delta entre paréntesis es la diferencia entre el valor actual de glucosa y el valor de glucosa de hace 8 minutos, multiplicado por dos. Los dos valores de "pronóstico" de glucosa se calculan sumando los valores delta al valor actual de glucosa. Glucosa se calcula a partir del valor en bruto de la siguiente manera:

	glucosa = crudo * pendiente + desplazamiento

Las secciones "Últimos 15 minutos" y "Últimas ocho horas" muestran los valores de glucosa, la fecha correspondiente, el valor en bruto, los 6 bytes de datos leídos del sensor FreeStyle Libre y algunos otros datos de prueba.

## Solución de problemas generales

- Si un CRC es incorrecto, lo más probable es que el dispositivo no esté lo suficientemente cerca del sensor FreeStyle Libre.
- Si los datos no se actualizan, desconecta y vuelve a conectar.
- Si no puedes conectar el dispositivo, verifica si Bluetooth está encendido.

## Lecturas sugeridas

- [Blog de Pierre Vandevenne](http://type1tennis.blogspot.de) con información sobre los detalles internos del FreeStyle Libre y sugerencias sobre cómo elegir pendiente y desplazamiento. Sin su trabajo, todo esto probablemente no habría sido posible.

## Proyectos similares

- [LimiTTer](https://github.com/JoernL/LimiTTer): Dispositivo similar, pero los datos se envían a [xDrip+](https://github.com/jamorham/xDrip-plus), una aplicación Android.
- [Freestyle Libre Alarm](https://github.com/pimpimmi/LibreAlarm/wiki): Utiliza un reloj inteligente Sony para leer datos del FreeStyle Libre y enviarlos a un teléfono Android.
- [Proyecto Bluereader](https://www.startnext.com/bluereader) por [Sandra Kessler](http://unendlichkeit.net/wordpress/), quien recibió financiación para construir un dispositivo pequeño y ordenado. Intento adaptar este proyecto para que funcione con Bluereader una vez que los primeros dispositivos estén disponibles.
- [Aplicación lectora Android](https://github.com/vicktor/FreeStyleLibre-NFC-Reader) por Viktor Bautista que fue útil al inicio de este trabajo.

## Nota personal

A partir de septiembre de 2018: He utilizado diferentes versiones de LibreMonitor y hardware similar (Transmisor de Marek) basado en Simblee o RFDuino desde principios de 2016 hasta marzo de 2018 y me ha gustado mucho. Cuando comencé el proyecto, no había nada similar disponible. Mientras tanto, han evolucionado muchos otros proyectos y también está disponible hardware comercial. Desde marzo de 2018 he estado utilizando el hardware [MiaoMiao](https://www.miaomiao.cool) y debo decir que, en mi opinión, es mucho mejor que cualquier cosa que había visto antes. Por lo tanto, ya no apoyaré el hardware LibreMonitor, sino solo el hardware MiaoMiao con la aplicación LibreMonitor para iPhone. Así que si quieres usar LibreMonitor con MiaoMiao, usa la rama swift4. Si eliges usar el hardware LibreMonitor antiguo, elige la rama master antigua.


