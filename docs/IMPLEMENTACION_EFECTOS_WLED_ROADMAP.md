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
| F1 | Transiciones entre efectos | [ ] | Cambio de efecto suave sin cortes bruscos |
| F2 | Primitivas compartidas + canvas | [ ] | Base comun para trail, blur, fade y mezcla |
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

---

## F1 - Transiciones entre efectos (Punto 2)

### Objetivo
Eliminar el cambio abrupto al cambiar effectId.

### Alcance MVP
- [ ] Transicion tipo fade.
- [ ] Transicion tipo wipe horizontal.
- [ ] Duracion configurable (ejemplo: 0 a 1500 ms).
- [ ] Fallback a cambio instantaneo si duracion = 0.

### Tareas tecnicas
- [ ] Introducir estado de transicion en el manager.
- [ ] Conservar frame anterior (prevFrame) y frame nuevo (nextFrame).
- [ ] Mezclar ambos frames segun progreso temporal.
- [ ] Aplicar show final una sola vez por frame.

### Criterio de cierre
- [ ] Cambiar entre 10 efectos seguidos no genera cortes bruscos.
- [ ] No aparecen artefactos ni parpadeos por carrera de buffer.

### Registro
- Responsable:
- Fecha inicio:
- Fecha cierre:
- PR/Commit:
- Notas:

---

## F2 - Primitivas compartidas + canvas (Punto 4 + tema primitivas)

### Objetivo
Unificar operaciones visuales de alto impacto para reutilizarlas en todos los efectos.

### Primitivas MVP
- [ ] setPixel / getPixel
- [ ] clear / fill
- [ ] addColor saturado
- [ ] fadeToBlackBy global
- [ ] blur1D (kernel simple)
- [ ] blend alpha entre buffers

### Tareas tecnicas
- [ ] Crear un canvas 1D por salida (o virtual unificado).
- [ ] Integrar flush del canvas al driver al final del frame.
- [ ] Reusar primitivas en al menos 3 efectos existentes (meteor, scanner, chase).
- [ ] Validar impacto en RAM y tiempo por frame.

### Criterio de cierre
- [ ] Al menos 3 efectos migrados a primitivas comunes.
- [ ] Se observa mejora visual de trails/desenfoque sin perder estabilidad.

### Registro
- Responsable:
- Fecha inicio:
- Fecha cierre:
- PR/Commit:
- Notas:

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
