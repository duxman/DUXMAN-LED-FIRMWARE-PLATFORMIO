# DUXMAN-LED-FIRMWARE-PLATFORMIO

Firmware principal de DUXMAN-LED para ESP32 (PlatformIO + Arduino + FreeRTOS).

## Estado

Repositorio activo y prioritario del proyecto.

## Estructura

- `platformio.ini`: perfiles de build por placa.
- `firmware/src`: codigo fuente principal.
- `firmware/include`: cabeceras del proyecto.
- `firmware/config`: configuraciones y defaults.
- `data/ui`: UI embebida servida por LittleFS.
- `tools/flash.ps1`: script de build/flash en Windows.

## Build rapido

```powershell
pio run -e esp32c3supermini
```

## Flash rapido

```powershell
pio run -e esp32c3supermini -t upload
```

## Documentacion

- Memoria tecnica: `docs/MEMORIA_PROYECTO.md`
- Checklist serial C3: `firmware/CHECKLIST_SERIAL_ESP32C3.md`

## Repos relacionados

- Meta-repo (deprecado): https://github.com/duxman/DUXMAN-LED-NEXT
- Android App: https://github.com/duxman/DUXMAN-LED-ANDROID-APP
- Mockup Python: https://github.com/duxman/DUXMAN-LED-MOCKUP-PYTHON
