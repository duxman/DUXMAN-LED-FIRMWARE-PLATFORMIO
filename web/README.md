# DUXMAN LED — Landing Page Web

Página web de presentación y anuncio del proyecto DUXMAN LED Firmware.

## Características de la Web
- **100% estática (HTML5 + CSS3 puro)**: sin frameworks externos, sin dependencias de Node.js, carga instantánea y optimizada.
- **Responsive & Dark Mode**: adaptada a móviles, tablets y monitores.
- **Desacoplada del firmware**: ubicada en `/web/` y añadida a `.gitignore` para publicarla en hosting compartido, VPS, Nginx, Apache o en su propio repositorio GitHub Pages.

## Contenido Reflejado en la Propuesta
1. **Hero & Estadísticas**:
   - Render a **~62.5 FPS** en Core 1.
   - 4 salidas LED paralelas gestionables.
   - Streaming en vivo con **DDP y E1.31**.
   - 100% local, autónomo y sin nube.
2. **Ingeniería FreeRTOS Dual-Core**:
   - Diagrama visual de separación entre `controlTask` (Core 0, 10ms: red, API, audio, sync) y `renderTask` (Core 1, 16ms: motor de efectos, paletas, limitador de mA y drivers NeoPixelBus/FastLED/Digital).
3. **Capacidades & LittleFS**:
   - Web UI embebida completa (Efectos, Paletas, GPIOs, Red, Micrófono I2S, Perfiles, Hardware y Debug).
   - Gestión activa de consumo térmico y miliamperios.
   - Scheduler de persistencia diferida para no desgastar la memoria flash.
4. **Matriz de Hardware Soportado**:
   - `esp32c3supermini` (RISC-V, pin 8).
   - `esp32dev` (NodeMCU/DevKit v1, pin 5).
   - `esp32s3` (LX7 Dual-Core, pin 48).
5. **Streaming & Sincronización**:
   - Soporte para **LedFx** mediante DDP (4048) y E1.31 (5568).
   - Descubrimiento mDNS (`_ddp._udp`, `_e131._udp` y `_duxsync._udp`).
   - Sincronización en Cluster con reloj de fase compartido.
6. **API REST v1**:
   - Rutas principales y OpenAPI / Swagger embebida.
7. **Sección de Agradecimientos de Corazón**:
   - **WLED (kno.wled.ge)**: agradecimiento destacado a Christian Schwinne (Aircoookie) y la comunidad WLED como faro de aprendizaje del mundo de luz y color.
   - **LedFx (ledfx/ledfx y @ledfx_)**: reconocimiento a su software y comunidad para iluminación reactiva al audio.
   - Librerías Open Source: NeoPixelBus, FastLED, ArduinoJson y PlatformIO.

## Cómo desplegar
Copia el contenido de esta carpeta (`index.html`, `style.css` y `README.md`) al directorio raíz web de tu servidor (`/var/www/html`, `public_html`, etc.).
