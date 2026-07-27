# Arquitectura del firmware

Version: v0.6.3-alpha

## Diseno general

- Firmware C++ modular (PlatformIO, Arduino framework)
- FreeRTOS con dos tareas principales (control y render)
- API HTTP (puerto 80) + comandos Serial equivalentes
- Persistencia en LittleFS con scheduler diferido
- UI embebida servida desde LittleFS con fallback a HTML embebido

## Tareas FreeRTOS

| Tarea | Core | Periodo | Responsabilidades |
|---|---|---|---|
| controlTask | core0 | ~10 ms | API, WiFi, audio, persistencia |
| renderTask | core1 | ~16 ms | EffectManager.renderFrame() (~62.5 FPS) |

## Servicios activos en arranque

- `CoreState` — estado central de runtime
- `StorageService` — lectura/escritura en LittleFS
- `PersistenceSchedulerService` — guardado diferido y agrupado
- `ProfileService` — gestion de perfiles completos de dispositivo
- `UserPaletteService` — paletas personalizadas del usuario
- `WifiService` — gestion de modos AP/STA y reconexion
- `AudioService` — pipeline I2S y FFT para efectos reactivos
- `EffectPersistenceService` — persistencia de efectos y secuencias
- `EffectManager` — seleccion, configuracion y render de efectos
- `SyncService` — sincronizacion multi-nodo (DDP, E1.31, cluster)
- `ApiService` — API HTTP y puente de comandos Serial
- `WatchdogService` — vigilancia de tareas criticas

## Motor LED

Cadena de render:

```
EffectManager -> EffectEngine -> LedDriver -> backend
```

Backends seleccionables en compilacion:

| Backend | Descripcion |
|---|---|
| NeoPixelBus | Principal: DMA/I2S/RMT |
| FastLED | Alternativo |
| Digital | Salida digital simple |

## Modelos de configuracion

- `NetworkConfig` — WiFi, IP (AP/STA), DNS, NTP
- `GpioConfig` — hasta 4 salidas LED independientes
- `GpioConfig.power` — limitacion de consumo + optimizacion de voltaje/termica
- `MicrophoneConfig` — I2S, sample rate, fftSize, gain, noise floor, pins
- `GeneralConfig` — idioma, region, debugEnabled, heartbeatMs
- `SyncConfig` — modo de sync, protocolo, puertos y timing

## Persistencia

Entidades guardadas en LittleFS:

- Config activa unificada en `/config.json` (network/gpio/microphone/general/sync)
- Perfil de arranque y perfiles de usuario
- Paletas de usuario
- Estado runtime y datos de efectos/secuencias

## Sincronizacion (stack S1-S6)

El stack de sincronizacion multi-nodo esta completado:

- S1-S2: DDP y E1.31 pixel streaming
- S3: cluster sync de estado entre nodos
- S4: reloj de fase compartido para efectos sincronizados
- S5: banner de conexion y telemetria runtime
- S6: configuracion de sync por API (`/api/v1/sync/*`)

## UI embebida

Paginas principales servidas desde LittleFS (`data/ui`), con fallback automatico al HTML embebido si falta algun archivo.

Rutas principales:

- `/` — control principal
- `/config`, `/config/network`, `/config/microphone`, `/config/gpio`, `/config/sync`, `/config/profiles`, `/config/palettes`, `/config/debug`, `/config/manual`
- `/api` — testers de endpoints
- `/version`
- `/docs` — ayuda integrada

## Referencias

- API completa: [API-v1](./API-v1)
- Perfil GPIO detallado: [GPIO-Profiles](./GPIO-Profiles)
