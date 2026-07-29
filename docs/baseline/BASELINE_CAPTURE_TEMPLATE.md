# Baseline de metricas de render — DUXMAN-LED-NEXT

> Plantilla de captura para la fase F0.
> Rellenar una tabla por version/build. Comparar antes/despues de cada fase implementada.

## Informacion del build

| Campo | Valor |
|---|---|
| Version firmware | |
| Fecha captura | |
| Hardware (placa) | |
| LED count (salida 0) | |
| LED type | |
| CPU freq (MHz) | |
| Build profile | |
| Git commit | |

## Resultado de GET /api/v1/hardware

```
(pegar JSON completo aqui)
```

---

## Escena 1 — smooth

**Payload aplicado:** `scenes.smooth` de `test-scenes.json`

**Lectura 1 (t+10 s)**
```json
(pegar GET /api/v1/metrics)
```

**Lectura 2 (t+40 s)**
```json
(pegar GET /api/v1/metrics)
```

| Metrica | Lectura 1 | Lectura 2 |
|---|---|---|
| fps | | |
| frameTimeAvgUs | | |
| frameTimeMinUs | | |
| frameTimeMaxUs | | |
| heapFreeBytes | | |
| heapMinFreeBytes | | |
| renderStackWords | | |

**Observaciones:**

---

## Escena 2 — show

**Payload aplicado:** `scenes.show` de `test-scenes.json`

**Lectura 1 (t+10 s)**
```json
(pegar GET /api/v1/metrics)
```

**Lectura 2 (t+40 s)**
```json
(pegar GET /api/v1/metrics)
```

| Metrica | Lectura 1 | Lectura 2 |
|---|---|---|
| fps | | |
| frameTimeAvgUs | | |
| frameTimeMinUs | | |
| frameTimeMaxUs | | |
| heapFreeBytes | | |
| heapMinFreeBytes | | |
| renderStackWords | | |

**Observaciones:**

---

## Escena 3 — aggressive

**Payload aplicado:** `scenes.aggressive` de `test-scenes.json`

**Lectura 1 (t+10 s)**
```json
(pegar GET /api/v1/metrics)
```

**Lectura 2 (t+40 s)**
```json
(pegar GET /api/v1/metrics)
```

| Metrica | Lectura 1 | Lectura 2 |
|---|---|---|
| fps | | |
| frameTimeAvgUs | | |
| frameTimeMinUs | | |
| frameTimeMaxUs | | |
| heapFreeBytes | | |
| heapMinFreeBytes | | |
| renderStackWords | | |

**Observaciones:**

---

## Resumen comparativo

| Metrica | smooth | show | aggressive | Limite aceptable |
|---|---|---|---|---|
| fps avg | | | | ≥ 40 |
| frameTimeAvgUs | | | | ≤ 8000 |
| frameTimeMaxUs | | | | ≤ 15000 |
| heapFreeBytes | | | | ≥ 30000 |
| renderStackWords | | | | ≥ 128 |

---

## Criterio de cierre F0

- [ ] Las 3 escenas capturadas.
- [ ] fps >= 40 en todas las escenas.
- [ ] heapFreeBytes >= 30000 en todas las escenas.
- [ ] renderStackWords >= 128 en todas las escenas.
- [ ] Archivo guardado como `docs/baseline/BASELINE_v<version>_<fecha>.md`.

---

## Historial de capturas

| Version | Fecha | smooth fps | show fps | aggressive fps | Notas |
|---|---|---|---|---|---|
| | | | | | |
