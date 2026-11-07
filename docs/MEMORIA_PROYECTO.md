# Memoria del Proyecto - Firmware

## Objetivo

Controlador LED modular para ESP32 con efectos visuales/audio, API REST/Serial y UI embebida.

## Stack tecnico

- ESP32 + Arduino
- FreeRTOS (tareas separadas control/render)
- LittleFS para configuracion y UI
- PlatformIO para build y despliegue

## Pilares de arquitectura

- `CoreState`: estado central de runtime.
- `StorageService`: persistencia en LittleFS.
- `PersistenceSchedulerService`: guardado diferido y agrupado.
- `EffectManager`: seleccion y render de efectos.
- `SyncService`: sincronizacion entre nodos/controladores.
- `ApiService`: API HTTP (`/api/v1`) y puente de comandos.

## Decisiones clave

- Repositorio firmware separado para acelerar iteracion y CI.
- Meta-repo original se conserva solo para historial/documentacion.
- UI principal en archivos LittleFS (`data/ui`) con fallback embebido.

## Flujo recomendado de trabajo

1. Crear rama por feature/bugfix.
2. Compilar perfil objetivo en PlatformIO.
3. Probar endpoints API criticos y render en hardware.
4. Validar persistencia de configuracion.
5. Merge a `main` tras validacion funcional.

## Riesgos conocidos

- Limites de memoria en endpoints de payload grande.
- Dependencia de hardware para validacion final de timing/render.
- Diferencias por perfil de placa (C3/ESP32/S3).

## Prioridades inmediatas

- Estabilidad de pipeline de audio y sincronizacion.
- Robustez de configuracion/perfiles/paletas.
- Cobertura de pruebas de regresion API y arranque.

## Hitos recientes

### F1 - Contrato API completo (2026-09-18)

- OpenAPI embebido ampliado para las rutas de estado, configuracion, efectos,
	paletas, perfiles, diagnostico, metricas y configuracion completa.
- Respuestas HTTP de mutacion normalizadas con `ok`, `updated` y codigos de
	error previsibles, manteniendo campos legacy para compatibilidad.
- `CoreState` valida tipos, rangos, catalogos de efectos/paletas, estilos de
	transicion y colores antes de modificar el estado.
- Guia API y ejemplos para clientes externos publicados en la wiki del proyecto.
- Coleccion Postman completa en `docs/postman/DUXMAN-LED-API-F1.postman_collection.json`,
  con cobertura de todas las rutas API v1, aliases de mutacion y validacion de
  errores. La guia de uso esta en `docs/postman/README.md`.
- Validacion de compilacion confirmada para `esp32dev` con PlatformIO.

El siguiente trabajo funcional es F2: pruebas de paridad entre las acciones de
`home` y la API, incluyendo efectos, paletas, secuencias y persistencia.

### F2 - Pruebas de paridad con `home` completadas (2026-11-07)

- Resultado: Paridad 100% verificada en placa real (`esp32dev`).
- Casos probados y confirmados:
  - Descubrimiento (`release`, `hardware`, `openapi.json`).
  - Control de brillo (`brightness: 80`).
  - Cambio de efecto y velocidad (`lava_flow`, velocidad 50).
  - Aplicación de paleta (`ocean_neon`).
  - Validación de errores HTTP 400 (`brightness: 500` rechazado con `invalid_parameter`).
  - Persistencia de arranque en LittleFS (`effects/startup/save`).
  - Gestión de secuencia (`sequence/add` y `sequence/delete`).
  - Control de energía (`power: false` / `power: true`).
  - Configuración complementaria (`sectionCount: 2`, transición 500 ms, colores RGB manuales y fondo).
  - Reinicio por API (`system/restart`) y recuperación del efecto de arranque guardado tras boot.
- Estado: F2 cerrada. Siguiente paso: F3 (separación de filesystem para perfiles `core` y `full`).

### Diagnostico provisional de pila OpenAPI (2026-11-07)

- Decision: aumentar la pila de `controlTask` de 4096 a 8192 bytes como prueba,
	sin modificar el contrato ni el generador OpenAPI.
- Motivo: se observo un desbordamiento de `dux-control` al solicitar
	`/api/v1/openapi.json`. La causa exacta y el tamano necesario no estan medidos;
	modificar el generador se pospone hasta disponer de esa evidencia.
- Impacto: la tarea solicita 4096 bytes adicionales de memoria; no cambia el
	hardware, el pinout ni el protocolo.
- Documentacion: `README.md`, `CHANGELOG.md` y esta memoria; la actualizacion
	de Graphify queda sujeta al resultado de la herramienta.
- Riesgos y seguimiento: comprobar compilacion, heap y margen de pila, y repetir
	la peticion en hardware. Si persiste el reinicio o falta memoria, investigar
	el generador antes de ajustar de nuevo la pila. No se ha validado en placa.

La prueba en `esp32dev` siguio provocando un desbordamiento con 8192 bytes. El
desensamblado del firmware anterior mostro un marco de 14128 bytes en
`buildOpenApiJson()`. Se separo la construccion de esquemas y rutas en bloques
sin modificar intencionadamente el contrato; se mantiene la pila de 8192 bytes
para la siguiente prueba. Quedan pendientes la confirmacion de compilacion, la
medicion de las nuevas funciones, la comparacion del JSON y la prueba en placa.

Seguimiento en `esp32dev`: el usuario compilo y subio el firmware, y OpenAPI
respondio en tres solicitudes sin reinicio observado. Tras reiniciar metricas,
se registraron 62 FPS y un minimo de heap muestreado de 154528 bytes. El
desensamblado muestra marcos de 352 bytes para el generador y de 432, 752, 976
y 1664 bytes para sus bloques. Estos valores no incluyen toda la cadena de
llamadas ni miden el margen real de `dux-control`.

### Preparacion de Release v0.6.7-beta y F3 pospuesto (2026-11-07)

- **Decision tomada:** Publicar la version `0.6.7-beta` con las mejoras acumuladas (F1, F2 y correccion de OpenAPI), posponiendo la separacion de filesystem F3 (`core`/`full`) como tarea pendiente (TODO) en evaluacion.
- **Motivo:** F1 y F2 estan 100% verificadas en hardware real. Mantener dos arboles de UI independientes (`data/core` y `data/full`) introduce complejidad de sincronizacion y mantenimiento que no esta justificada mientras no se demuestre una ganancia critica de espacio flash (LittleFS actual ocupa ~215 KB de 960 KB asignados).
- **Impacto:**
  - Version incrementada a `0.6.7-beta` en `platformio.ini`, `README.md`, `CHANGELOG.md` y `firmware/config/release-info.json` (versionCode 24).
  - Tareas F3 marcadas explicitamente como TODO en `docs/CORE_FULL_BUILD_PLAN.md`.
  - Preparado el changelog para la generacion automatica de notas de release en GitHub Actions.
- **Documentacion actualizada:** `README.md`, `CHANGELOG.md`, `platformio.ini`, `release-info.json`, `docs/CORE_FULL_BUILD_PLAN.md` y esta memoria.
- **Riesgos y seguimiento:** Si en el futuro se retoma F3, medir primero con `pio run -t buildfs` la comparativa real de bytes antes de duplicar archivos.
