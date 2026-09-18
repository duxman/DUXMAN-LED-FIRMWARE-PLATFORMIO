# Plan de perfiles `core` y `full`

Este documento define la arquitectura, el contrato funcional, las fases de trabajo y los criterios de validacion para crear una version del firmware sin interfaz grafica completa.

La version `core` tendra una interfaz local minima para configurar la red. El control normal del dispositivo se realizara desde un cliente externo, por ejemplo una aplicacion Java/Spring Boot.

La version `full` conservara la experiencia actual con la interfaz web embebida completa.

## Estado del documento

- Estado: F0 en progreso
- Implementacion: F0 parcialmente completada
- Fecha de creacion: 2026-09-16
- Responsable:
- Version objetivo:

## Convenciones de estado

- [ ] Pendiente
- [~] En progreso
- [x] Implementado
- [!] Bloqueado

## 1. Objetivos

### Objetivo principal

Permitir seleccionar durante la compilacion si se genera un firmware `core` o `full`, reduciendo el espacio utilizado por la interfaz web local sin perder capacidades de control ni configuracion.

### Objetivos concretos

- Mantener toda la API REST disponible en `core`.
- Permitir controlar desde un cliente externo todo lo que actualmente permite `home`.
- Mantener configurables todos los parametros del dispositivo mediante API.
- Incluir en `core` solo una pagina minima de configuracion de red.
- Excluir del filesystem `core` los HTML, CSS, JavaScript, traducciones y documentacion full.
- Excluir del binario los builders, rutas y fallbacks HTML que no sean necesarios en `core`.
- Mantener `full` compatible con el comportamiento actual.
- Documentar el contrato API, la matriz de builds, las pruebas y el proceso de release.

### Fuera de alcance inicial

- Crear ahora el cliente Java/Spring Boot.
- Cambiar el motor de efectos.
- Eliminar servicios funcionales como audio, sincronizacion, perfiles o paletas.
- Crear una segunda variante que elimine funcionalidades del firmware.

La primera iteracion reduce la interfaz local, no la funcionalidad del dispositivo.

## 2. Situacion actual

### Seleccion de placa

La configuracion actual de PlatformIO ya define entornos por placa y backend LED en [platformio.ini](../platformio.ini):

- `esp32c3supermini`
- `esp32dev`
- `esp32s3`
- Variantes de backend NeoPixelBus, FastLED y Digital.

### Interfaz actual

La UI embebida se encuentra en `data/ui` y se genera en una imagen LittleFS. Ademas, [ApiService.cpp](../firmware/src/api/ApiService.cpp) contiene builders y fallbacks HTML.

Esto implica dos trabajos independientes:

1. Seleccionar un filesystem minimo para `core`.
2. Excluir mediante preprocesador las rutas y funciones HTML full del binario `core`.

Eliminar solamente archivos de `data/` no es suficiente para reducir el firmware porque los fallbacks HTML seguirian compilados.

### Configuracion de red existente

La configuracion actual ya soporta:

- WiFi AP, STA y AP+STA.
- DHCP e IP estatica.
- Direccion IP.
- Gateway.
- Mascara de red.
- DNS primario y secundario.
- SSID y password.
- Disponibilidad del AP.
- Hostname y mDNS.
- NTP.

La version `core` debe reutilizar esta logica, no duplicarla.

## 3. Regla funcional principal

`core` no significa firmware limitado. Significa firmware sin UI local completa.

El siguiente principio es obligatorio:

> Si una accion se puede realizar actualmente desde `home`, debe existir una forma documentada y funcional de realizarla desde la API en el perfil `core`.

El cliente externo debe poder sustituir completamente a `home`.

## 4. Capacidades que debe cubrir la API

### 4.1 Control de estado y efectos

La API debe permitir leer y modificar:

- Encendido/apagado (`power`).
- Brillo (`brightness`).
- Efecto por ID o clave (`effectId`, `effect`).
- Numero de secciones (`sectionCount`).
- Velocidad (`effectSpeed`).
- Nivel/intensidad (`effectLevel`).
- Duracion de transicion (`effectTransitionMs`).
- Tipo de transicion (`effectTransitionStyle`).
- Paleta activa (`paletteId`, `palette`).
- Tres colores principales (`primaryColors`).
- Color de fondo (`backgroundColor`).
- Estado de audio reactivo.

Endpoints principales:

```text
GET   /api/v1/state
PATCH /api/v1/state
POST  /api/v1/state
```

Ejemplo de actualizacion:

```json
{
  "power": true,
  "brightness": 180,
  "effect": "lava_flow",
  "sectionCount": 4,
  "effectSpeed": 60,
  "effectLevel": 7,
  "effectTransitionMs": 500,
  "effectTransitionStyle": "fade",
  "primaryColors": ["#ff0000", "#00ff00", "#0000ff"],
  "backgroundColor": "#000000"
}
```

### 4.2 Catalogo de efectos

La API debe exponer para cada efecto:

- ID estable.
- Clave estable.
- Nombre visible.
- Descripcion.
- Si utiliza velocidad.
- Si utiliza audio.
- Parametros aplicables.

Endpoint:

```text
GET /api/v1/effects
```

El cliente externo no debe tener que codificar una lista privada de efectos.

### 4.3 Persistencia de efectos

Debe conservarse el control que actualmente ofrece `home`:

```text
GET  /api/v1/effects
POST /api/v1/effects/startup/save
POST /api/v1/effects/sequence/add
POST /api/v1/effects/sequence/delete
```

La API debe permitir:

- Guardar el estado actual como efecto de arranque.
- Consultar el efecto de arranque.
- Anadir el estado actual a una secuencia.
- Indicar la duracion de una entrada.
- Eliminar una entrada por ID.
- Consultar la secuencia resultante.

### 4.4 Paletas

```text
GET  /api/v1/palettes
POST /api/v1/palettes/apply
POST /api/v1/palettes/save
POST /api/v1/palettes/delete
```

Debe ser posible:

- Listar paletas del sistema y de usuario.
- Aplicar por ID o clave.
- Guardar una paleta.
- Editar una paleta.
- Eliminar una paleta de usuario.
- Recibir el estado actualizado despues de aplicar.

### 4.5 Configuracion del dispositivo

La API debe conservar todas las configuraciones actuales:

```text
GET/PATCH /api/v1/config/network
GET/PATCH /api/v1/config/gpio
GET/PATCH /api/v1/config/microphone
GET/PATCH /api/v1/config/general
GET/PATCH /api/v1/config/debug
GET/PATCH /api/v1/config/sync
GET/POST  /api/v1/config/all
```

El cliente externo debe poder configurar todos los parametros, no solo la red.

### 4.6 Perfiles

```text
GET          /api/v1/profiles
GET          /api/v1/profiles/get?id=<id>
POST/PATCH   /api/v1/profiles/save
POST/PATCH   /api/v1/profiles/apply
POST/PATCH   /api/v1/profiles/default
POST/PATCH   /api/v1/profiles/delete
POST/PATCH   /api/v1/profiles/clone
```

### 4.7 Diagnostico y sistema

```text
GET  /api/v1/diag
GET  /api/v1/hardware
GET  /api/v1/metrics
POST /api/v1/metrics/reset
GET  /api/v1/release
POST /api/v1/system/restart
GET  /api/v1/openapi.json
```

### 4.8 Sincronizacion

```text
GET        /api/v1/sync/state
GET        /api/v1/sync/connected
GET/PATCH  /api/v1/sync/config
POST/PATCH /api/v1/sync/mode
```

## 5. Contrato API y calidad

Antes de declarar terminado `core`, la API debe tener un contrato estable y verificable.

### Requisitos del contrato

- Rutas y metodos documentados.
- JSON de entrada documentado.
- JSON de salida documentado.
- Rangos numericos documentados.
- Valores enumerados documentados.
- Ejemplos de uso.
- Codigos HTTP documentados.
- Errores identificables por codigo.
- Respuestas consistentes.
- Compatibilidad con clientes que solo soporten POST.

### Respuesta recomendada

Las respuestas de mutacion deberian seguir una forma comun:

```json
{
  "ok": true,
  "updated": true,
  "state": {}
}
```

Los errores deberian seguir una forma comun:

```json
{
  "ok": false,
  "error": {
    "code": "invalid_parameter",
    "field": "effectSpeed",
    "message": "Value must be between 1 and 100"
  }
}
```

### Codigos HTTP minimos

| Codigo | Uso |
|---:|---|
| 200 | Operacion correcta con respuesta |
| 204 | Operacion correcta sin cuerpo, si se decide utilizarlo |
| 400 | JSON o parametro invalido |
| 404 | Efecto, paleta, perfil o recurso inexistente |
| 405 | Metodo no permitido |
| 409 | Conflicto de estado |
| 500 | Error interno |

### Punto a verificar

`CoreState::applyPatchJson()` actualmente devuelve `false` tanto para un JSON invalido como para un patch que no cambia nada. La capa HTTP debe distinguir ambos casos para que el cliente externo pueda diagnosticar errores correctamente.

## 6. Arquitectura de builds

### Perfiles previstos

| Entorno | Variante | UI local | API | Funcionalidad |
|---|---|---|---|---|
| `esp32dev` | full | Completa | Completa | Completa |
| `esp32dev_core` | core | Red minima | Completa | Completa |
| `esp32c3supermini` | full | Completa | Completa | Completa |
| `esp32c3supermini_core` | core | Red minima | Completa | Completa |
| `esp32s3` | full | Completa | Completa | Completa |
| `esp32s3_core` | core | Red minima | Completa | Completa |

La variante debe quedar visible en los metadatos de build y en `/api/v1/release`.

### Macros de compilacion

Se recomienda una macro explicita, por ejemplo:

```text
DUX_BUILD_FLAVOR_CORE
DUX_BUILD_FLAVOR_FULL
```

O una macro numerica equivalente. Debe existir un valor por defecto seguro y documentado.

### Filesystem

La estructura propuesta es:

```text
data/
  full/
    ui/
      ... interfaz actual ...

  core/
    ui/
      network-config.html
```

El build `full` debe usar `data/full` y el build `core` debe usar `data/core`.

La seleccion puede implementarse mediante `data_dir` por entorno o mediante un `extra_script` de PlatformIO. Debe validarse con una compilacion real antes de fijar la solucion.

## 7. Interfaz local minima de `core`

La pagina local de `core` debe permitir configurar:

- Modo WiFi: AP, STA o AP+STA.
- SSID.
- Password.
- Disponibilidad del AP.
- DHCP o IP estatica.
- IP.
- Gateway.
- Mascara.
- DNS primario y secundario.
- Hostname.
- NTP, si se decide mantenerlo visible en la pagina minima.

Debe utilizar exclusivamente:

```text
GET   /api/v1/config/network
PATCH /api/v1/config/network
```

No debe depender de:

- Navegacion full.
- `i18n.js` full.
- CSS completo.
- Otras paginas HTML.

La pagina `/` de `core` debe redirigir o servir directamente la configuracion de red.

## 8. Fases de desarrollo

## F0 - Baseline y auditoria funcional

### Objetivo

Capturar el comportamiento actual antes de modificar la arquitectura.

### Tareas

- [x] Inventariar todas las acciones de `data/ui/home.html`.
- [x] Asociar cada accion con su endpoint API.
- [x] Inventariar todos los campos aceptados por cada configuracion.
- [x] Comparar API implementada contra `docs/wiki/API-v1.md`.
- [x] Capturar ejemplos reales de peticiones y respuestas.
- [x] Medir tamano actual de firmware y LittleFS por placa.
- [ ] Registrar RAM libre y minima durante uso normal.

### Criterio de cierre

- [x] Existe una matriz completa de paridad `home -> API`.
- [~] Existe un baseline de tamanos y un baseline parcial de memoria.
- [x] No quedan acciones de `home` sin endpoint identificado.

### Registro

- Fecha inicio: 2026-09-16
- Fecha cierre:
- Documento: [CORE_FULL_F0_AUDIT.md](CORE_FULL_F0_AUDIT.md)
- Notas: auditoria funcional completada. Se midieron los tres firmware y la imagen LittleFS. Queda pendiente la RAM runtime y confirmar la linea estatica de RAM de S3.

## F1 - Contrato API completo

### Objetivo

Convertir la API existente en un contrato estable para clientes externos.

### Tareas

- [x] Revisar todos los endpoints de estado, efectos, paletas y configuracion.
- [x] Completar los schemas de `/api/v1/openapi.json`.
- [x] Documentar rangos y enumeraciones del estado en la API y OpenAPI.
- [x] Unificar respuestas de exito.
- [x] Unificar respuestas de error.
- [x] Diferenciar JSON invalido, parametro invalido y operacion sin cambios en `/api/v1/state`.
- [x] Verificar compatibilidad PATCH/POST.
- [x] Actualizar `docs/wiki/API-v1.md`.
- [x] Anadir ejemplos para el cliente externo.

### Criterio de cierre

- [x] Un cliente externo puede descubrir las capacidades sin leer el codigo fuente.
- [x] Todos los errores de entrada devuelven codigo HTTP y codigo de error previsibles.
- [x] La documentacion coincide con el firmware probado.

### Registro de reanudacion F1

- Se amplio el OpenAPI embebido con `StatePatch`, respuestas comunes y las rutas
  de efectos, paletas, perfiles, diagnostico, metricas y configuracion completa.
- Todos los handlers HTTP usan `ok: true` en exito y `ok: false` con un codigo
  estable en error, conservando los campos legacy.
- La documentacion de `API-v1.md` fue publicada en el wiki remoto.
- El checker de drift puede mostrar `docs/wiki/API-v1.md` como modificado porque
  compara contra el commit del repositorio principal; el mirror remoto esta
  sincronizado tras el push.

## F2 - Pruebas de paridad con `home`

### Objetivo

Demostrar que la API puede sustituir todas las acciones de la UI actual.

### Tareas

- [ ] Probar encendido y apagado.
- [ ] Probar brillo.
- [ ] Probar seleccion de cada efecto.
- [ ] Probar velocidad y nivel.
- [ ] Probar secciones.
- [ ] Probar transiciones.
- [ ] Probar colores manuales.
- [ ] Probar color de fondo.
- [ ] Probar aplicar paleta.
- [ ] Probar guardar efecto de arranque.
- [ ] Probar anadir entrada a secuencia.
- [ ] Probar eliminar entrada de secuencia.
- [ ] Probar reinicio.
- [ ] Probar que el estado devuelto coincide con el estado aplicado.

### Criterio de cierre

- [ ] Todas las acciones de `home` tienen una prueba API automatizada o reproducible.
- [ ] No hay divergencia entre lo que muestra `home` y `GET /api/v1/state`.

## F3 - Separacion de filesystem

### Objetivo

Generar imagenes LittleFS distintas para `core` y `full`.

### Tareas

- [ ] Crear `data/full` y migrar la UI actual sin cambios funcionales.
- [ ] Crear `data/core` con solo la pagina de red minima.
- [ ] Seleccionar el directorio por entorno PlatformIO.
- [ ] Verificar que `buildfs` usa el directorio correcto.
- [ ] Confirmar que la imagen `core` no contiene HTML full.
- [ ] Documentar el comando de compilacion y subida del filesystem.

### Criterio de cierre

- [ ] `core` y `full` producen imagenes LittleFS diferentes.
- [ ] La imagen `core` no contiene `home`, documentacion, traducciones ni formularios full.
- [ ] La UI minima funciona desde la imagen `core` instalada.

## F4 - Exclusiones de codigo HTML

### Objetivo

Evitar que el binario `core` incluya builders, rutas y fallbacks HTML innecesarios.

### Tareas

- [ ] Definir macros de compilacion.
- [ ] Condicionar el registro de rutas HTML full.
- [ ] Condicionar los fallbacks HTML full.
- [ ] Mantener activas todas las rutas `/api/v1`.
- [ ] Servir la pagina de red desde `/` en `core`.
- [ ] Eliminar dependencias full no necesarias para compilar `core`.
- [ ] Confirmar que no se excluye ningun servicio funcional.

### Criterio de cierre

- [ ] El binario `core` no contiene las cadenas HTML full innecesarias.
- [ ] La API responde igual en `core` y `full`.
- [ ] `full` conserva sus rutas y paginas actuales.

## F5 - Matriz de compilacion por placa y variante

### Objetivo

Hacer reproducible la compilacion de todas las combinaciones soportadas.

### Tareas

- [ ] Crear entornos `*_core` para cada placa.
- [ ] Heredar correctamente flags de placa y backend.
- [ ] Evitar duplicacion innecesaria de configuracion.
- [ ] Exponer la variante en release info.
- [ ] Compilar todas las variantes localmente.
- [ ] Revisar warnings y tamanos.

### Criterio de cierre

- [ ] Compilan todas las variantes soportadas.
- [ ] Cada variante utiliza la placa y pin correctos.
- [ ] El backend LED no cambia accidentalmente entre `core` y `full`.

## F6 - Pruebas en hardware

### Objetivo

Validar que la reduccion de UI no afecta al funcionamiento del firmware.

### Tareas

- [ ] Arranque en AP.
- [ ] Configuracion STA con DHCP.
- [ ] Configuracion STA con IP estatica.
- [ ] Verificacion de gateway y mascara.
- [ ] Verificacion de DNS.
- [ ] Verificacion de hostname/mDNS.
- [ ] Control de efecto desde cliente externo.
- [ ] Cambio de colores y brillo.
- [ ] Persistencia tras reinicio.
- [ ] Funcionamiento de audio si esta configurado.
- [ ] Funcionamiento de sincronizacion si esta configurada.
- [ ] Funcionamiento de perfiles y paletas.

### Criterio de cierre

- [ ] El dispositivo vuelve a estar controlable despues de reiniciar.
- [ ] La aplicacion de red responde antes de que el WiFi se reinicie.
- [ ] La API no presenta regresiones respecto a `full`.

## F7 - Automatizacion y CI

### Objetivo

Evitar que futuras modificaciones rompan la paridad entre perfiles.

### Tareas

- [ ] Ampliar `firmware-build-release.yml` con variantes `core`.
- [ ] Generar artefactos de firmware y filesystem.
- [ ] Etiquetar cada artefacto con placa y variante.
- [ ] Generar checksums.
- [ ] Anadir pruebas de endpoints API.
- [ ] Verificar ausencia de archivos full en la imagen core.
- [ ] Publicar tamanos comparativos.

### Criterio de cierre

- [ ] CI compila todas las variantes.
- [ ] CI prueba la API minima y la API completa.
- [ ] Los artefactos no son ambiguos respecto a placa y variante.

## F8 - Documentacion y release

### Objetivo

Dejar el proceso completamente reproducible y documentado.

### Tareas

- [ ] Actualizar `README.md` con los comandos de build.
- [ ] Actualizar `docs/wiki/API-v1.md`.
- [ ] Crear o actualizar la referencia de arquitectura.
- [ ] Documentar la matriz de entornos.
- [ ] Documentar la configuracion de red de `core`.
- [ ] Documentar el flujo de cliente externo.
- [ ] Actualizar `CHANGELOG.md`.
- [ ] Actualizar `docs/RELEASE_CHECKLIST.md`.
- [ ] Registrar tamanos finales y ahorro conseguido.
- [ ] Preparar notas de release.

### Criterio de cierre

- [ ] Una persona nueva puede compilar, flashear y probar cualquier variante.
- [ ] La API esta documentada sin depender de la UI.
- [ ] La release identifica claramente `core` y `full`.

## 9. Matriz de paridad `home` -> API

| Accion de `home` | Endpoint | Estado esperado |
|---|---|---|
| Leer estado | `GET /api/v1/state` | Estado completo y capacidades |
| Aplicar estado | `PATCH /api/v1/state` | Estado actualizado |
| Encender/apagar | `PATCH /api/v1/state` | `power` actualizado |
| Brillo | `PATCH /api/v1/state` | `brightness` actualizado |
| Efecto | `PATCH /api/v1/state` | `effectId` y `effect` validos |
| Velocidad | `PATCH /api/v1/state` | `effectSpeed` validado |
| Nivel | `PATCH /api/v1/state` | `effectLevel` validado |
| Secciones | `PATCH /api/v1/state` | `sectionCount` validado |
| Colores | `PATCH /api/v1/state` | Colores normalizados |
| Paleta | `POST /api/v1/palettes/apply` | Paleta y estado actualizados |
| Listar efectos | `GET /api/v1/effects` | Catalogo completo |
| Listar paletas | `GET /api/v1/palettes` | Paletas del sistema y usuario |
| Guardar arranque | `POST /api/v1/effects/startup/save` | Persistencia confirmada |
| Anadir secuencia | `POST /api/v1/effects/sequence/add` | ID de entrada devuelto |
| Eliminar secuencia | `POST /api/v1/effects/sequence/delete` | Eliminacion confirmada |
| Reiniciar | `POST /api/v1/system/restart` | Respuesta antes del reinicio |

## 10. Validacion de memoria

Cada variante debe registrar como minimo:

- Tamano de `firmware.bin`.
- Tamano de `littlefs.bin`.
- Flash utilizada.
- RAM libre al arrancar.
- RAM minima durante operacion.
- Tamano de particiones.
- Tamano de la imagen filesystem.

La comparativa debe separar:

1. Ahorro por eliminar archivos LittleFS.
2. Ahorro por eliminar builders y fallbacks HTML del binario.
3. Cualquier cambio de RAM en ejecucion.

No se debe declarar exito solo porque el filesystem `core` sea menor.

## 11. Riesgos y mitigaciones

| Riesgo | Impacto | Mitigacion |
|---|---|---|
| Eliminar solo archivos de `data` | El binario sigue incluyendo HTML | Condicionar rutas y builders C++ |
| Excluir una dependencia funcional | Regresion en API o efectos | Mantener matriz de paridad y pruebas |
| Multiplicar excesivamente entornos | Configuracion dificil de mantener | Herencia de entornos y variables comunes |
| Inconsistencia OpenAPI/API real | Cliente externo incompatible | Validar documentacion contra pruebas |
| Error al aplicar red | Perdida de conexion | Responder antes de reaplicar WiFi |
| Error de validacion silencioso | Cliente no puede diagnosticar | Codigos HTTP y errores estructurados |
| Confundir `core` con firmware sin servicios | Alcance descontrolado | Mantener el alcance de esta primera iteracion |

## 12. Definition of Done

La iniciativa se considera terminada cuando:

- [ ] Existen builds `core` y `full` para cada placa soportada.
- [ ] `core` contiene solamente la UI local minima de red.
- [ ] `core` no contiene la UI full en LittleFS.
- [ ] `core` no compila rutas ni fallbacks HTML full innecesarios.
- [ ] Toda la API funcional esta disponible en `core`.
- [ ] Todas las acciones de `home` tienen equivalencia API probada.
- [ ] Efectos, velocidad, colores, brillo, paletas y secuencias funcionan desde API.
- [ ] Todos los parametros configurables estan documentados.
- [ ] Los errores de API son distinguibles y documentados.
- [ ] OpenAPI coincide con el comportamiento real.
- [ ] CI compila y valida las variantes.
- [ ] README, Wiki, changelog y checklist de release estan actualizados.
- [ ] Se conocen y documentan los tamanos finales y el ahorro conseguido.

## 13. Registro de decisiones

| Fecha | Decision | Motivo |
|---|---|---|
| 2026-09-16 | `core` reduce UI, no funcionalidad | El cliente externo debe sustituir completamente a `home` |
| 2026-09-16 | La API es el contrato principal | Permite clientes Java/Spring Boot y otros clientes |
| 2026-09-16 | Se separan filesystem y codigo HTML | Eliminar `data` por si solo no reduce todo el binario |
| 2026-09-16 | Se mantienen efectos, paletas y configuraciones | Todos los parametros deben seguir siendo configurables |

## 14. Documentos relacionados

- [API REST v1](wiki/API-v1.md)
- [Arquitectura](wiki/Architecture.md)
- [Esquema de configuracion](wiki/Configuration-Schema.md)
- [Checklist de release](RELEASE_CHECKLIST.md)
- [README principal](../README.md)
- [Configuracion PlatformIO](../platformio.ini)