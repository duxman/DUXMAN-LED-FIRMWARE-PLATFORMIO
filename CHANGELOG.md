# Changelog

All notable changes to this project are documented in this file.

The format is based on Keep a Changelog.

## [Unreleased]

## [1.0.0-beta1] - 2026-11-08

### Added
- Debug REST and Serial logging endpoints (`/api/v1/marcalog` and `/api/v1/debug/log`) to write arbitrary debug marks and messages directly to the serial console.
- Enhanced serial heartbeat telemetry (`[hb] alive ms=... heap=... fps=... avgUs=...`) when `debugEnabled=true`.
- PlatformIO custom serial monitor filter (`monitor/filter_log2file.py` and `platformio.ini`) to log serial output directly to `log/device-monitor-*.log` and `log/device-monitor-latest.log`.
- Execution and results logging to `log/test-api-*.log` and `log/test-api-latest.log` across Python (`tools/test-api.py`) and PowerShell (`tools/test-api.ps1`) test runners.
- Updated Postman collection (`DUXMAN-LED-API-F1.postman_collection.json`) with tests for `/api/v1/marcalog` via POST (JSON) and GET (query parameters).
- Automated end-to-end API integration test suites in Python (`tools/test-api.py`) and PowerShell (`tools/test-api.ps1`) with interactive user prompts (Web UI and LedFx streaming) and serial marcalog synchronization.
- Interactive embedded technical documentation wiki in the web portal (`web/index.html`) covering all API schemas, architecture, and hardware configurations.

### Changed
- Automatic local audio and effect suppression when in active client sync mode (`role: "client"` and `sourceAlive: true`): `AudioService` is suspended and local `EffectManager` execution is bypassed to prevent room noise and local effects from interfering with LedFx or master node streaming. When in server/master mode (`role: "server"`), local microphone and effects remain fully active.
- Replaced deprecated Arduino `I2S_COMM_FORMAT_I2S_MSB` with `I2S_COMM_FORMAT_STAND_I2S` and excluded obsolete Arduino `I2S` framework library via `lib_ignore` in `platformio.ini` to ensure zero compilation warnings.

## [0.6.8-beta] - 2026-11-07

### Added
- Native LedFx DDP (Distributed Display Protocol) real-time streaming support verified on hardware at 68+ FPS.

### Fixed
- Fixed LedFx DDP packet rejection in `SyncService` by adding support for standard DDP data types (`0x00` default and `0x01` RGB24) alongside legacy `0x0B`.
- Fixed DDP frame dropping when packet payload length exceeds physical strip size by clamping to available buffer instead of dropping the entire frame.

### Validation
- End-to-end LedFx DDP streaming verified on hardware (`esp32dev`) with 7,400+ consecutive packets applied, 0 packets dropped, and 68.7 input FPS.

## [0.6.7-beta] - 2026-11-07

### Added
- Changelog governance with required `CHANGELOG.md` updates in pull requests.
- Release pipeline publishes notes from versioned changelog sections.
- Fast operational release checklist in `docs/RELEASE_CHECKLIST.md`.
- F1 API contract schemas for state, network, GPIO, microphone, general configuration and synchronization in `/api/v1/openapi.json`.
- Complete F1 API contract coverage for effects, palettes, profiles, diagnostics, metrics and full configuration import/export.
- Stable external-client examples and response envelope documentation in `docs/wiki/API-v1.md`.
- Complete importable Postman collection for all API v1 routes and F2 parity preparation in `docs/postman/DUXMAN-LED-API-F1.postman_collection.json`.
- mDNS service advertisement for sync discoverability: `_ddp._udp` (LedFx), `_e131._udp`, `_duxsync._udp`, `_http._tcp`.
- Shared `EffectCanvas1D` layer for reusable per-output 1D rendering primitives (clear/set/get/add/fade/blur/flush).

### Changed
- Control task stack increased to 8192 bytes for safe headroom during large JSON generation.
- Split OpenAPI schema and route construction into modular functions in `ApiService.cpp`, reducing function frame size from 14.1 KB to 1.6 KB.
- `/api/v1/state` now distinguishes invalid JSON, invalid parameters and valid patches with no changes.
- API wiki documents response envelopes, validation ranges, enumerations and synchronization/configuration schemas.
- HTTP mutation responses now expose `ok: true` on success and `ok: false` with a stable `error` code on failure, while preserving legacy fields such as `saved`, `deleted`, `added` and `restart`.
- State patches reject invalid types, ranges, effects, palettes, transition styles and colors instead of silently coercing them.
- F1 and F2 completion recorded in `docs/CORE_FULL_BUILD_PLAN.md`.
- Sync UI now separates LedFx external input usage from cluster sync usage to avoid ambiguous role/mode combinations.
- F2 primitives/canvas rollout completed across visual-only effects (`fixed`, `gradient`, `blink_*`, `breath_*`, `diagnostic`, `lava_flow`, `polar_ice`, `random_color_pop`, `gradient_meteor`, `scanning_pulse`, `triple_chase`) for a unified frame pipeline.
- Standardized periodic serial audio telemetry to 1 line per second under `debugEnabled`.
- Firmware version markers bumped to `0.6.7-beta` (`platformio.ini`, `release-info.json`).

### Fixed
- Fixed FreeRTOS stack overflow in `dux-control` task when serving `GET /api/v1/openapi.json`.
- Fixed audio dynamic range compression by isolating the physical mono I2S channel (`i = 0, 2, 4...`) and removing DC offset per channel in `AudioService::calculateRMS()`, ignoring floating-bus noise spikes ($\pm 32768$).
- Calibrated perceptual acoustic scale reference (`kAudioScaleRef = 24.0f`) and capped AGC baseline to 100 in `AudioService`, unlocking full dynamic response (0..255) for ambient voice, typing, whistling, and music.

### Validation
- On esp32dev, OpenAPI stack overflow resolved and verified across multiple consecutive requests with 62 FPS and stable heap (>154 KB).
- Phase F2 (home UI -> API parity) fully validated against real hardware: power on/off, brightness, effect selection, speed, sections, transition params, custom colors, palette application, startup effect persistence, sequence add/delete, and reboot recovery.
- Audio capture and beat detection verified in real hardware with Gledopto GL-C-017WL MEMS I2S microphone across silence (0/255), typing/voice (40..100/255), and whistling/peaks (180..237/255 with `beat=YES`).
- Pending manual validation in real LedFx environment for mDNS auto-discovery flow (`_ddp._udp` / `_e131._udp`) and end-to-end frame ingestion.

### Docs
- New sync compatibility guide for LedFx/WLED/E1.31/MQTT at `docs/wiki/Sync-Compatibility.md`.
- Roadmap updated in `docs/IMPLEMENTACION_EFECTOS_WLED_ROADMAP.md` reflecting F2 state and latest field metrics.
- Updated root README with version `0.6.7-beta`, F2 parity status, and F3 LittleFS separation notes.

## [0.6.6-beta] - 2026-07-29

### Added
- `RenderMetricsService` — nuevo servicio de metricas del render loop: FPS (ventana deslizante 1 s), frame time min/max/avg (microsegundos via `esp_timer`), heap libre, stack headroom del renderTask.
- Endpoints `GET /api/v1/metrics` y `POST /api/v1/metrics/reset` accesibles por HTTP y Serial.
- Infraestructura de baseline F0 en `docs/baseline/`: escenas de prueba (`test-scenes.json`), plantilla de captura (`BASELINE_CAPTURE_TEMPLATE.md`) y guia paso a paso (`HOW-TO-BASELINE.md`).
- Analisis comparativo WLED vs motor de efectos propio con roadmap de mejoras en `docs/IMPLEMENTACION_EFECTOS_WLED_ROADMAP.md`.

### Changed
- `renderTask` en `main.cpp` instrumentado con `gRenderMetrics.frameBegin()` / `frameEnd()` para medir el ciclo real de render.
- Firmware version markers bumped to `0.6.6-beta` (`platformio.ini`, `release-info.json`).

### Docs
- Updated root README and wiki Home/Release-History version references to `0.6.6-beta`.
- F0 del roadmap de mejoras de efectos marcado como completado.

## [0.6.5-beta] - 2026-07-28

### Changed
- Firmware version markers bumped to `0.6.5-beta` (`platformio.ini`, `release-info.json`).
- Shared UI CSS injection changed from large inline replacement to external stylesheet link (`/ui/common.css`) to reduce heap pressure on large templates.

### Fixed
- Home page no longer renders literal `__CSS__` marker when `home.html` is loaded from LittleFS.

### Docs
- Updated root README and wiki Home version references to `0.6.5-beta`.
- Added release-history timeline entry for `0.6.5-beta` and documented the Home CSS delivery fix.

## [0.6.4-beta] - 2026-07-27

### Added
- Beta validation release for automated artifact generation and publication pipeline.

### Changed
- Firmware release marker updated from alpha to beta for first release workflow validation.

### Notes
- Includes all changes listed under `0.6.4-alpha`.

## [0.6.4-alpha] - 2026-07-27

### Added
- Automated firmware CI/CD workflow for `esp32c3supermini`, `esp32dev`, `esp32s3`.
- Automatic release artifacts (`.zip`) and `SHA256SUMS.txt` on tag/release events.
- Local wiki mirror tooling with sync scripts and VS Code tasks.
- Wiki sync drift CI check.
- Pull request template with wiki sync checklist.
- Canonical configuration schema document at `docs/wiki/Configuration-Schema.md`.

### Changed
- Unified canonical configuration contract to `network`, `gpio`, `microphone`, `general`, `sync`.
- API and storage flows aligned to the canonical general configuration schema.
- Root README updated with CI/CD and canonical configuration documentation.

### Docs
- Updated wiki pages: API-v1, Architecture, GPIO-Profiles, Home.

## [0.6.3-alpha] - 2026-07-23

### Notes
- Baseline firmware version in `platformio.ini` before changelog enforcement.

## Historical Releases (Imported)

Legacy release history from monorepo era was consolidated from:

- `docs/old/CHANGELOG.md`
- `docs/old/RELEASE_NOTES.md`
- `docs/old/GITHUB_RELEASE_0.6.3-alpha.es.md`

Quick index (see full summary in `docs/wiki/Release-History.md`):

- `0.6.3-alpha` (2026-05-02): Sync stack S1-S6 completed (DDP, E1.31/sACN, cluster sync, shared clock).
- `0.6.2-alpha` (2026-05-03): Initial sync sprint with `SyncConfig`, `SyncService`, and `/api/v1/sync/*` endpoints.
- `0.6.1-alpha` (2026-05-02): UI navigation hardening and global version alignment.
- `0.6.0-alpha` (2026-04-30): Full i18n and move from debug config semantics to general config.
- `0.5.0-alpha` (2026-04-30): `VoltageOptimizer` and advanced power/thermal config.
- `0.4.2-beta` to `0.3.2-beta` (2026-04): UI migration to LittleFS templates, new audio-reactive effects, palette services, and API/architecture stabilization.
