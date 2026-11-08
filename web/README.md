# DUXMAN LED — Landing Page Web & Wiki Técnica Embebida

Página web de presentación, catálogo de funcionalidades y documentación técnica integrada del proyecto **DUXMAN LED Firmware (ESP32)**.

## Características de la Web
- **100% estática (HTML5 + CSS3 + JS puro)**: sin frameworks externos, sin dependencias de Node.js, carga instantánea y optimizada.
- **Responsive & Dark Mode**: adaptada a móviles, tablets y monitores.
- **Wiki Técnica Integrada**: toda la documentación técnica de `docs/wiki/` embebida directamente con panel de navegación lateral sin necesidad de enlazar a repositorios externos.
- **Desacoplada del firmware**: ubicada en `/web/` y añadida a `.gitignore` para publicarla en hosting compartido, VPS, Nginx, Apache o en su propio repositorio GitHub Pages.

## Contenido Reflejado en la Propuesta
1. **Hero & Estadísticas**:
   - Render a **~62.5 FPS** en Core 1.
   - 4 salidas LED paralelas gestionables.
   - Streaming en vivo con **DDP y E1.31**.
   - 100% local, autónomo y sin nube.
2. **Guía de Inicio Rápido (3 pasos)**:
   - Flasheo por PlatformIO / PowerShell.
   - Conexión eléctrica segura y GPIOs.
   - Conexión directa vía Web UI local (AP o router).
3. **Ingeniería FreeRTOS Dual-Core**:
   - Diagrama visual de separación entre `controlTask` (Core 0, 10ms: red, API, audio, sync) y `renderTask` (Core 1, 16ms: motor de efectos, paletas, limitador de mA y drivers NeoPixelBus/FastLED/Digital).
4. **Catálogo de Efectos**:
   - Visuales: Lava Flow, Polar Ice, Gradient Meteor, Scanning Pulse, Triple Chase, Breath/Blink, Pop, Diagnostic.
   - Audio-reactivos (I2S): Neon EQ, Spectrum, Audio Pulse, Spectrum Chase, Rainbow Wave, Bouncing Physics, Stellar Twinkle, Trig Ribbon.
5. **Capacidades & LittleFS**:
   - Web UI embebida completa (Efectos, Paletas, GPIOs, Red, Micrófono I2S, Perfiles, Hardware y Debug).
   - Gestión activa de consumo térmico y miliamperios.
   - Scheduler de persistencia diferida para no desgastar la memoria flash.
6. **Matriz de Hardware Soportado**:
   - `esp32c3supermini` (RISC-V, pin 8).
   - `esp32dev` (NodeMCU/DevKit v1, pin 5).
   - `esp32s3` (LX7 Dual-Core, pin 48).
7. **Streaming & Sincronización**:
   - Soporte para **LedFx** mediante DDP (4048) y E1.31 (5568).
   - Descubrimiento mDNS (`_ddp._udp`, `_e131._udp` y `_duxsync._udp`).
   - Sincronización en Cluster con reloj de fase compartido.
8. **Wiki Técnica Embebida (9 secciones interactivas)**:
   - Visión General & Inicio (compilación y particionado `huge_app`).
   - Arquitectura Interna (servicios activos y cadena de render).
   - API REST v1 & Serial (endpoints y peticiones HTTP).
   - Esquema de Configuración (`/config.json`).
   - Perfiles GPIO & Limitador de Corriente.
   - Paletas de Color (sistema y usuario).
   - Sincronización & LedFx.
   - Roadmap & Evolución.
   - Preguntas Frecuentes (FAQ).
9. **Sección de Agradecimientos de Corazón**:
   - **WLED (kno.wled.ge)**: agradecimiento destacado a Christian Schwinne (Aircoookie) y la comunidad WLED como faro de aprendizaje del mundo de luz y color.
   - **LedFx (ledfx/ledfx y @ledfx_)**: reconocimiento a su software y comunidad para iluminación reactiva al audio.
   - Librerías Open Source: NeoPixelBus, FastLED, ArduinoJson y PlatformIO.

## Cómo desplegar
Copia los archivos de esta carpeta (`index.html` y `style.css`) al directorio raíz web de tu servidor (`/var/www/html`, `public_html`, etc.).
