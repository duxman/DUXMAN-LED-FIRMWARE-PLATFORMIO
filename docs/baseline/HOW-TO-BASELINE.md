# Guia paso a paso — Captura del Baseline de Metricas (F0)

## Que es el baseline y para que sirve

El baseline es una fotografia del rendimiento del firmware ANTES de implementar
cualquier mejora de efectos (F1-F5). Sirve para:

- Saber exactamente cuantos FPS y cuanta memoria consume cada escena hoy.
- Comparar con la misma medicion despues de implementar cada fase.
- Detectar regresiones antes de que lleguen a produccion.

---

## Requisitos previos

| Requisito | Como verificar |
|---|---|
| Firmware compilado y flasheado (v0.4.x o superior) | `tools\flash.ps1` o `pio run --target upload` |
| ESP32 encendido y en red WiFi (modo STA o AP) | Ver IP en monitor serial: `[api] STA ip=...` |
| Tiras LED conectadas y configuradas | Al menos 1 salida con LEDs funcionando |
| Monitor serial o acceso HTTP al dispositivo | Puerto 80 / baud 115200 |

---

## Herramientas de captura

Elige la que prefieras. Ambas funcionan igual.

### Opcion A — HTTP (recomendada si tienes IP)

Necesitas `curl` (incluido en Windows 10+) o cualquier cliente HTTP (Postman, Thunder Client, etc.)

```
IP del dispositivo: http://192.168.x.x   (ver en serial al arrancar)
```

### Opcion B — Serial (si no tienes red)

Abre el monitor serial a 115200 baud:

```powershell
# Con PlatformIO:
pio device monitor --baud 115200
# O con el script de flash:
.\tools\flash.ps1 -Action monitor
```

Escribe los comandos directamente en el monitor y presiona Enter.

---

## Paso a paso completo

### PASO 1 — Flashear el firmware

```powershell
cd c:\desarrollo\DUXMAN-LED-FIRMWARE-PLATFORMIO
.\tools\flash.ps1 -Action upload-monitor
```

Espera a ver en el serial:

```
[boot] DUXMAN-LED-NEXT started
[api] STA ip=192.168.x.x
[api] ready: GET /api/v1/metrics
```

Apunta la IP del dispositivo.

---

### PASO 2 — Anotar informacion del build

Ejecuta antes de cualquier escena:

**HTTP:**
```
GET http://192.168.x.x/api/v1/hardware
```

**Serial:**
```
GET /api/v1/hardware
```

Copia el resultado completo en la seccion **Resultado de GET /api/v1/hardware**
del archivo `BASELINE_CAPTURE_TEMPLATE.md`.

---

### PASO 3 — Escena 1: smooth

#### 3.1 Aplicar la escena

**HTTP:**
```bash
curl -X PATCH http://192.168.x.x/api/v1/state \
  -H "Content-Type: application/json" \
  -d "{\"power\":true,\"brightness\":120,\"effect\":\"lava_flow\",\"sectionCount\":3,\"effectSpeed\":24,\"effectLevel\":3,\"primaryColors\":[\"#FF4D00\",\"#FFD400\",\"#002BFF\"],\"backgroundColor\":\"#020202\"}"
```

**Serial:**
```
PATCH /api/v1/state {"power":true,"brightness":120,"effect":"lava_flow","sectionCount":3,"effectSpeed":24,"effectLevel":3,"primaryColors":["#FF4D00","#FFD400","#002BFF"],"backgroundColor":"#020202"}
```

#### 3.2 Resetear contadores de metricas

**HTTP:**
```
POST http://192.168.x.x/api/v1/metrics/reset
```

**Serial:**
```
POST /api/v1/metrics/reset
```

Respuesta esperada: `{"reset":true}`

#### 3.3 Esperar 10 segundos y hacer la primera lectura

**HTTP:**
```
GET http://192.168.x.x/api/v1/metrics
```

**Serial:**
```
GET /api/v1/metrics
```

Copia el JSON completo en **Lectura 1** de la escena smooth en la plantilla.

#### 3.4 Esperar 30 segundos mas y hacer la segunda lectura

Repite el mismo comando anterior. Copia en **Lectura 2**.
Esta lectura tiene min/max mas robustos porque ha acumulado mas frames.

#### 3.5 Rellenar la tabla de la escena smooth en la plantilla

Extrae estos valores del JSON de Lectura 2:

| Campo JSON | Columna de la tabla |
|---|---|
| `fps` | fps |
| `frameTimeAvgUs` | frameTimeAvgUs |
| `frameTimeMinUs` | frameTimeMinUs |
| `frameTimeMaxUs` | frameTimeMaxUs |
| `heapFreeBytes` | heapFreeBytes |
| `heapMinFreeBytes` | heapMinFreeBytes |
| `renderStackWords` | renderStackWords |

---

### PASO 4 — Escena 2: show

#### 4.1 Aplicar la escena

**HTTP:**
```bash
curl -X PATCH http://192.168.x.x/api/v1/state \
  -H "Content-Type: application/json" \
  -d "{\"power\":true,\"brightness\":170,\"effect\":\"triple_chase\",\"sectionCount\":4,\"effectSpeed\":56,\"effectLevel\":6,\"primaryColors\":[\"#FF0040\",\"#00FF80\",\"#0040FF\"],\"backgroundColor\":\"#000000\"}"
```

**Serial:**
```
PATCH /api/v1/state {"power":true,"brightness":170,"effect":"triple_chase","sectionCount":4,"effectSpeed":56,"effectLevel":6,"primaryColors":["#FF0040","#00FF80","#0040FF"],"backgroundColor":"#000000"}
```

#### 4.2 Resetear, esperar y capturar

Repite exactamente los pasos 3.2 → 3.5 para la seccion **show** de la plantilla.

---

### PASO 5 — Escena 3: aggressive

#### 5.1 Aplicar la escena

**HTTP:**
```bash
curl -X PATCH http://192.168.x.x/api/v1/state \
  -H "Content-Type: application/json" \
  -d "{\"power\":true,\"brightness\":220,\"effect\":\"audio_neon_eq\",\"sectionCount\":6,\"effectSpeed\":86,\"effectLevel\":9,\"primaryColors\":[\"#FF0000\",\"#00FF00\",\"#0000FF\"],\"backgroundColor\":\"#000000\"}"
```

**Serial:**
```
PATCH /api/v1/state {"power":true,"brightness":220,"effect":"audio_neon_eq","sectionCount":6,"effectSpeed":86,"effectLevel":9,"primaryColors":["#FF0000","#00FF00","#0000FF"],"backgroundColor":"#000000"}
```

> Nota: si el microfono I2S no esta conectado, el efecto audio_neon_eq funciona
> en modo fallback con nivel 0. Los FPS seran representativos del coste de calculo
> sin procesado de audio.

#### 5.2 Resetear, esperar y capturar

Repite exactamente los pasos 3.2 → 3.5 para la seccion **aggressive** de la plantilla.

---

### PASO 6 — Rellenar el resumen comparativo

Con los valores de Lectura 2 de cada escena, rellena la tabla **Resumen comparativo**
de la plantilla. Compara cada valor con el limite aceptable:

| Metrica | Limite aceptable | Si no se cumple |
|---|---|---|
| fps | >= 40 | Revisar coste del efecto o reducir LEDs |
| frameTimeAvgUs | <= 8000 | El render tarda mas de 8ms; riesgo de jitter |
| frameTimeMaxUs | <= 15000 | Picos esporadicos; revisar mutex/bloqueos |
| heapFreeBytes | >= 30000 | Poca memoria libre; riesgo al implementar F2/F4 |
| renderStackWords | >= 128 | Stack ajustado; aumentar kStackSize en main.cpp |

---

### PASO 7 — Guardar el baseline

Duplica la plantilla con el nombre de la version y la fecha:

```powershell
Copy-Item docs\baseline\BASELINE_CAPTURE_TEMPLATE.md `
          docs\baseline\BASELINE_v0.4.0_2026-07-29.md
```

Rellena el archivo copiado con todos los datos capturados.
La plantilla original queda en blanco para futuras capturas.

---

### PASO 8 — Marcar F0 como cerrada

Abre `docs\IMPLEMENTACION_EFECTOS_WLED_ROADMAP.md` y marca:

```
- [x] Guardar capturas/video corto para comparacion antes/despues.
```

Anota la referencia al archivo de baseline guardado en el campo **Notas** de F0.

---

## Referencia rapida de comandos

| Accion | HTTP | Serial |
|---|---|---|
| Ver metricas | `GET /api/v1/metrics` | `GET /api/v1/metrics` |
| Resetear contadores | `POST /api/v1/metrics/reset` | `POST /api/v1/metrics/reset` |
| Ver hardware | `GET /api/v1/hardware` | `GET /api/v1/hardware` |
| Aplicar escena | `PATCH /api/v1/state {json}` | `PATCH /api/v1/state {json}` |
| Ver estado actual | `GET /api/v1/state` | `GET /api/v1/state` |

---

## Respuesta JSON esperada de /api/v1/metrics

```json
{
  "fps": 62,
  "frameTimeAvgUs": 1240,
  "frameTimeMinUs": 980,
  "frameTimeMaxUs": 3100,
  "frameCount": 3720,
  "heapFreeBytes": 182400,
  "heapMinFreeBytes": 181200,
  "renderStackWords": 512,
  "uptimeMs": 62000
}
```

> Los valores anteriores son de referencia. Los tuyos dependen del hardware,
> numero de LEDs y efectos activos.
