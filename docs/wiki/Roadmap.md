# Roadmap y evolucion

## Completado en v0.6.3-alpha

- Configuracion persistente de red, debug y GPIO/LEDs en LittleFS
- API HTTP/Serial versionada (`/api/v1/*`) con UI embebida en LittleFS
- Perfiles de placa: `esp32c3supermini`, `esp32dev`, `esp32s3`
- Particion `huge_app` (3 MB app, 960 KB LittleFS, sin OTA dual)
- Motor de efectos robusto validado en hardware real
- Editor visual de paletas de usuario (CRUD completo)
- Perfiles GPIO completos, aplicables en caliente
- Stack de sincronizacion S1-S6 completo:
  - DDP y E1.31 pixel streaming
  - Cluster sync entre nodos
  - Reloj de fase compartido para efectos coordinados
  - Banner de conexion y telemetria runtime
- Pipeline de audio afinado: menor buffering I2S, respuesta mas reactiva
- Ayuda integrada en UI desde `/docs`
- Separacion del firmware en repositorio independiente

## Fases siguientes

- OTA seguro con rollback
- Integracion con LedFx y fuentes audio-reactive externas
- Editor avanzado de efectos y presets por JSON
- Segmentos virtuales configurables por API
- Tests automatizados de regresion de API y arranque
- Soporte ampliado para hardware y sensores

## Decisiones tecnicas consolidadas

- Arquitectura no bloqueante: FreeRTOS, tareas separadas control/render
- NeoPixelBus como backend LED principal (DMA/I2S/RMT)
- API REST versionada, validada y con OpenAPI
- UI en LittleFS con fallback a HTML embebido
- Persistencia diferida via scheduler para reducir escrituras en flash

Para detalles de implementacion ver [Architecture](./Architecture) y [API-v1](./API-v1).
