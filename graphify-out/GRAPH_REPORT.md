# Graph Report - DUXMAN-LED-FIRMWARE-PLATFORMIO  (2026-11-07)

## Corpus Check
- 160 files · ~115,202 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 11 file(s) not represented in the graph (top: (none) 8, .log 2, .ini 1)

## Summary
- 1913 nodes · 3637 edges · 128 communities (98 shown, 30 thin omitted)
- Extraction: 84% EXTRACTED · 16% INFERRED · 0% AMBIGUOUS · INFERRED: 600 edges (avg confidence: 0.86)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `551e324a`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- LedDriver
- ApiService
- ProfileService
- NeoOutputBase
- Config.cpp
- EffectPersistenceService
- AudioService
- LedDriverFastLed.cpp
- Implementacion de mejoras de efectos (inspirado en WLED)
- EffectEngine.h
- with
- UserPaletteService
- SyncService
- update_headers.py
- main.cpp
- WifiService.cpp
- impl_
- LanguageManager
- CoreState.cpp
- Paso a paso completo
- PowerConfig - Advanced Voltage Optimizer Configuration
- SyncService.cpp
- EffectAudioRainbowWave
- StorageService
- F0 - Auditoria funcional y baseline
- Stats
- ColorPaletteDescriptor
- VoltageOptimizer
- I18nEngine
- EffectRegistry.cpp
- VoltageOptimizer - Software Voltage Management System
- steps
- WatchdogService
- RenderMetricsService
- EffectAudioSectionStrobe
- steps
- Servicios activos en arranque
- Arduino y PlatformIO
- Checklist de diagnostico serial (ESP32-C3)
- HardwareInfo.cpp
- Instrucciones del Proyecto y Memoria de Arquitectura
- API-v1.md
- Plan de perfiles `core` y `full`
- API REST v1
- Compatibilidad de sincronizacion (LedFx, WLED, E1.31, MQTT)
- EffectAudioSpectrumChase
- PersistenceSchedulerService
- Baseline de metricas de render — DUXMAN-LED-NEXT
- EffectBlinkFixed
- .resetE131Assembly
- LedDriverNeoPixelBus
- Changelog
- 4. Capacidades que debe cubrir la API
- Release Checklist (2 minutos)
- Arquitectura del firmware
- EffectStellarTwinkle.cpp
- EffectRandomColorPop.cpp
- .normalizedRole
- Instrucciones de Graphify para memoria arquitectónica y agentes
- Context Transfer From Monorepo
- Configuration Schema
- Stats
- Instrucciones del Proyecto y Memoria de Arquitectura
- java.instructions.md
- Registro y logging
- EffectGradientMeteor.cpp
- ApiService::ApiService
- StorageService::StorageService
- flash.ps1
- Tools
- wiki-sync.ps1
- Perfiles GPIO
- Paletas de color
- LedDriverColorOrder
- NeoOutputRgb
- EffectCanvas1D.h
- NeoOutputRgbw
- Pruebas unitarias
- Patrones de diseño
- Spring Boot 4
- pull_request_template.md
- LedDriverOutputConfig
- LedDriverType
- 5. Contrato API y calidad
- F0 - Baseline y auditoria funcional
- F1 - Contrato API completo
- F2 - Pruebas de paridad con `home`
- main.cpp
- SynchronizedClockState
- Java en este repositorio
- Pruebas externas y de carga
- 1. Objetivos
- 2. Situacion actual
- 6. Arquitectura de builds
- F3 - Separacion de filesystem
- F4 - Exclusiones de codigo HTML
- F5 - Matriz de compilacion por placa y variante
- F6 - Pruebas en hardware
- F7 - Automatizacion y CI
- F8 - Documentacion y release
- DUXMAN-LED-FIRMWARE-PLATFORMIO
- Roadmap y evolucion
- Fase de pruebas
- Spring Data
- Pruebas API con Postman
- SyncService::SyncService
- test/README.md
- ColorUtils.h
- [0.6.4-alpha] - 2026-07-27
- Hitos recientes
- EffectAudioPulse
- EffectBlinkGradient

## God Nodes (most connected - your core abstractions)
1. `SyncService` - 92 edges
2. `ApiService` - 84 edges
3. `LedDriver` - 67 edges
4. `impl_` - 65 edges
5. `EffectEngine` - 59 edges
6. `ProfileService` - 54 edges
7. `AudioService` - 45 edges
8. `VoltageOptimizer` - 43 edges
9. `UserPaletteService` - 35 edges
10. `EffectPersistenceService` - 34 edges

## Surprising Connections (you probably didn't know these)
- `Fixed` --references--> `AudioService`  [INFERRED]
  CHANGELOG.md → firmware/src/services/AudioService.h
- `Historical Releases (Imported)` --references--> `SyncService`  [INFERRED]
  CHANGELOG.md → firmware/src/services/SyncService.h
- `Historical Releases (Imported)` --references--> `VoltageOptimizer`  [INFERRED]
  CHANGELOG.md → firmware/src/services/VoltageOptimizer.h
- `Pilares de arquitectura` --references--> `ApiService`  [INFERRED]
  docs/MEMORIA_PROYECTO.md → firmware/src/api/ApiService.h
- `Pilares de arquitectura` --references--> `PersistenceSchedulerService`  [INFERRED]
  docs/MEMORIA_PROYECTO.md → firmware/src/services/PersistenceSchedulerService.h

## Import Cycles
- None detected.

## Communities (128 total, 30 thin omitted)

### Community 0 - "LedDriver"
Cohesion: 0.05
Nodes (31): isRgbwOutput(), LedDriver, frameColors_, LedDriver::~LedDriver(), level_, logicalPixelCount_, logNextShow_, outputLevels_ (+23 more)

### Community 1 - "ApiService"
Cohesion: 0.07
Nodes (79): addJsonResponse(), ApiService, ApiService::begin(), ApiService::buildApiConfigAllHtml(), ApiService::buildApiConfigDebugHtml(), ApiService::buildApiConfigGpioHtml(), ApiService::buildApiConfigMicrophoneHtml(), ApiService::buildApiConfigNetworkHtml() (+71 more)

### Community 2 - "ProfileService"
Cohesion: 0.07
Nodes (24): AppProfile, builtIn, debug, description, gpio, id, microphone, name (+16 more)

### Community 3 - "NeoOutputBase"
Cohesion: 0.15
Nodes (9): createNeoOutput(), createNeoOutputRgbByChannel(), createNeoOutputRgbByMethod(), createNeoOutputRgbwByChannel(), createNeoOutputRgbwByMethod(), destroyNeoOutput(), isAddressableOutputType(), LedDriverNeoPixelBus::~LedDriverNeoPixelBus() (+1 more)

### Community 4 - "Config.cpp"
Cohesion: 0.09
Nodes (44): applyGledoptoMicDefaults(), areDistinctPins(), GeneralConfig::applyPatchJson(), GeneralConfig::defaults(), GeneralConfig::toJson(), GeneralConfig::validate(), GpioConfig::applyPatchJson(), GpioConfig::defaults() (+36 more)

### Community 5 - "EffectPersistenceService"
Cohesion: 0.08
Nodes (26): EffectPersistenceService, activeSequenceIndex_, activeSequenceStartedAtMs_, EffectPersistenceService::EffectPersistenceService(), entries_, entryCount_, hasStartupEffect_, kMaxEntries (+18 more)

### Community 6 - "AudioService"
Cohesion: 0.05
Nodes (35): AudioService, ambientBaseline_, audioBuffer_, audioLevel_, beatCooldownMs_, AudioService::begin(), AudioService::calculateRMS(), AudioService::end() (+27 more)

### Community 7 - "LedDriverFastLed.cpp"
Cohesion: 0.16
Nodes (7): addFastLedController(), createFastLedController(), LedDriverFastLed, activeLedCount_, activeOutputIndex_, initialized_, resolveFastLedOrder()

### Community 8 - "Implementacion de mejoras de efectos (inspirado en WLED)"
Cohesion: 0.05
Nodes (43): Alcance MVP, Alcance MVP, Alcance MVP, Backlog priorizado, Bitacora de cambios, Control de riesgos, Convenciones de estado, Criterio de cierre (+35 more)

### Community 9 - "EffectEngine.h"
Cohesion: 0.10
Nodes (8): EffectBouncingPhysics, EffectTrigRibbon, blendColorLerp(), EffectBreathFixed, EffectBreathGradient, EffectLavaFlow, EffectPolarIce, EffectScanningPulse

### Community 10 - "with"
Cohesion: 0.05
Nodes (43): name, runs-on, steps, strategy, jobs, build, release, environment (+35 more)

### Community 11 - "UserPaletteService"
Cohesion: 0.11
Nodes (12): UserPalette, color1, color2, color3, description, id, label, style (+4 more)

### Community 12 - "SyncService"
Cohesion: 0.05
Nodes (38): SyncService, activeDdpPort_, clusterLastSentAtMs_, clusterLastSequenceRx_, clusterListening_, clusterSequenceTx_, clusterUdp_, ddpUdp_ (+30 more)

### Community 13 - "update_headers.py"
Cohesion: 0.06
Nodes (27): Flujo recomendado de trabajo, Control de flujo, Documentación, Errores, Hilos, Java básico, siempre presente, No generar, aunque el curso lo enseñe, Objetos (+19 more)

### Community 15 - "WifiService.cpp"
Cohesion: 0.10
Nodes (13): hasCustomDns(), normalizedWifiMode(), parseIpOrDefault(), printIpDetails(), registerDiscoveryServices(), WifiService, apEnabled_, dnsApplied_ (+5 more)

### Community 16 - "impl_"
Cohesion: 0.06
Nodes (34): EffectManager::EffectManager(), impl_, audioNeonEqEffect, audioPulseEffect, audioRainbowWaveEffect, audioSectionStrobeEffect, audioSpectrumChaseEffect, audioSpectrumEffect (+26 more)

### Community 17 - "LanguageManager"
Cohesion: 0.12
Nodes (14): Language, ENGLISH, FRENCH, GERMAN, ITALIAN, SPANISH, UNKNOWN, LanguageManager (+6 more)

### Community 18 - "CoreState.cpp"
Cohesion: 0.14
Nodes (16): CoreState::applyPatchJson(), CoreState::defaults(), CoreState::lock(), CoreState::setMutex(), CoreState::snapshot(), CoreState::toJson(), hasValidStatePatch(), isIntegerInRange() (+8 more)

### Community 19 - "Paso a paso completo"
Cohesion: 0.07
Nodes (26): 3.1 Aplicar la escena, 3.2 Resetear contadores de metricas, 3.3 Esperar 10 segundos y hacer la primera lectura, 3.4 Esperar 30 segundos mas y hacer la segunda lectura, 3.5 Rellenar la tabla de la escena smooth en la plantilla, 4.1 Aplicar la escena, 4.2 Resetear, esperar y capturar, 5.1 Aplicar la escena (+18 more)

### Community 20 - "PowerConfig - Advanced Voltage Optimizer Configuration"
Cohesion: 0.08
Nodes (23): API Endpoints, Backward Compatibility, Colors Look Wrong After Dimming, Configuration Examples, Configuration Not Persisting, Configuration Parameters, Example 1: Simple Setup (5V, ~100 LEDs), Example 2: Long Cable Run (300 LEDs, 3m cable) (+15 more)

### Community 22 - "EffectAudioRainbowWave"
Cohesion: 0.10
Nodes (16): EffectAudioNeonEq, beatFlashAtMs_, highEnv_, lowEnv_, midEnv_, smoothEnv(), EffectAudioRainbowWave, beatMs_ (+8 more)

### Community 23 - "StorageService"
Cohesion: 0.18
Nodes (3): PersistenceSchedulerService::PersistenceSchedulerService(), buildGeneralPayloadFromLegacyDebug(), StorageService

### Community 24 - "F0 - Auditoria funcional y baseline"
Cohesion: 0.09
Nodes (22): 10. Criterio de cierre F0, 1. Resultado ejecutivo, 2. Acciones observadas en `home`, 3. Matriz de paridad `home -> API`, 4. Estado configurable del motor, 5. Rutas API funcionales observadas, 6. Hallazgos para fases posteriores, 7. Baseline de filesystem actual (+14 more)

### Community 25 - "Stats"
Cohesion: 0.09
Nodes (21): Stats, activeProtocol, clockOffsetMs, degraded, frameBytes, framesApplied, inputFpsTenths, lastFrameAtMs (+13 more)

### Community 26 - "ColorPaletteDescriptor"
Cohesion: 0.16
Nodes (19): all(), applyToColors(), ColorPaletteDescriptor, color1, color2, color3, description, id (+11 more)

### Community 27 - "VoltageOptimizer"
Cohesion: 0.06
Nodes (29): OutputPowerMetrics, correctionFactor, estimatedCurrentmA, isThrottled, predictedPeakmA, throttleLevel, voltageDrop, PowerHistoryPoint (+21 more)

### Community 29 - "EffectRegistry.cpp"
Cohesion: 0.19
Nodes (17): all(), buildHtmlOptions(), count(), EffectDescriptor, description, id, key, label (+9 more)

### Community 30 - "VoltageOptimizer - Software Voltage Management System"
Cohesion: 0.10
Nodes (20): API Endpoints (Future), Architecture, Basic Initialization, Calibration Guide, Configuration, Core Components, Future Enhancements, LEDs Dimming Unexpectedly (+12 more)

### Community 31 - "steps"
Cohesion: 0.12
Nodes (16): runs-on, steps, jobs, check-wiki-sync, name, on, pull_request, push (+8 more)

### Community 32 - "WatchdogService"
Cohesion: 0.16
Nodes (5): WatchdogService, autoResetOnTimeout_, initialized_, stats_, timeoutSeconds_

### Community 33 - "RenderMetricsService"
Cohesion: 0.09
Nodes (15): RenderMetricsService, fpsCounter_, fpsCurrent_, fpsWindowMs_, frameBeginUs_, frameCount_, frameTimeAccUs_, frameTimeAvgUs_ (+7 more)

### Community 34 - "EffectAudioSectionStrobe"
Cohesion: 0.15
Nodes (11): EffectAudioSectionStrobe, activeSectionIndex_, beatMs_, kColorSectionA, kColorSectionB, kColorSectionC, kColorSectionD, kFlashDurationMs (+3 more)

### Community 35 - "steps"
Cohesion: 0.13
Nodes (14): name, runs-on, steps, jobs, changelog-required, name, on, pull_request (+6 more)

### Community 36 - "Servicios activos en arranque"
Cohesion: 0.17
Nodes (9): Decisiones clave, Memoria del Proyecto - Firmware, Objetivo, Pilares de arquitectura, Prioridades inmediatas, Riesgos conocidos, Stack tecnico, Servicios activos en arranque (+1 more)

### Community 37 - "Arduino y PlatformIO"
Cohesion: 0.14
Nodes (13): Arduino y PlatformIO, Comisión de cambios, Contexto de lenguaje, Fuentes de referencia, Librerías y dependencias, Objetivo, Pinout y documentación, Principios generales (+5 more)

### Community 38 - "Checklist de diagnostico serial (ESP32-C3)"
Cohesion: 0.15
Nodes (12): 10) Descarte rapido de hardware, 1) Cable USB con datos, 2) Puerto correcto, 3) Puerto libre, 4) Subida forzando puerto, 5) Monitor a 115200, 6) Reinicio con monitor abierto, 7) Boot manual si falla upload (+4 more)

### Community 40 - "Instrucciones del Proyecto y Memoria de Arquitectura"
Cohesion: 0.15
Nodes (12): Cómo trabajas, Formato de respuesta, Fuentes, en este orden, Instrucciones del Proyecto y Memoria de Arquitectura, Premisas, Qué nunca haces, Reglas de contexto de código, Reglas de documentación técnica (+4 more)

### Community 42 - "Plan de perfiles `core` y `full`"
Cohesion: 0.17
Nodes (12): 10. Validacion de memoria, 11. Riesgos y mitigaciones, 12. Definition of Done, 13. Registro de decisiones, 14. Documentos relacionados, 3. Regla funcional principal, 7. Interfaz local minima de `core`, 8. Fases de desarrollo (+4 more)

### Community 43 - "API REST v1"
Cohesion: 0.17
Nodes (12): API REST v1, Coleccion Postman, Configuracion, Efectos y secuencias, Ejemplos para cliente externo, Estado y sistema, Metadatos del sistema, Notas de implementacion (+4 more)

### Community 44 - "Compatibilidad de sincronizacion (LedFx, WLED, E1.31, MQTT)"
Cohesion: 0.17
Nodes (12): Alcance y limites actuales, Cluster DUXMAN (emisor), Cluster DUXMAN (receptor), Compatibilidad de sincronizacion (LedFx, WLED, E1.31, MQTT), Diagnostico recomendado, Ejemplos de configuracion por API, LedFx + DDP, LedFx + E1.31 (+4 more)

### Community 45 - "EffectAudioSpectrumChase"
Cohesion: 0.20
Nodes (8): EffectAudioSpectrumChase, beatMs_, chasePosition_, kBeatFlashMs, kColorBass, kColorMid, kColorTreble, kTailWidth

### Community 46 - "PersistenceSchedulerService"
Cohesion: 0.27
Nodes (6): PendingFlags, kPendingConfig, kPendingState, PersistenceSchedulerService, pendingFlags_, UserPaletteService::UserPaletteService()

### Community 47 - "Baseline de metricas de render — DUXMAN-LED-NEXT"
Cohesion: 0.20
Nodes (9): Baseline de metricas de render — DUXMAN-LED-NEXT, Criterio de cierre F0, Escena 1 — smooth, Escena 2 — show, Escena 3 — aggressive, Historial de capturas, Informacion del build, Resultado de GET /api/v1/hardware (+1 more)

### Community 48 - "EffectBlinkFixed"
Cohesion: 0.40
Nodes (3): EffectBlinkFixed, lastToggleAtMs_, visible_

### Community 51 - "LedDriverNeoPixelBus"
Cohesion: 0.33
Nodes (4): LedDriverNeoPixelBus, initialized_, outputs_, peakLedCounts_

### Community 52 - "Changelog"
Cohesion: 0.10
Nodes (19): [0.6.3-alpha] - 2026-07-23, [0.6.4-beta] - 2026-07-27, [0.6.5-beta] - 2026-07-28, [0.6.7-beta] - 2026-11-07, Added, Added, Changed, Changed (+11 more)

### Community 53 - "4. Capacidades que debe cubrir la API"
Cohesion: 0.22
Nodes (9): 4.1 Control de estado y efectos, 4.2 Catalogo de efectos, 4.3 Persistencia de efectos, 4.4 Paletas, 4.5 Configuracion del dispositivo, 4.6 Perfiles, 4.7 Diagnostico y sistema, 4.8 Sincronizacion (+1 more)

### Community 54 - "Release Checklist (2 minutos)"
Cohesion: 0.22
Nodes (8): 1) Preparar version, 2) Actualizar changelog, 3) Commit y push, 4) Crear tag de release, 5) Verificar GitHub Actions, 6) Verificar assets de release, 7) Post-release rapido, Release Checklist (2 minutos)

### Community 55 - "Arquitectura del firmware"
Cohesion: 0.22
Nodes (9): Arquitectura del firmware, Diseno general, Modelos de configuracion, Motor LED, Persistencia, Referencias, Sincronizacion (stack S1-S6), Tareas FreeRTOS (+1 more)

### Community 56 - "EffectStellarTwinkle.cpp"
Cohesion: 0.31
Nodes (3): EffectStellarTwinkle, seed_, hash32()

### Community 57 - "EffectRandomColorPop.cpp"
Cohesion: 0.31
Nodes (3): EffectRandomColorPop, seed_, hash32()

### Community 59 - "Instrucciones de Graphify para memoria arquitectónica y agentes"
Cohesion: 0.22
Nodes (8): 1. Propósito del grafo de conocimiento, 2. Regla operativa obligatoria, 3. Patrón de automatización del grafo, 4. Inyección de memoria en prompts de agentes, 5. Búsqueda semántica y consulta de dependencias, 6. Patrón recomendado para multi-agente, 7. Regla final del repositorio, Instrucciones de Graphify para memoria arquitectónica y agentes

### Community 60 - "Context Transfer From Monorepo"
Cohesion: 0.25
Nodes (7): Context Transfer From Monorepo, Convenciones de mantenimiento en firmware, Decisiones tomadas, Lecciones importantes, Método de separación validado, Prioridades de continuidad, Repositorios resultantes

### Community 61 - "Configuration Schema"
Cohesion: 0.25
Nodes (8): Configuration Schema, Endpoints relacionados, general, gpio, microphone, network, Root JSON, sync

### Community 62 - "Stats"
Cohesion: 0.33
Nodes (5): Stats, controlTaskResets, lastResetTimeMs, renderTaskResets, resetCount

### Community 63 - "Instrucciones del Proyecto y Memoria de Arquitectura"
Cohesion: 0.25
Nodes (7): Cómo trabajas, Formato de respuesta, Fuentes, en este orden, Instrucciones del Proyecto y Memoria de Arquitectura, Premisas, Qué nunca haces, Uso Obligatorio del Grafo de Conocimiento

### Community 64 - "java.instructions.md"
Cohesion: 0.32
Nodes (3): Qué no se escribe, Qué se escribe, Versión de Java

### Community 65 - "Registro y logging"
Cohesion: 0.25
Nodes (7): API estándar, Configuración en Spring Boot (`application.yml` o `logback-spring.xml`), Log especial de auditoría de rendimiento y tiempos (cuando se solicite), Registro y logging, Reglas de seguridad y buenas prácticas, Trazabilidad obligatoria en fichero, Uso estricto de niveles de log

### Community 69 - "flash.ps1"
Cohesion: 0.43
Nodes (5): Get-AutoPort(), Get-PlatformIoWorkingDirectory(), Get-RepoRoot(), Get-SerialPorts(), Test-IsRecognizedUsbSerialPort()

### Community 70 - "Tools"
Cohesion: 0.29
Nodes (6): esptool, flash.ps1, LittleFS / plantillas UI, Tools, wiki-check-drift.ps1, wiki-sync.ps1

### Community 71 - "wiki-sync.ps1"
Cohesion: 0.43
Nodes (4): ConvertTo-WindowsArgument(), Invoke-Git(), Resolve-DefaultBranch(), Resolve-WikiUrl()

### Community 72 - "Perfiles GPIO"
Cohesion: 0.33
Nodes (6): API de perfiles, Ejemplo de perfil completo, Notas, Perfiles GPIO, Que es un perfil, Tipos

### Community 73 - "Paletas de color"
Cohesion: 0.33
Nodes (5): API de paletas, Ejemplo de paleta de usuario, Paletas de color, Paletas de usuario, Paletas predefinidas

### Community 74 - "LedDriverColorOrder"
Cohesion: 0.14
Nodes (14): LedDriverColorOrder, B, BGR, BRG, G, GBR, GRB, GRBW (+6 more)

### Community 75 - "NeoOutputRgb"
Cohesion: 0.12
Nodes (6): NeoOutputRgb, bus_, busCount_, ledCount_, rgbFromColor(), rgbwFromColor()

### Community 76 - "EffectCanvas1D.h"
Cohesion: 0.14
Nodes (5): Metricas F2 (Triple Chase), EffectDiagnostic, EffectFixed, EffectGradient, EffectTripleChase

### Community 77 - "NeoOutputRgbw"
Cohesion: 0.22
Nodes (4): NeoOutputRgbw, bus_, busCount_, ledCount_

### Community 78 - "Pruebas unitarias"
Cohesion: 0.33
Nodes (6): Cómo se escribe, Mockito, No generar, Pruebas unitarias, Spring Boot, Versión

### Community 79 - "Patrones de diseño"
Cohesion: 0.33
Nodes (5): Comportamiento, Creación, Cómo elegir, Estructura, Patrones de diseño

### Community 80 - "Spring Boot 4"
Cohesion: 0.33
Nodes (5): Cómo se compone, Preferencia por Spring Boot, Qué no se genera, Referencias oficiales y documentación viva, Spring Boot 4

### Community 81 - "pull_request_template.md"
Cohesion: 0.33
Nodes (5): Checklist Wiki, Notas adicionales, Resumen, Tipo de cambio, Validacion realizada

### Community 82 - "LedDriverOutputConfig"
Cohesion: 0.20
Nodes (9): LedDriverOutputConfig, colorOrder, enabled, id, isDigital, isRgbw, ledCount, ledType (+1 more)

### Community 83 - "LedDriverType"
Cohesion: 0.22
Nodes (9): LedDriverType, Digital, Sk6812, Tm1814, Unknown, Ws2811, Ws2812b, Ws2813 (+1 more)

### Community 84 - "5. Contrato API y calidad"
Cohesion: 0.40
Nodes (5): 5. Contrato API y calidad, Codigos HTTP minimos, Punto a verificar, Requisitos del contrato, Respuesta recomendada

### Community 85 - "F0 - Baseline y auditoria funcional"
Cohesion: 0.40
Nodes (5): Criterio de cierre, F0 - Baseline y auditoria funcional, Objetivo, Registro, Tareas

### Community 86 - "F1 - Contrato API completo"
Cohesion: 0.40
Nodes (5): Criterio de cierre, F1 - Contrato API completo, Objetivo, Registro de reanudacion F1, Tareas

### Community 87 - "F2 - Pruebas de paridad con `home`"
Cohesion: 0.33
Nodes (6): Criterio de cierre, Estado, F2 - Pruebas de paridad con `home`, Herramienta de prueba, Objetivo, Tareas

### Community 89 - "SynchronizedClockState"
Cohesion: 0.40
Nodes (5): SynchronizedClockState, currentOffsetSec, enabled, lastUpdateMs, targetOffsetSec

### Community 90 - "Java en este repositorio"
Cohesion: 0.40
Nodes (5): Conocimiento ya extraído, Cómo procesar un PDF nuevo, Java en este repositorio, Premisas, Prácticas declaradas

### Community 91 - "Pruebas externas y de carga"
Cohesion: 0.40
Nodes (4): JMeter, No generar, Pruebas externas y de carga, Selenium

### Community 94 - "1. Objetivos"
Cohesion: 0.50
Nodes (4): 1. Objetivos, Fuera de alcance inicial, Objetivo principal, Objetivos concretos

### Community 95 - "2. Situacion actual"
Cohesion: 0.50
Nodes (4): 2. Situacion actual, Configuracion de red existente, Interfaz actual, Seleccion de placa

### Community 96 - "6. Arquitectura de builds"
Cohesion: 0.50
Nodes (4): 6. Arquitectura de builds, Filesystem, Macros de compilacion, Perfiles previstos

### Community 97 - "F3 - Separacion de filesystem"
Cohesion: 0.50
Nodes (4): Criterio de cierre, F3 - Separacion de filesystem, Objetivo, Tareas

### Community 98 - "F4 - Exclusiones de codigo HTML"
Cohesion: 0.50
Nodes (4): Criterio de cierre, F4 - Exclusiones de codigo HTML, Objetivo, Tareas

### Community 99 - "F5 - Matriz de compilacion por placa y variante"
Cohesion: 0.50
Nodes (4): Criterio de cierre, F5 - Matriz de compilacion por placa y variante, Objetivo, Tareas

### Community 100 - "F6 - Pruebas en hardware"
Cohesion: 0.50
Nodes (4): Criterio de cierre, F6 - Pruebas en hardware, Objetivo, Tareas

### Community 101 - "F7 - Automatizacion y CI"
Cohesion: 0.50
Nodes (4): Criterio de cierre, F7 - Automatizacion y CI, Objetivo, Tareas

### Community 102 - "F8 - Documentacion y release"
Cohesion: 0.50
Nodes (4): Criterio de cierre, F8 - Documentacion y release, Objetivo, Tareas

### Community 103 - "DUXMAN-LED-FIRMWARE-PLATFORMIO"
Cohesion: 0.50
Nodes (4): Contenido, DUXMAN-LED-FIRMWARE-PLATFORMIO, Inicio rapido, Repositorios

### Community 104 - "Roadmap y evolucion"
Cohesion: 0.50
Nodes (4): Completado en v0.6.3-alpha, Decisiones tecnicas consolidadas, Fases siguientes, Roadmap y evolucion

### Community 107 - "Fase de pruebas"
Cohesion: 0.50
Nodes (4): Cómo queda documentada, Fase de pruebas, Qué entra en el incremento, Qué se propone siempre

### Community 108 - "Spring Data"
Cohesion: 0.50
Nodes (3): Cómo se escribe, Qué no se genera, Spring Data

### Community 109 - "Pruebas API con Postman"
Cohesion: 0.67
Nodes (3): Importar, Orden recomendado, Pruebas API con Postman

### Community 120 - "ColorUtils.h"
Cohesion: 0.40
Nodes (3): formatHexColor(), parseColorValue(), parseHexColor()

### Community 121 - "[0.6.4-alpha] - 2026-07-27"
Cohesion: 0.50
Nodes (4): [0.6.4-alpha] - 2026-07-27, Added, Changed, Docs

### Community 122 - "Hitos recientes"
Cohesion: 0.14
Nodes (13): [0.6.6-beta] - 2026-07-29, Added, Changed, Docs, Calibración DSP de Audio y Eliminación de Pedestal DC (2026-11-07), Diagnostico provisional de pila OpenAPI (2026-11-07), F1 - Contrato API completo (2026-09-18), F2 - Pruebas de paridad con `home` completadas (2026-11-07) (+5 more)

### Community 124 - "EffectBlinkGradient"
Cohesion: 0.40
Nodes (3): EffectBlinkGradient, lastToggleAtMs_, visible_

## Knowledge Gaps
- **344 isolated node(s):** `frameColors_`, `level_`, `logicalPixelCount_`, `logNextShow_`, `outputLevels_` (+339 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 931 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **30 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Servicios activos en arranque` connect `Servicios activos en arranque` to `WatchdogService`, `ApiService`, `ProfileService`, `EffectPersistenceService`, `AudioService`, `UserPaletteService`, `SyncService`, `PersistenceSchedulerService`, `WifiService.cpp`, `StorageService`, `Arquitectura del firmware`?**
  _High betweenness centrality (0.155) - this node is a cross-community bridge._
- **Are the 3 inferred relationships involving `SyncService` (e.g. with `Historical Releases (Imported)` and `Pilares de arquitectura`) actually correct?**
  _`SyncService` has 3 INFERRED edges - model-reasoned connections that need verification._
- **What connects `frameColors_`, `level_`, `logicalPixelCount_` to the rest of the system?**
  _344 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `LedDriver` be split into smaller, more focused modules?**
  _Cohesion score 0.053904428904428904 - nodes in this community are weakly interconnected._
- **Why does `LedDriver` connect `LedDriver` to `ProfileService`, `LedDriverFastLed.cpp`, `main.cpp`, `impl_`, `LedDriverOutputConfig`, `LedDriverNeoPixelBus`?**
  _High betweenness centrality (0.118) - this node is a cross-community bridge._
- **Are the 3 inferred relationships involving `ApiService` (e.g. with `Pilares de arquitectura` and `Servicios activos en arranque`) actually correct?**
  _`ApiService` has 3 INFERRED edges - model-reasoned connections that need verification._
- **Should `ApiService` be split into smaller, more focused modules?**
  _Cohesion score 0.07166042446941323 - nodes in this community are weakly interconnected._