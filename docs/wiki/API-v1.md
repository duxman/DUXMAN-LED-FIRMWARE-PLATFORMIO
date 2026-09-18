# API REST v1

Version del firmware: v0.6.3-alpha
Base HTTP: `/api/v1`

Los metodos PATCH y POST se aceptan como equivalentes en todos los endpoints de mutacion.

## Respuestas y errores

Las mutaciones exitosas devuelven HTTP `200` y `ok: true`. Cuando la operacion
puede no cambiar el estado, tambien devuelven `updated`:

```json
{"ok":true,"updated":false,"state":{}}
```

Los errores de entrada devuelven HTTP `400`, `ok: false` y un codigo estable:

```json
{"ok":false,"error":"invalid_parameter"}
```

Los codigos principales son `invalid_payload` para un cuerpo ausente,
`invalid_json` para JSON no parseable e `invalid_parameter` para tipos, rangos,
enumeraciones, efectos, paletas o colores no validos. Los valores fuera de
rango no se recortan automaticamente y no producen cambios parciales.

En `/state`, los rangos validos son `brightness` 0..255, `sectionCount` 1..10,
`effectSpeed` 1..100, `effectLevel` 1..10 y `effectTransitionMs` 0..1500.
`effectTransitionStyle` acepta `fade` o `wipe`; los efectos y paletas deben
existir en sus catalogos. Los colores deben ser hexadecimales de seis digitos
o valores numericos RGB validos.

Las respuestas historicas pueden conservar campos como `saved`, `deleted`,
`added` o `restart`, pero incluyen `ok: true` cuando terminan correctamente.

## Ejemplos para cliente externo

Descubrimiento y lectura:

```http
GET /api/v1/openapi.json
GET /api/v1/state
GET /api/v1/effects
GET /api/v1/palettes
```

Aplicar estado con `PATCH` o `POST`:

```http
PATCH /api/v1/state
Content-Type: application/json

{"power":true,"brightness":180,"effect":"lava_flow","effectSpeed":60}
```

Guardar el estado actual y añadirlo a una secuencia:

```http
POST /api/v1/effects/startup/save
POST /api/v1/effects/sequence/add
Content-Type: application/json

{"durationSec":30}
```

Una aplicacion externa debe tratar `ok: false` como fallo y usar `error` como
codigo estable.

## Coleccion Postman

La coleccion importable [DUXMAN-LED-API-F1.postman_collection.json](../postman/DUXMAN-LED-API-F1.postman_collection.json)
cubre todas las familias de API v1: estado, configuracion, sincronizacion,
perfiles, efectos, paletas, diagnostico, metricas y metadatos. La guia de
importacion y ejecucion esta en [docs/postman/README.md](../postman/README.md).

Configura `baseUrl` con la URL de la placa y ejecuta primero `00 -
Descubrimiento`. Las carpetas posteriores incluyen mutaciones persistentes;
`04 - API completa: configuracion` puede reaplicar la red y `07 - API
completa: paletas, metricas y sistema` contiene el reinicio del dispositivo.
Ejecuta esas operaciones solo con valores revisados y hardware de laboratorio.

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
