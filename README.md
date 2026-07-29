# DUXMAN-LED-FIRMWARE-PLATFORMIO

**v0.6.6-beta** — Firmware modular para control LED en ESP32 con API REST/Serial, UI embebida, motor de efectos visual/audio y sincronizacion multi-nodo.

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

## CI/CD de firmware

El workflow `.github/workflows/firmware-build-release.yml` automatiza:

- Build en GitHub Actions para `esp32c3supermini`, `esp32dev`, `esp32s3`.
- Generacion de artefactos `.zip` por entorno (incluye `firmware.bin` y binarios de flash cuando existen).
- Publicacion automatica de artefactos en la release al crear un tag o al publicar una release.
- Notas de release generadas desde `CHANGELOG.md` (seccion de version del tag).
- Archivo `SHA256SUMS.txt` con checksums de todos los artefactos.

Eventos que lo disparan:

- `pull_request`
- `push` a `main`
- `push` de tags
- `release` publicada
- `workflow_dispatch` manual

## Changelog y releases

- Archivo fuente: `CHANGELOG.md`.
- Historico consolidado de versiones legacy: `docs/wiki/Release-History.md`.
- Archivo de origen legacy (monorepo): `docs/old/CHANGELOG.md`.
- En cada PR se exige actualizar `CHANGELOG.md` via `.github/workflows/changelog-check.yml`.
- Checklist operativa de release: `docs/RELEASE_CHECKLIST.md`.
- Para publicar una release por tag, debe existir seccion en changelog con formato:

```markdown
## [0.6.4-alpha] - 2026-07-27
```

Si faltase esa seccion, el workflow de release falla para evitar releases sin trazabilidad.

## Arquitectura rapida

- **2 tareas FreeRTOS**: `controlTask` (core0, ~10 ms) y `renderTask` (core1, ~16 ms / ~62.5 FPS)
- **Servicios**: CoreState, StorageService, PersistenceSchedulerService, ProfileService, UserPaletteService, WifiService, AudioService, EffectManager, SyncService, ApiService, WatchdogService
- **UI**: plantillas HTML/CSS en LittleFS (`data/ui`) con fallback a HTML embebido
- **Backends LED**: NeoPixelBus (principal), FastLED (alternativo), Digital
- **Sync stack S1-S6**: DDP, E1.31, cluster sync, reloj de fase compartido, telemetria runtime
- **Descubrimiento mDNS**: `_ddp._udp` y `_e131._udp` para LedFx (entrada externa) y `_duxsync._udp` para sync interno de cluster

## API v1 (resumen)

Base: `http://<ip>/api/v1`

| Grupo | Rutas principales |
|---|---|
| Estado | `GET/PATCH /state`, `POST /system/restart`, `GET /diag` |
| Config | `GET/PATCH /config/{network,gpio,microphone,general}`, `GET/PATCH /config/debug`, `GET/POST /config/all` |
| Sync | `GET /sync/state`, `GET/PATCH /sync/config`, `PATCH /sync/mode` |
| Perfiles | `GET /profiles`, `/profiles/{save,apply,default,delete,clone}` |
| Efectos | `GET /effects`, `/effects/startup/save`, `/effects/sequence/{add,delete}` |
| Paletas | `GET /palettes`, `/palettes/{apply,save,delete}` |
| Sistema | `GET /hardware`, `GET /release`, `GET /metrics`, `POST /metrics/reset`, `GET /openapi.json` |

Referencia completa: [wiki/API-v1](https://github.com/duxman/DUXMAN-LED-FIRMWARE-PLATFORMIO/wiki/API-v1)

## Configuracion persistida

- `NetworkConfig`: WiFi (AP/STA), IP, DNS, NTP
- `GpioConfig`: hasta 4 salidas LED + bloque `power` (consumo/voltaje/termica)
- `MicrophoneConfig`: I2S, sample rate, fftSize, gain, noise floor, pins
- `GeneralConfig`: idioma, region, debugEnabled, heartbeatMs
- `SyncConfig`: modo sync, protocolo y puertos
- Perfiles de usuario, paletas, estado runtime y secuencias de efectos

Esquema completo de configuracion: `docs/wiki/Configuration-Schema.md`

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
- [Esquema de configuracion](https://github.com/duxman/DUXMAN-LED-FIRMWARE-PLATFORMIO/wiki/Configuration-Schema)
- [Roadmap](https://github.com/duxman/DUXMAN-LED-FIRMWARE-PLATFORMIO/wiki/Roadmap)
- Memoria tecnica: `docs/MEMORIA_PROYECTO.md`
- Checklist de release: `docs/RELEASE_CHECKLIST.md`
- Checklist serial C3: `firmware/CHECKLIST_SERIAL_ESP32C3.md`
- Workflow build/release: `.github/workflows/firmware-build-release.yml`

### Mirror local de Wiki

La Wiki de GitHub se gestiona en un repo separado (`<repo>.wiki.git`).

Para mantener una copia versionada en este workspace usa `tools/wiki-sync.ps1` con mirror local en `docs/wiki`:

```powershell
# Wiki remota -> docs/wiki
powershell -ExecutionPolicy Bypass -File .\tools\wiki-sync.ps1 -Mode pull

# docs/wiki -> Wiki remota
powershell -ExecutionPolicy Bypass -File .\tools\wiki-sync.ps1 -Mode push
```

Referencia completa: `tools/README.md`

Tambien tienes tasks de VS Code en `.vscode/tasks.json`:

- `Wiki: Pull to docs/wiki`
- `Wiki: Push from docs/wiki`
- `Wiki: Check sync drift`

Y validacion automatica en CI con `.github/workflows/wiki-sync-check.yml`.
Ademas, el PR template en `.github/pull_request_template.md` incluye checklist de sincronizacion de wiki.

## Repos relacionados

| Proyecto | Estado |
|---|---|
| [DUXMAN-LED-ANDROID-APP](https://github.com/duxman/DUXMAN-LED-ANDROID-APP) | Activo |
| [DUXMAN-LED-MOCKUP-PYTHON](https://github.com/duxman/DUXMAN-LED-MOCKUP-PYTHON) | Activo |
| [DUXMAN-LED-NEXT](https://github.com/duxman/DUXMAN-LED-NEXT) | Deprecado / archivado |
