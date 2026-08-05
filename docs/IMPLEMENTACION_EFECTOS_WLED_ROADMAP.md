# Implementacion de mejoras de efectos (inspirado en WLED)

Este documento sirve para llevar registro de avance, decisiones y estado de implementacion.

## Convenciones de estado

- [ ] Pendiente
- [~] En progreso
- [x] Implementado
- [!] Bloqueado

## Resumen de fases

| Fase | Objetivo | Estado | Resultado esperado |
|---|---|---|---|
| F0 | Baseline y metricas visuales | [x] | Referencia reproducible de FPS/calidad para comparar cambios |
| F1 | Transiciones entre efectos | [~] | Cambio de efecto suave sin cortes bruscos |
| F2 | Primitivas compartidas + canvas | [~] | Base comun para trail, blur, fade y mezcla |
| F3 | Grouping y spacing | [ ] | Mejor legibilidad visual en tiras largas |
| F4 | Capa overlay (composicion ligera) | [ ] | Efecto base + acento visual combinables |
| F5 | Ideas adicionales de WLED | [ ] | Mejoras de visibilidad y usabilidad sin romper API |

---

## F0 - Baseline y metricas visuales

### Objetivo
Tener una referencia objetiva antes de tocar el motor.

### Tareas
- [x] Definir 3 escenas de prueba: smooth, show, aggressive.
- [x] Medir FPS medio y estabilidad visual en cada escena.
- [ ] Guardar capturas/video corto para comparacion antes/despues.
- [x] Registrar consumo aproximado de CPU y RAM si aplica.

### Criterio de cierre
- [x] Existe un baseline documentado y repetible para comparar cada fase.

### Registro
- Responsable:
- Fecha inicio: 2026-07-29
- Fecha cierre: 2026-07-29
- Notas: RenderMetricsService implementado (GET /api/v1/metrics + POST /api/v1/metrics/reset). Escenas y plantilla de captura en docs/baseline/.

### Metricas baseline (hardware real)

| Escena | FPS | frameTimeAvgUs | frameTimeMinUs | frameTimeMaxUs | heapFreeBytes | heapMinFreeBytes | renderStackWords |
|---|---:|---:|---:|---:|---:|---:|---:|
| smooth | 62 | 215 | 162 | 20434 | 157732 | 123244 | 3088 |
| show | 62 | 294 | 162 | 20728 | 158092 | 123244 | 3088 |
| aggressive | 62 | 339 | 162 | 20728 | 158092 | 123244 | 3088 |

Comparativa directa:

- show vs smooth: +36.7% en frameTimeAvgUs (294 vs 215).
- aggressive vs smooth: +57.7% en frameTimeAvgUs (339 vs 215).
- FPS estable (62) en las 3 escenas.
- Memoria libre y minimo historico estables; sin evidencia de fuga en esta ventana.

---

## F1 - Transiciones entre efectos (Punto 2)

### Objetivo
Eliminar el cambio abrupto al cambiar effectId.

### Alcance MVP
- [x] Transicion tipo fade.
- [x] Transicion tipo wipe horizontal.
- [x] Duracion configurable (ejemplo: 0 a 1500 ms).
- [x] Fallback a cambio instantaneo si duracion = 0.

### Tareas tecnicas
- [x] Introducir estado de transicion en el manager.
- [x] Conservar frame anterior (prevFrame) y frame nuevo (nextFrame).
- [x] Mezclar ambos frames segun progreso temporal.
- [ ] Aplicar show final una sola vez por frame.

### Criterio de cierre
- [ ] Cambiar entre 10 efectos seguidos no genera cortes bruscos.
- [ ] No aparecen artefactos ni parpadeos por carrera de buffer.

### Registro
- Responsable:
- Fecha inicio: 2026-08-05
- Fecha cierre:
- PR/Commit:
- Notas: MVP fade + wipe implementado con parametros `effectTransitionMs` (0..1500) y `effectTransitionStyle` (`fade|wipe`) en CoreState/API. Mezcla prev->next en EffectManager. Pendiente optimizacion para evitar doble show durante la ventana de transicion.

---

## F2 - Primitivas compartidas + canvas (Punto 4 + tema primitivas)

### Objetivo
Unificar operaciones visuales de alto impacto para reutilizarlas en todos los efectos.

### Primitivas MVP
- [x] setPixel / getPixel
- [x] clear / fill
- [x] addColor saturado
- [x] fadeToBlackBy global
- [x] blur1D (kernel simple)
- [x] blend alpha entre buffers

### Tareas tecnicas
- [x] Crear un canvas 1D por salida (o virtual unificado).
- [ ] Integrar flush del canvas al driver al final del frame.
- [x] Reusar primitivas en al menos 3 efectos existentes (meteor, scanner, chase).
- [ ] Validar impacto en RAM y tiempo por frame.

### Criterio de cierre
- [ ] Al menos 3 efectos migrados a primitivas comunes.
- [ ] Se observa mejora visual de trails/desenfoque sin perder estabilidad.

### Registro
- Responsable:
- Fecha inicio: 2026-08-05
- Fecha cierre:
- PR/Commit:
- Notas: Primitivas F2 integradas en EffectEngine (set/get/fill/add/fade/blur/blend) con canvas virtual apoyado en LedDriver. Migrados EffectGradientMeteor, EffectScanningPulse y EffectTripleChase.

### Metricas preliminares F2 (Gradient Meteor)

| Escena | FPS | frameTimeAvgUs | frameTimeMinUs | frameTimeMaxUs | heapFreeBytes | heapMinFreeBytes | renderStackWords |
|---|---:|---:|---:|---:|---:|---:|---:|
| smooth | 62 | 2276 | 584 | 27821 | 159284 | 152108 | 2960 |
| show | 62 | 1532 | 584 | 28857 | 159284 | 152108 | 2960 |
| aggressive | 62 | 1390 | 584 | 28857 | 159292 | 151396 | 2960 |

Lectura inicial:

- FPS estable (62) en los 3 perfiles.
- El coste medio por frame baja al subir intensidad (smooth > show > aggressive), señal de que la parametrizacion actual de cola/blur castiga mas el preset smooth.
- Memoria estable (sin saltos bruscos de heap libre/minimo) durante la ventana de prueba.
- Proxima accion recomendada: recalibrar `fadeToBlackBy` y `blur1D` en `EffectGradientMeteor` para que `smooth` reduzca coste computacional sin perder suavidad visual.

### Metricas tras ajuste F2 (Gradient Meteor)

| Escena | FPS | frameTimeAvgUs | frameTimeMinUs | frameTimeMaxUs | heapFreeBytes | heapMinFreeBytes | renderStackWords |
|---|---:|---:|---:|---:|---:|---:|---:|
| smooth | 62 | 661 | 581 | 25547 | 159732 | 158016 | 3092 |
| show | 62 | 745 | 581 | 27186 | 159284 | 158016 | 3092 |
| aggressive | 62 | 818 | 581 | 27186 | 159272 | 157408 | 3092 |

Comparativa vs medicion anterior (frameTimeAvgUs):

- smooth: 2276 -> 661 (mejora de -70.9%).
- show: 1532 -> 745 (mejora de -51.4%).
- aggressive: 1390 -> 818 (mejora de -41.2%).

Lectura:

- Se corrige el perfil invertido de coste (ahora smooth < show < aggressive), que era el objetivo del ajuste.
- FPS se mantiene estable en 62.
- Memoria libre/minima mejora y se mantiene estable durante la ventana de prueba.

### Metricas F2 (Scanning Pulse / Pulso Barrido)

| Escena | FPS | frameTimeAvgUs | frameTimeMinUs | frameTimeMaxUs | heapFreeBytes | heapMinFreeBytes | renderStackWords |
|---|---:|---:|---:|---:|---:|---:|---:|
| smooth | 62 | 847 | 539 | 27186 | 159328 | 155336 | 3024 |
| show | 62 | 842 | 539 | 28041 | 159328 | 155336 | 3024 |
| aggressive | 62 | 857 | 539 | 31710 | 159328 | 155336 | 3024 |

Lectura:

- FPS estable (62) en los 3 perfiles.
- Coste medio por frame muy uniforme (842..857 us), sin picos de degradacion ligados al perfil.
- Memoria estable en toda la prueba (`heapFreeBytes` y `heapMinFreeBytes` constantes).
- Siguiente medicion recomendada para cerrar F2: `triple_chase` con la misma plantilla smooth/show/aggressive.

### Metricas F2 (Triple Chase)

| Escena | FPS | frameTimeAvgUs | frameTimeMinUs | frameTimeMaxUs | heapFreeBytes | heapMinFreeBytes | renderStackWords |
|---|---:|---:|---:|---:|---:|---:|---:|
| smooth | 62 | 596 | 524 | 12395 | 160180 | 159792 | 3028 |
| show | 62 | 636 | 524 | 27576 | 158956 | 158896 | 3028 |
| aggressive | 62 | 709 | 524 | 27576 | 158904 | 158896 | 3028 |

Lectura:

- FPS estable (62) y coste medio escalando de forma esperada (smooth < show < aggressive).
- Se reporta artefacto visual en `show` y `aggressive` (algunos LEDs quedan aparentementes fijos).
- Mitigacion aplicada: en `EffectTripleChase` se sustituye el post-procesado acumulativo (fade+blur) por composicion alfa directa por frame para evitar retencion residual de pixeles.
- Estado actual: el artefacto persiste en `show` tras la mitigacion inicial.
- Pendiente: validar en hardware una segunda mitigacion (3 cabezas moviles + composicion completa por pixel/frame) y repetir metricas.

---

## F3 - Grouping y spacing (Punto 6)

### Objetivo
Mejorar visibilidad en tiras largas creando bloques de luz con huecos controlados.

### Alcance MVP
- [ ] Parametro grouping (1..8)
- [ ] Parametro spacing (0..8)
- [ ] Aplicacion global para efectos 1D

### Tareas tecnicas
- [ ] Extender estado/config con grouping y spacing.
- [ ] Aplicar transformacion virtual->fisica de pixel.
- [ ] Exponer parametros en API/UI.
- [ ] Validar comportamiento en tiras cortas y largas.

### Criterio de cierre
- [ ] Grouping y spacing funcionales en al menos 5 efectos.
- [ ] Sin regresiones en salidas digitales no direccionables.

### Registro
- Responsable:
- Fecha inicio:
- Fecha cierre:
- PR/Commit:
- Notas:

---

## F4 - Capa overlay (Punto 1)

### Objetivo
Permitir composicion ligera de efectos para ganar impacto visual rapido.

### Alcance MVP
- [ ] overlayEffectId opcional.
- [ ] overlayLevel (intensidad del overlay).
- [ ] Modos de mezcla iniciales: add, alpha.
- [ ] Overlay desactivable para mantener compatibilidad.

### Tareas tecnicas
- [ ] Crear pipeline base + overlay + composicion.
- [ ] Definir lista corta de overlays iniciales (sparkle, strobe, twinkle).
- [ ] Limitar carga de CPU para mantener FPS estable.
- [ ] Exponer seleccion de overlay en API/UI.

### Criterio de cierre
- [ ] Se puede combinar un efecto base con 1 overlay sin flicker.
- [ ] FPS se mantiene dentro del objetivo definido en F0.

### Registro
- Responsable:
- Fecha inicio:
- Fecha cierre:
- PR/Commit:
- Notas:

---

## F5 - Ideas adicionales de WLED recomendadas

### Objetivo
Incorporar mejoras de visibilidad/operativa con bajo riesgo.

### Backlog priorizado
- [ ] Simulacion de audio para efectos reactivos cuando no hay micro.
- [ ] Presets por efecto con defaults recomendados.
- [ ] Flags reverse y mirror en 1D.
- [ ] Freeze frame (pausa de animacion).
- [ ] Metadata de capacidades por efecto (usa speed, level, audio, etc.).

### Criterio de cierre
- [ ] Al menos 2 mejoras implementadas y validadas en demo visual.

### Registro
- Responsable:
- Fecha inicio:
- Fecha cierre:
- PR/Commit:
- Notas:

---

## Control de riesgos

- [ ] RAM insuficiente por buffers extra (transicion/canvas/overlay).
- [ ] Bajada de FPS por mezcla y blur.
- [ ] Regresiones en salidas no direccionables.
- [ ] Desalineacion API/UI al introducir nuevos parametros.

Mitigacion sugerida:
- Mantener switches de feature para activar por fases.
- Probar en hardware objetivo tras cada fase.
- Medir FPS y estabilidad en cada merge.

---

## Definicion de terminado global

- [ ] F1, F2, F3 y F4 en estado Implementado.
- [ ] Al menos 2 items de F5 implementados.
- [ ] Baseline F0 actualizado con comparativa antes/despues.
- [ ] Documentacion de uso API/UI actualizada.

---

## Bitacora de cambios

| Fecha | Fase | Cambio | Estado | Referencia |
|---|---|---|---|---|
| 2026-07-29 | F0 | RenderMetricsService + endpoint /api/v1/metrics + escenas baseline | [x] | — |
