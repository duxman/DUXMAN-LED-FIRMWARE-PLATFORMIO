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
