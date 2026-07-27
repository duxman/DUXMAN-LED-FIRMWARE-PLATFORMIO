# API REST v1

Version del firmware: v0.6.3-alpha
Base HTTP: `/api/v1`

Los metodos PATCH y POST se aceptan como equivalentes en todos los endpoints de mutacion.

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
