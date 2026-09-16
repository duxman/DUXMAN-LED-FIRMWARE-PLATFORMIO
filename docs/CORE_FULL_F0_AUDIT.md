# F0 - Auditoria funcional y baseline

Este documento registra la auditoria inicial de la iniciativa `core/full`. La finalidad de F0 es conocer el comportamiento actual antes de separar los perfiles de compilacion.

## Estado

- Fase: F0
- Estado: parcialmente completada
- Fecha: 2026-09-16
- Repositorio: DUXMAN-LED-FIRMWARE-PLATFORMIO
- Rama: main

## 1. Resultado ejecutivo

La auditoria confirma que la API actual cubre las acciones principales de `home`:

- Lectura y modificacion del estado.
- Encendido y apagado.
- Brillo.
- Seleccion de efecto.
- Velocidad y nivel.
- Secciones.
- Colores manuales.
- Paletas.
- Efectos de arranque.
- Secuencias de efectos.
- Estado de sincronizacion.
- Reinicio.

La API tambien expone configuracion de red, GPIO, microfono, debug/general, sincronizacion, perfiles, diagnostico, hardware, metricas y release.

Se detecta un trabajo de calidad para F1: la capa de estado no distingue claramente entre JSON invalido, parametro invalido y patch valido sin cambios. La API externa necesitara esa distincion.

## 2. Acciones observadas en `home`

Fuente auditada: `data/ui/home.html`.

| Accion | Datos enviados o consultados | Endpoint |
|---|---|---|
| Consultar sincronizacion | Estado de conexion | `GET /api/v1/sync/connected` |
| Aplicar paleta | `paletteId` | `POST /api/v1/palettes/apply` |
| Cargar persistencia | Efecto de arranque y secuencia | `GET /api/v1/effects` |
| Guardar efecto de arranque | Estado actual implicito | `POST /api/v1/effects/startup/save` |
| Anadir a secuencia | `durationSec` | `POST /api/v1/effects/sequence/add` |
| Eliminar de secuencia | `id` | `POST /api/v1/effects/sequence/delete` |
| Reiniciar | Sin parametros | `POST /api/v1/system/restart` |
| Cargar paletas | Catalogo de paletas | `GET /api/v1/palettes` |
| Cargar estado | Estado completo | `GET /api/v1/state` |
| Aplicar controles | Ver payload siguiente | `PATCH /api/v1/state` |

### Payload actual de `home` para aplicar controles

`home` envia los siguientes campos:

```json
{
  "power": true,
  "brightness": 180,
  "effect": "lava_flow",
  "sectionCount": 4,
  "effectSpeed": 60,
  "effectLevel": 7,
  "reactiveToAudio": false,
  "backgroundColor": "#000000",
  "primaryColors": ["#ff0000", "#00ff00", "#0000ff"]
}
```

Cuando hay una paleta seleccionada, `home` envia `paletteId` en lugar de `primaryColors`. El estado del firmware deriva `reactiveToAudio` del efecto seleccionado.

## 3. Matriz de paridad `home -> API`

| Capacidad de `home` | Implementacion actual | Resultado F0 |
|---|---|---|
| Leer estado | `GET /api/v1/state` | Cubierta |
| Encendido/apagado | `PATCH /api/v1/state` con `power` | Cubierta |
| Brillo | `PATCH /api/v1/state` con `brightness` | Cubierta |
| Efecto | `effect` o `effectId` | Cubierta |
| Secciones | `sectionCount` | Cubierta |
| Velocidad | `effectSpeed` | Cubierta |
| Nivel | `effectLevel` | Cubierta |
| Colores manuales | `primaryColors` | Cubierta |
| Color de fondo | `backgroundColor` | Cubierta |
| Paleta activa | `paletteId` o `palette` | Cubierta |
| Catalogo de efectos | `availableEffects` y `/effects` | Cubierta |
| Catalogo de paletas | `availablePalettes` y `/palettes` | Cubierta |
| Guardar arranque | `/effects/startup/save` | Cubierta |
| Anadir secuencia | `/effects/sequence/add` | Cubierta |
| Eliminar secuencia | `/effects/sequence/delete` | Cubierta |
| Consultar sync | `/sync/connected` | Cubierta |
| Reiniciar | `/system/restart` | Cubierta |

## 4. Estado configurable del motor

Fuente auditada: `firmware/src/core/CoreState.h` y `CoreState.cpp`.

| Campo | Tipo | Rango o formato observado | Lectura | Escritura |
|---|---|---|---|---|
| `power` | boolean | `true`/`false` | Si | Si |
| `brightness` | entero | `0..255` | Si | Si |
| `effectId` | entero | ID registrado | Si | Si |
| `effect` | string | Clave registrada | Si | Si |
| `sectionCount` | entero | `1..10` | Si | Si |
| `effectSpeed` | entero | `1..100` | Si | Si |
| `effectLevel` | entero | `1..10` | Si | Si |
| `effectTransitionMs` | entero | `0..1500` ms | Si | Si |
| `effectTransitionStyle` | string | `fade` o `wipe` | Si | Si |
| `paletteId` | entero | `-1` manual o ID valido | Si | Si |
| `palette` | string | Clave de paleta | Si | Si |
| `primaryColors` | array | 3 colores hex | Si | Si |
| `backgroundColor` | string | Color hex | Si | Si |
| `reactiveToAudio` | boolean | Derivado del efecto | Si | No directo |
| `audioLevel` | entero | Runtime | Si | No directo |
| `beatDetected` | boolean | Runtime | Si | No directo |
| `audioPeakHold` | entero | Runtime | Si | No directo |

## 5. Rutas API funcionales observadas

Fuente auditada: `firmware/src/api/ApiService.cpp`.

### Estado y sistema

```text
GET/PATCH/POST /api/v1/state
POST           /api/v1/system/restart
GET            /api/v1/diag
GET            /api/v1/hardware
GET            /api/v1/metrics
POST           /api/v1/metrics/reset
GET            /api/v1/release
GET            /api/v1/openapi.json
```

### Configuracion

```text
GET/PATCH/POST /api/v1/config/network
GET/PATCH/POST /api/v1/config/microphone
GET/PATCH/POST /api/v1/config/gpio
GET/PATCH/POST /api/v1/config/debug
GET/PATCH/POST /api/v1/config/general
GET/POST       /api/v1/config/all
```

### Efectos, paletas y persistencia

```text
GET          /api/v1/effects
POST/PATCH   /api/v1/effects/startup/save
POST/PATCH   /api/v1/effects/sequence/add
POST/PATCH   /api/v1/effects/sequence/delete
GET          /api/v1/palettes
POST/PATCH   /api/v1/palettes/apply
POST/PATCH   /api/v1/palettes/save
POST/PATCH   /api/v1/palettes/delete
```

### Sincronizacion y perfiles

```text
GET            /api/v1/sync/state
GET            /api/v1/sync/connected
GET/PATCH/POST /api/v1/sync/config
PATCH/POST     /api/v1/sync/mode
GET            /api/v1/profiles
GET            /api/v1/profiles/get?id=<id>
POST/PATCH     /api/v1/profiles/save
POST/PATCH     /api/v1/profiles/apply
POST/PATCH     /api/v1/profiles/default
POST/PATCH     /api/v1/profiles/delete
POST/PATCH     /api/v1/profiles/clone
```

## 6. Hallazgos para fases posteriores

### H1 - Errores de estado no suficientemente diferenciados

`CoreState::applyPatchJson()` devuelve `false` cuando el JSON no se puede deserializar y tambien cuando el patch no cambia el estado. La ruta HTTP puede responder `updated: false` en ambos casos.

Accion para F1:

- Separar parseo, validacion, aplicacion y deteccion de cambios.
- Devolver un error identificable para JSON invalido.
- Rechazar efectos, paletas y valores invalidos cuando proceda.
- Mantener el resultado `updated: false` solo para un patch valido sin cambios.

### H2 - OpenAPI necesita validacion contra implementacion

Existe `/api/v1/openapi.json` y documentacion en `docs/wiki/API-v1.md`, pero F1 debe comprobar que cada ruta, metodo, parametro y respuesta coincide con el comportamiento real.

### H3 - La paridad funcional no equivale a paridad visual

El cliente externo no necesita copiar el HTML de `home`, pero si debe poder representar todas sus capacidades a partir de:

- `GET /api/v1/state`.
- `GET /api/v1/effects`.
- `GET /api/v1/palettes`.
- Configuraciones y metadatos de hardware.

### H4 - La API debe sobrevivir sin filesystem full

La validacion de `core` debe comprobar que ninguna ruta API depende accidentalmente de HTML, CSS, JavaScript, traducciones o navegacion local.

## 7. Baseline de filesystem actual

Medicion local realizada el 2026-09-16 sobre `data/`:

| Medida | Valor |
|---|---:|
| Archivos totales | 28 |
| Bytes totales | 215885 |
| Archivos HTML | 25 |
| Bytes HTML | 175027 |
| Archivos JavaScript | 1 |
| Bytes JavaScript | 5421 |
| Archivos JSON | 2 |
| Bytes JSON | 35437 |

Estos datos son el baseline del contenido fuente. Todavia no existe una imagen LittleFS `core` con la que compararlo.

## 8. Baseline reproducible de firmware

`esp32dev` se compilo correctamente el 2026-09-16 usando la instalacion local de PlatformIO.

| Entorno | Estado | Flash programa | RAM estatica | Uso flash | Uso RAM |
|---|---|---:|---:|---:|---:|
| `esp32dev` | Success | 1308501 bytes | 59980 bytes | 41.6% de 3145728 | 18.3% de 327680 |
| `esp32c3supermini` | Success | 1287182 bytes | 52780 bytes | 40.9% de 3145728 | 16.1% de 327680 |
| `esp32s3` | Success | 1252528 bytes | Pendiente de salida `size` | Pendiente | Pendiente |

Artefactos reproducibles registrados:

| Artefacto | Bytes |
|---|---:|
| `.pio/build/esp32dev/firmware.bin` | 1315072 |
| `.pio/build/esp32c3supermini/firmware.bin` | 1361360 |
| `.pio/build/esp32s3/firmware.bin` | 1252528 |
| `.pio/build/esp32dev/littlefs.bin` | 917504 |

Pendiente:

- [x] Recompilar `esp32dev` con el entorno actual.
- [x] Compilar `esp32c3supermini`.
- [~] Compilar `esp32s3`.
- [x] Generar `littlefs.bin` actual.
- [~] Registrar uso de flash y RAM de cada entorno.

## 9. Incidencia de entorno

El comando `pio` no estaba disponible directamente en el PATH. Se encontro y utilizo la instalacion local:

```text
C:\Users\duxma\.platformio\penv\Scripts\pio.exe
```

Los comandos inicialmente intentados fueron:

```text
pio run -e esp32dev
pio run -e esp32c3supermini
pio run -e esp32s3
```

El modulo tampoco esta instalado en el Python por defecto:

```text
python -m platformio --version
py -m platformio --version
```

Resultado: `No module named platformio`.

La auditoria funcional queda completada con evidencia de codigo. La compilacion se pudo desbloquear con el ejecutable del entorno PlatformIO local.

## 10. Criterio de cierre F0

- [x] Inventariar las acciones de `home`.
- [x] Asociar las acciones de `home` con endpoints.
- [x] Inventariar campos del estado visual.
- [x] Inventariar las rutas API actuales.
- [x] Identificar huecos de contrato para F1.
- [x] Medir el contenido fuente actual de `data/`.
- [x] Obtener baseline reproducible de compilacion.
- [x] Obtener baseline reproducible de LittleFS.
- [ ] Obtener baseline reproducible de RAM.

F0 queda en estado `[~]` hasta completar las mediciones de PlatformIO.

## Documentos relacionados

- [Plan general core/full](CORE_FULL_BUILD_PLAN.md)
- [API REST v1](wiki/API-v1.md)
- [Esquema de configuracion](wiki/Configuration-Schema.md)
- [Home embebido](../data/ui/home.html)
- [Servicio API](../firmware/src/api/ApiService.cpp)