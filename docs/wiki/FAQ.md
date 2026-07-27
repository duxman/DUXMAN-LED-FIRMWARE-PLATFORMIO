# Preguntas frecuentes (FAQ)

**¿Que placas ESP32 estan soportadas?**
ESP32-C3-DevKitM-1 (`esp32c3supermini`), ESP32 DevKit (`esp32dev`), ESP32-S3-DevKitC-1 (`esp32s3`).

**¿Cuantas salidas LED puedo configurar?**
Hasta 4 salidas independientes por perfil GPIO.

**¿Puedo crear mis propias paletas de color?**
Si, desde la UI o la API (`/api/v1/palettes/save`).

**¿Como se actualiza el firmware?**
Actualmente por USB/Serial con PlatformIO. OTA seguro con rollback esta en el roadmap.

**¿Que backend LED es preferente?**
NeoPixelBus (DMA/I2S/RMT). FastLED esta disponible como alternativo en tiempo de compilacion.

**¿Puedo controlar el dispositivo por red y por Serial?**
Si, la API es identica en HTTP (`/api/v1/`) y en comandos Serial.

**¿Como funciona la sincronizacion multi-nodo?**
El stack S1-S6 esta completado: DDP, E1.31, cluster sync, reloj de fase compartido y telemetria runtime. Ver [Architecture](./Architecture) para detalles.

**¿Donde esta la documentacion tecnica completa?**
En esta wiki, el README del repositorio y los docs en `docs/`.

**¿Como reporto un bug o solicito una feature?**
Abre un issue en https://github.com/duxman/DUXMAN-LED-FIRMWARE-PLATFORMIO con logs y pasos de reproduccion.

**¿De donde viene este repositorio?**
Fue extraido del monorepo DUXMAN-LED-NEXT (ahora archivado y deprecado) para continuar el desarrollo de forma independiente.
