# Perfiles GPIO

## Que es un perfil

Un perfil es un snapshot completo de configuracion de dispositivo (network + gpio + microphone + general + sync), con foco en la topologia LED.

## Tipos

| Tipo | Descripcion |
|---|---|
| Integrados | Presets read-only incluidos en el firmware |
| Usuario | Hasta 8 perfiles persistidos en LittleFS |

## API de perfiles

| Metodo | Ruta | Descripcion |
|---|---|---|
| GET | `/api/v1/profiles` | Listar todos |
| GET | `/api/v1/profiles/get?id=<id>` | Obtener por id |
| POST, PATCH | `/api/v1/profiles/save` | Guardar o actualizar |
| POST, PATCH | `/api/v1/profiles/apply` | Aplicar perfil activo |
| POST, PATCH | `/api/v1/profiles/default` | Establecer perfil de arranque |
| POST, PATCH | `/api/v1/profiles/delete` | Eliminar perfil de usuario |
| POST, PATCH | `/api/v1/profiles/clone` | Clonar perfil existente |

> Las rutas antiguas `/api/v1/profiles/gpio*` son legacy; usar siempre `/api/v1/profiles*`.

## Ejemplo de perfil completo

```json
{
  "profile": {
    "id": "custom1",
    "name": "Mi perfil",
    "description": "Salida principal + digital auxiliar",
    "gpio": {
      "outputs": [
        { "pin": 8, "ledType": "ws2812b", "colorOrder": "GRB", "ledCount": 60 },
        { "pin": 16, "ledType": "digital", "colorOrder": "R", "ledCount": 1 }
      ],
      "power": {
        "powerLimitEnabled": true,
        "maxTotalCurrentmA": 2000,
        "milliAmpsPerLedBase": 20,
        "voltageSagCorrectionEnabled": false,
        "thermalThrottlingEnabled": false,
        "smartDimmingEnabled": false
      }
    },
    "network": { "wifi": { "mode": "ap" } },
    "microphone": { "enabled": false },
    "general": { "debugEnabled": false }
  }
}
```

## Notas

- Al aplicar un perfil, el driver LED se reconfigura en caliente sin reinicio.
- La limitacion de consumo (`power`) es software; no reemplaza proteccion de hardware.
