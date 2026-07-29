# Historial de versiones y cambios

Este documento consolida el historico de versiones migrado desde `docs/old` para mantener trazabilidad completa en el repositorio activo.

Fuentes integradas:

- `docs/old/CHANGELOG.md`
- `docs/old/RELEASE_NOTES.md`
- `docs/old/GITHUB_RELEASE_0.6.3-alpha.es.md`

## Linea de tiempo

## [0.6.6-beta] - 2026-07-29

### Clave
- `RenderMetricsService`: nuevo servicio de metricas del render loop con FPS, frame timing y heap.
- Nuevos endpoints `GET /api/v1/metrics` y `POST /api/v1/metrics/reset`.
- Infraestructura de baseline F0 completada: escenas de prueba, plantilla de captura, guia paso a paso.
- Roadmap de mejoras de efectos inspirado en analisis comparativo con WLED.

## [0.6.5-beta] - 2026-07-28

### Clave
- Fix de entrega CSS en Home: cambio de inyeccion inline a hoja de estilo externa (`/ui/common.css`) para evitar fallos por heap en plantillas grandes.
- Alineacion de versionado y metadata a `0.6.5-beta`.

## [0.6.4-beta] - 2026-07-27

### Clave
- Release de validacion del pipeline automatico de build/publicacion.
- Publicacion de artefactos por entorno y checksums en release.

## [0.6.4-alpha] - 2026-07-27

### Clave
- Automatizacion CI/CD de firmware para `esp32c3supermini`, `esp32dev`, `esp32s3`.
- Mirror local de wiki (`docs/wiki`) con scripts de pull/push y control de drift en CI.
- Contrato canonico de configuracion (`network`, `gpio`, `microphone`, `general`, `sync`).

## [0.6.3-alpha] - 2026-05-02

### Clave
- Stack de sincronizacion S1-S6 completado: DDP, E1.31/sACN, `cluster_sync`, reloj global.
- Nueva UI de sync (`/config/sync`) y nuevas rutas API de estado/configuracion sync.
- Actualizacion integral de documentacion y metadata de release.

## [0.6.2-alpha] - 2026-05-03

### Clave
- Sprint 1 de sync: `SyncConfig`, `SyncService` y endpoints `/api/v1/sync/*`.
- Integracion de la seccion `sync` en import/export de configuracion completa.

## [0.6.1-alpha] - 2026-05-02

### Clave
- Versionado global alineado a `0.6.1-alpha`.
- Endurecimiento de carga de navegacion UI y mejoras de rutas `/ui/*.html`.

## [0.6.0-alpha] - 2026-04-30

### Clave
- Sistema i18n completo en firmware y UI (EN/ES extensible).
- Nueva configuracion general (`/config/general`) con idioma/region y debug.
- Migracion conceptual `debug` -> `general` con compatibilidad transitoria.

## [0.5.0-alpha] - 2026-04-30

### Clave
- Nuevo `VoltageOptimizer` para gestion avanzada de consumo/voltaje/termica.
- Ampliacion de `PowerConfig` y validaciones de configuracion energetica.

## [0.4.2-beta] - 2026-04-30

### Clave
- Ayuda integrada en UI (`/docs`) con guias funcionales y catalogo de efectos.
- Sincronizacion de documentacion principal y wiki con estado real del firmware.

## [0.4.1-beta] - 2026-04-30

### Clave
- Migracion de plantillas UI a `data/ui` en LittleFS con fallback en firmware.
- Ajustes de latencia percibida en pipeline de audio.

## [0.4.0-beta] - 2026-04-30

### Clave
- Tres nuevos efectos audio-reactivos (`Rainbow Wave`, `Spectrum Chase`, `Section Strobe`).
- Reorganizacion de efectos por carpetas: `audio-reactive` y `visual-only`.

## [0.3.11-beta] - 2026-04-30

### Clave
- Nuevo efecto `AUDIO · Neon EQ`.
- Extensiones en `powerLimit` para control de consumo software.

## [0.3.10-beta] - 2026-04-30

### Clave
- Nuevo efecto `AUDIO · Spectrum VU` con modo multi-salida y modo segmentado.

## [0.3.9-beta] - 2026-04-30

### Clave
- Correcciones de transicion de efectos y ciclo de vida de `begin()` en drivers.

## [0.3.8-beta] - 2026-04-29

### Clave
- Refactor arquitectonico documentado y utilidades compartidas de color.
- Canonizacion documental de rutas `/api/v1/profiles*`.

## [0.3.7-beta] - 2026-04-28

### Clave
- Navegacion horizontal unificada para paginas embebidas y mejoras responsive.
- Limpieza de botones redundantes en formularios.

## [0.3.6-beta] - 2026-04-13

### Clave
- `UserPaletteService` con CRUD y persistencia de paletas de usuario.
- Nuevos endpoints para guardar/eliminar paletas y listado combinado sistema/usuario.

## [0.3.5-beta] - 2026-04-12

### Clave
- Catalogo de 12 paletas predefinidas y flujo completo de aplicacion por API/UI.

## [0.3.4-beta] - 2026-04-12

### Clave
- Efecto `audio_pulse` y separacion formal de efectos visuales/audio.
- AGC con `noise gate` para normalizacion de entrada de micro.

## [0.3.3-beta] - 2026-04-03

### Clave
- Telemetria de audio por Serial y exposicion de `audioLevel` en estado API.
- Migracion de audio I2S a `MASTER_RX` para estabilizar captura.

## [0.3.2-beta] - 2026-04-03

### Clave
- Presets visuales rapidos en UI y alineacion global de versionado.
- Introduccion inicial de changelog de releases en git.

## Notas de mantenimiento

- El detalle completo por release se mantiene en `CHANGELOG.md` y en los archivos fuente de `docs/old`.
- Este documento funciona como indice historico consolidado para consulta rapida.