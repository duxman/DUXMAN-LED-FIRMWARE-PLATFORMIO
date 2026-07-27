# DUXMAN-LED-FIRMWARE-PLATFORMIO

**v0.6.3-alpha** — Firmware modular para control LED en ESP32 con API REST/Serial, UI embebida, motor de efectos visual/audio y sincronizacion multi-nodo.

Repositorio activo y prioritario del proyecto.

## Hardware soportado

| Perfil | Board | Pin LED | LEDs por defecto |
|---|---|---:|---:|
| `esp32c3supermini` | esp32-c3-devkitm-1 | 8 | 60 |
| `esp32dev` | esp32dev | 5 | 60 |
| `esp32s3` | esp32-s3-devkitc-1 | 48 | 60 |

## Build y flash

```powershell
# Compilar
pio run -e esp32c3supermini
pio run -e esp32dev
pio run -e esp32s3

# Flash
pio run -e esp32c3supermini -t upload

# Script auxiliar (Windows)
.\tools\flash.ps1 -Profile esp32c3supermini -Action upload -Port COM6
```

## Arquitectura rapida

- **2 tareas FreeRTOS**: `controlTask` (core0, ~10 ms) y `renderTask` (core1, ~16 ms / ~62.5 FPS)
- **Servicios**: CoreState, StorageService, PersistenceSchedulerService, ProfileService, UserPaletteService, WifiService, AudioService, EffectManager, SyncService, ApiService, WatchdogService
- **UI**: plantillas HTML/CSS en LittleFS (`data/ui`) con fallback a HTML embebido
- **Backends LED**: NeoPixelBus (principal), FastLED (alternativo), Digital
- **Sync stack S1-S6**: DDP, E1.31, cluster sync, reloj de fase compartido, telemetria runtime

## API v1 (resumen)

Base: `http://<ip>/api/v1`

| Grupo | Rutas principales |
|---|---|
| Estado | `GET/PATCH /state`, `POST /system/restart`, `GET /diag` |
| Config | `GET/PATCH /config/{network,gpio,microphone,debug}`, `GET/POST /config/all` |
| Sync | `GET /sync/state`, `GET/PATCH /sync/config`, `PATCH /sync/mode` |
| Perfiles | `GET /profiles`, `/profiles/{save,apply,default,delete,clone}` |
| Efectos | `GET /effects`, `/effects/startup/save`, `/effects/sequence/{add,delete}` |
| Paletas | `GET /palettes`, `/palettes/{apply,save,delete}` |
| Sistema | `GET /hardware`, `GET /release`, `GET /openapi.json` |

Referencia completa: [wiki/API-v1](https://github.com/duxman/DUXMAN-LED-FIRMWARE-PLATFORMIO/wiki/API-v1)

## Configuracion persistida

- `NetworkConfig`: WiFi (AP/STA), IP, DNS, NTP
- `GpioConfig`: hasta 4 salidas LED + limitacion de consumo software
- `MicrophoneConfig`: I2S, sample rate, fftSize, gain, noise floor, pins
- `DebugConfig`: heartbeat, logging
- Perfiles de usuario, paletas, estado runtime y secuencias de efectos

## Estructura del repositorio

```
platformio.ini        # Perfiles de build por placa
firmware/
  src/                # Codigo fuente principal
  include/            # Cabeceras del proyecto
  config/             # Configuraciones y defaults
data/
  ui/                 # UI embebida (LittleFS)
tools/
  flash.ps1           # Script build/flash Windows
docs/
  MEMORIA_PROYECTO.md
  CONTEXT_TRANSFER_FROM_MONOREPO.md
```

## Documentacion

- [Wiki del proyecto](https://github.com/duxman/DUXMAN-LED-FIRMWARE-PLATFORMIO/wiki)
- [Arquitectura](https://github.com/duxman/DUXMAN-LED-FIRMWARE-PLATFORMIO/wiki/Architecture)
- [API v1 completa](https://github.com/duxman/DUXMAN-LED-FIRMWARE-PLATFORMIO/wiki/API-v1)
- [Roadmap](https://github.com/duxman/DUXMAN-LED-FIRMWARE-PLATFORMIO/wiki/Roadmap)
- Memoria tecnica: `docs/MEMORIA_PROYECTO.md`
- Checklist serial C3: `firmware/CHECKLIST_SERIAL_ESP32C3.md`

## Repos relacionados

| Proyecto | Estado |
|---|---|
| [DUXMAN-LED-ANDROID-APP](https://github.com/duxman/DUXMAN-LED-ANDROID-APP) | Activo |
| [DUXMAN-LED-MOCKUP-PYTHON](https://github.com/duxman/DUXMAN-LED-MOCKUP-PYTHON) | Activo |
| [DUXMAN-LED-NEXT](https://github.com/duxman/DUXMAN-LED-NEXT) | Deprecado / archivado |
