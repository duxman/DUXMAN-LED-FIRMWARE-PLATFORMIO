# API REST v1

Version del firmware: v0.6.3-alpha
Base HTTP: `/api/v1`

Los metodos PATCH y POST se aceptan como equivalentes en todos los endpoints de mutacion.

## Respuestas y errores

Las mutaciones exitosas devuelven HTTP `200` y un objeto con `updated`:

```json
{
	"updated": false,
	"state": {}
}
```

`updated: false` significa que el JSON era valido y no modifico el estado. Los errores de entrada devuelven HTTP `400` con un codigo estable:

```json
{"error":"invalid_payload"}
{"error":"invalid_json"}
{"error":"invalid_parameter"}
```

- `invalid_payload`: cuerpo ausente o vacio.
- `invalid_json`: cuerpo no parseable como JSON.
- `invalid_parameter`: JSON parseable, pero no es un objeto de parametros aceptable.

En `/state`, los valores numericos se limitan a estos rangos: `brightness` 0..255, `sectionCount` 1..10, `effectSpeed` 1..100, `effectLevel` 1..10 y `effectTransitionMs` 0..1500. `effectTransitionStyle` acepta `fade` o `wipe`.

`GET /openapi.json` publica estos mismos rangos y enumeraciones en los schemas `State`, `StatePatch`, `StateResponse` y `ErrorResponse`. El cliente puede usar `StatePatch` como cuerpo parcial para `PATCH` o `POST /state`.

Para `GET/PATCH/POST /config/network`, OpenAPI publica los schemas `NetworkConfig`, `NetworkIpConfig`, `NetworkStaConfig` y `NetworkResponse`. Los modos admitidos son `ap`, `sta`, `ap_sta`, `dhcp`, `static`, `always` y `untilStaConnected`; las validaciones de IP, hostname y NTP devuelven códigos `invalid_*` documentados en `ErrorResponse`.

Para `GET/PATCH/POST /config/microphone`, publica `MicrophoneConfig` y `MicrophoneResponse`. `fftSize` admite `256`, `512`, `1024` o `2048`; `sampleRate` admite `8000..48000`; y los perfiles son `DEFAULT` y `gledopto_gl_c_017wl_d`.

Para `GET/PATCH/POST /config/general`, publica `GeneralConfig` y `GeneralResponse`. `language` admite `en`, `es`, `fr`, `de` e `it`; `heartbeatMs` admite `0..600000`. `/config/debug` permanece como alias legacy de esta configuración.

Para `GET/PATCH/POST /config/gpio`, publica `GpioConfig` y `GpioResponse`. Admite hasta 4 salidas, pines `-1..48`, `ledCount` `1..1500`, los tipos LED documentados en [Configuration-Schema](./Configuration-Schema) y parámetros de potencia, tensión, temperatura y dimming con sus rangos OpenAPI.

Para `GET/PATCH/POST /sync/config`, publica `SyncConfig` y `SyncConfigResponse`. Los modos son `off`, `local_effects`, `ledfx_realtime` y `cluster_sync`; los protocolos de entrada son `ddp` y `e131`; `e131UniverseCount` admite `1..9` y `sourceTimeoutMs` `100..60000`. `/sync/mode` usa `SyncModeRequest` y `SyncModeResponse`.

## Estado y sistema

| Metodo | Ruta | Descripcion |
|---|---|---|
| GET | `/state` | Estado runtime completo |
| PATCH, POST | `/state` | Actualizar estado (power, brightness, effectId, speed...) |
| POST | `/system/restart` | Reiniciar dispositivo |
| GET | `/diag` | Diagnostico de memoria y sistema |

## Configuracion

| Metodo | Ruta | Descripcion |
|---|---|---|
| GET | `/config/network` | Red actual |
| PATCH, POST | `/config/network` | Actualizar red |
| GET | `/config/microphone` | Configuracion microfono |
| PATCH, POST | `/config/microphone` | Actualizar microfono |
| GET | `/config/gpio` | Configuracion GPIO/LED |
| PATCH, POST | `/config/gpio` | Actualizar GPIO/LED |
| GET | `/config/general` | Configuracion general (idioma, region, debug) |
| PATCH, POST | `/config/general` | Actualizar configuracion general |
| GET | `/config/debug` | Configuracion debug |
| PATCH, POST | `/config/debug` | Actualizar debug |
| GET | `/config/all` | Toda la configuracion en un solo JSON |
| POST | `/config/all` | Aplicar configuracion completa |

`/config/all` usa el esquema canónico documentado en [Configuration-Schema](./Configuration-Schema).

## Sincronizacion

| Metodo | Ruta | Descripcion |
|---|---|---|
| GET | `/sync/state` | Estado del stack de sync |
| GET | `/sync/connected` | Nodos conectados |
| GET | `/sync/config` | Configuracion de sync |
| PATCH, POST | `/sync/config` | Actualizar configuracion de sync |
| PATCH, POST | `/sync/mode` | Cambiar modo (master/slave/standalone) |

Guia de interoperabilidad y ejemplos listos para LedFx/WLED/E1.31/MQTT: [Sync-Compatibility](./Sync-Compatibility).

## Perfiles

| Metodo | Ruta | Descripcion |
|---|---|---|
| GET | `/profiles` | Listar todos los perfiles |
| GET | `/profiles/get?id=<id>` | Obtener perfil por id |
| POST, PATCH | `/profiles/save` | Guardar o actualizar |
| POST, PATCH | `/profiles/apply` | Aplicar perfil activo |
| POST, PATCH | `/profiles/default` | Establecer perfil de arranque |
| POST, PATCH | `/profiles/delete` | Eliminar perfil de usuario |
| POST, PATCH | `/profiles/clone` | Clonar perfil existente |

> Nota: las rutas antiguas `/api/v1/profiles/gpio*` son legacy; usar `/api/v1/profiles*`.

## Efectos y secuencias

| Metodo | Ruta | Descripcion |
|---|---|---|
| GET | `/effects` | Listar efectos disponibles |
| POST, PATCH | `/effects/startup/save` | Guardar efecto de arranque |
| POST, PATCH | `/effects/sequence/add` | Agregar efecto a secuencia |
| POST, PATCH | `/effects/sequence/delete` | Eliminar efecto de secuencia |

## Paletas

| Metodo | Ruta | Descripcion |
|---|---|---|
| GET | `/palettes` | Listar todas las paletas |
| POST, PATCH | `/palettes/apply` | Aplicar paleta |
| POST, PATCH | `/palettes/save` | Guardar/editar paleta de usuario |
| POST, PATCH | `/palettes/delete` | Eliminar paleta de usuario |

## Metadatos del sistema

| Metodo | Ruta | Descripcion |
|---|---|---|
| GET | `/hardware` | Info de placa y pines |
| GET | `/release` | Version del firmware |
| GET | `/openapi.json` | Esquema OpenAPI completo |

## Notas de implementacion

- `POST /config/network` y `POST /config/all` responden antes de reaplicar WiFi para evitar `ERR_CONNECTION_RESET`.
- `GET /config/all` genera el JSON completo con bajo pico de memoria.
- Los comandos Serial son equivalentes 1:1 a los HTTP.
