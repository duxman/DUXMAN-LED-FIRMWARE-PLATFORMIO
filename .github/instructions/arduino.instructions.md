# Arduino y PlatformIO

## Objetivo

Estas instrucciones definen el criterio técnico para programar firmware con Arduino, ESP32 y Raspberry Pi Pico/4 usando PlatformIO o MicroPython cuando se aplique. El objetivo es escribir firmware seguro, eficiente, modular y documentado, con un enfoque de hardware realista y con trazabilidad de conexiones, memoria, flujo y validación.

## Principios generales

1. **Se prioriza la seguridad del hardware.** Antes de sugerir una conexión o un pin, se comprueba si es un pin válido para la función deseada, si hay restricciones por flash/strapping, si es entrada únicamente o si requiere nivel de tensión correcto.
2. **Se evita el bloqueo.** Toda lógica de control se diseña como máquina de estados o con timers no bloqueantes; `delay()` solo se usa si es imprescindible y se documenta.
3. **Se minimiza la memoria.** Evita `String` dinámico, buffers desproporcionados, tareas innecesarias y asignación no controlada. En ESP32 se revisa la huella del heap y de la pila.
4. **Se documenta el pinout.** Cualquier GPIO o periferio propuesto va acompañado de nombre, función, tensión, resistencia pull-up/down, dirección (entrada/salida), y referencia al esquema lógico.
5. **Se valida la compilación y la integración.** El firmware se prueba mediante compilación válidada por PlatformIO, revisión de warnings relevantes y comprobación de conectividad/pinout en el hardware.

## Contexto de lenguaje

### Si el proyecto es C++ / Arduino / PlatformIO

- Usa C++ moderno con tipos compactos (`uint8_t`, `int16_t`, `float`, etc.) cuando aporten claridad o ahorro de memoria.
- Prefiere `constexpr`, `enum class`, `struct` y funciones pequeñas con una responsabilidad clara.
- Usa `Serial.begin(baud)` solo con baudrate apropiado y con propósito de depuración, no como “log de producción” irresponsable.
- Si se trabaja con sensores, actuadores o redes, se modulariza por driver o capa de abstracción.
- Para ESP32, si hay tareas concurrentes o control de red, se considera `xTaskCreatePinnedToCore` con criterio y documentación.

### Si el proyecto es MicroPython

- Usa `machine`, `time`, `network`, `uasyncio` y `gc` cuando el hardware lo permita.
- Considera el ahorro de memoria para dispositivos pequeños: tareas breves, métodos simples y limpieza con `gc.collect()` si es necesario.
- Evita `time.sleep()` en bucles de control cuando se puede modelar un patrón de eventos no bloqueante.

## Seguridad del hardware

- Comprueba siempre la tensión de funcionamiento del MCU y del periférico.
- Si el nivel lógico del sensor o actuador difiere de la placa, se recomienda un conversor de nivel o un diseño adecuado.
- Revisa si el pin es un pin de entrada únicamente, de boot, de flash o de uso compartido por funciones especiales.
- No se conectan periféricos con consumo que la salida del MCU no pueda soportar sin protección.
- Cuando el usuario pide una conexión nueva, se describe la tabla: `Componente -> MCU`, voltage, pull-up/pull-down, tipo funcional, protección.

## Reglas de arquitectura

1. **Estructura simple y mantenible.**
   - Configuración central (`pins.h`, `config.h`, `platformio.ini`, `secrets.h` si procede).
   - Drivers de periféricos.
   - Lógica de control y FSM.
   - Tareas o bucles de supervisión y comunicación.
2. **No bloqueante.**
   - `millis()`, `micros()`, timers, eventos o tareas cooperativas.
   - `delay()` solo si es irremplazable y está justificado.
3. **Prevención de errores.**
   - Validación de sensor, protocolo y estado de salida.
   - Manejo de errores con contexto claro de hardware, pin y valores.
   - Recuperación por watchdog, reseteos y reintentos con backoff si aplica.
4. **Memoria y power.**
   - Sin `String` repetido.
   - Sin buffers grandes sin necesidad.
   - Se documenta consumo y arranque si el proyecto es portable o alimentado por batería.

## Pinout y documentación

- Cada GPIO propuesto debe aparecer en una tabla de conexiones con estas columnas: `Componente`, `Pin del componente`, `MCU Pin`, `Voltaje`, `Tipo`, `Notas`.
- Un pin debe incluir si es entrada, salida, ADC, DAC, I2C, SPI, UART o “input only”.
- En ESP32 se debe advertir de puertos de flash, bootstrapping y restricciones específicas por modelo (ESP32, ESP32-S2, ESP32-S3, ESP32-C3, etc.).
- Si el firmware usa redes, serial, sensores o buses, la documentación debe describir velocidad, puerto físico y lógica de comunicación.

## Librerías y dependencias

- Se usa la librería mínima necesaria y compatible con PlatformIO o con la arquitectura elegida.
- Para Arduino/ESP32, se prioriza la biblioteca oficial o ampliamente soportada y mantenida.
- Si se usa JSON para payloads o intercambio de datos, se prioriza `ArduinoJson` con tamaño adecuado al payload.
- No se añaden dependencias pesadas si el proyecto puede resolverlo con lógica simple, drivers propios o bibliotecas estándar del framework.

## Pruebas y validación

- La validación del firmware se hace mediante compilación y análisis de warnings de la plataforma.
- Si el hardware es accesible, se comprueba: voltajes, conexiones, estado de puertas, log a serial y comportamiento en el objetivo final.
- Para proyectos con sensores complejos o protocolos de red, se propone una prueba mínima de concepto: encender, leer, procesar y reportar.
- Si el firmware hace streaming de datos, comunicación serial o MQTT, se documenta la prueba de integración y los criterios de éxito.

## Comisión de cambios

- Los cambios deben seguir la convención de software: un incremento pequeño, verificable y documentado.
- El borrado o cambio de interfaz de un pin o protocolo se da como riesgo explícito antes de aceptar la modificación.
- Cuando el hardware o la memoria exigen una decisión técnica, se explica el porqué y se documenta la alternativa descartada.

## Fuentes de referencia

- `doc/esp32/oldaggent.md`
- `doc/esp32/ref2.md`
- `doc/esp32/referencias.md`
- Documentación oficial de los fabricantes y de PlatformIO / Arduino / ESP-IDF según corresponda.

## Prácticas declaradas

- **2026-11-07 — Incremento obligatorio de versión/fecha en cada commit:** Cada vez que el usuario indique o solicite preparar un commit o release, es obligatorio incrementar un micro/patch de versión (o build/versionCode) y actualizar la fecha (`DUX_FW_DATE` en `platformio.ini`, `buildDate` en `release-info.json`, `README.md` y `CHANGELOG.md`). Nunca hacer commit sin diferenciar la versión o fecha respecto al estado anterior.
