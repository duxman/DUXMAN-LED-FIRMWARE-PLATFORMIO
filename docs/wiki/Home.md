# DUXMAN-LED-FIRMWARE-PLATFORMIO

Bienvenido a la documentacion tecnica del firmware ESP32 DUXMAN-LED (v0.6.6-beta).

## Contenido

- [Arquitectura general](./Architecture)
- [API REST v1](./API-v1)
- [Esquema de configuracion](./Configuration-Schema)
- [Compatibilidad de sincronizacion](./Sync-Compatibility)
- [Roadmap y evolucion](./Roadmap)
- [Historial de releases](./Release-History)
- [Perfiles GPIO](./GPIO-Profiles)
- [Paletas de color](./Palettes)
- [Preguntas frecuentes](./FAQ)

## Repositorios

| Proyecto | Repositorio |
|---|---|
| Firmware (este) | https://github.com/duxman/DUXMAN-LED-FIRMWARE-PLATFORMIO |
| Android App | https://github.com/duxman/DUXMAN-LED-ANDROID-APP |
| Mockup Python | https://github.com/duxman/DUXMAN-LED-MOCKUP-PYTHON |
| Monorepo historico | https://github.com/duxman/DUXMAN-LED-NEXT (deprecado) |

## Inicio rapido

```powershell
# Compilar para ESP32-C3 Super Mini
pio run -e esp32c3supermini

# Flash
pio run -e esp32c3supermini -t upload
```

Para detalles de hardware, configuracion y API consulta las secciones de esta wiki.
