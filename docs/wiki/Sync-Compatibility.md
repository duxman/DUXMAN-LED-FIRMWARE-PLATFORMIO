# Compatibilidad de sincronizacion (LedFx, WLED, E1.31, MQTT)

Version: v0.6.4-beta

## Resumen rapido

Este firmware soporta entrada de frames externos por protocolos estandar de iluminacion:

- DDP (recomendado para LedFx)
- E1.31 / sACN (universos DMX sobre UDP)

Tambien soporta sincronizacion de estado entre nodos del propio firmware mediante `cluster_sync`.

## Matriz de compatibilidad actual

| Integracion / Protocolo | Estado | Modo recomendado | Notas clave |
|---|---|---|---|
| LedFx via DDP | Soportado | `ledfx_realtime` + `client` + `inputProtocol=ddp` | Puerto DDP configurable (`ddpPort`, default 4048) |
| LedFx via E1.31 | Soportado | `ledfx_realtime` + `client` + `inputProtocol=e131` | Puerto E1.31 fijo: 5568; universos configurables |
| WLED nativo (JSON realtime / protocolo propio) | No implementado | N/A | Interoperabilidad solo por protocolos comunes (DDP/E1.31) |
| MQTT para streaming/sync de pixeles | No implementado | N/A | No hay transporte MQTT en el stack de Sync actual |
| Cluster entre nodos DUXMAN | Soportado | `cluster_sync` + `server/client` | Sync de estado + reloj de fase + grupo |

## Requisitos para LedFx (deteccion y recepcion)

Para recibir datos de LedFx de forma estable:

1. Configurar `mode = ledfx_realtime`.
2. Configurar `role = client`.
3. Seleccionar `inputProtocol = ddp` o `e131`.
4. Ajustar puertos/universos:
   - DDP: `ddpPort` (normalmente 4048)
   - E1.31: `e131UniverseStart` y `e131UniverseCount`
5. Verificar conectividad en:
   - `GET /api/v1/sync/state`
   - `GET /api/v1/sync/connected`

## Ejemplos de configuracion por API

### LedFx + DDP

```http
PATCH /api/v1/sync/config
Content-Type: application/json

{
  "sync": {
    "enabled": true,
    "mode": "ledfx_realtime",
    "role": "client",
    "inputProtocol": "ddp",
    "ddpPort": 4048,
    "sourceTimeoutMs": 1500
  }
}
```

### LedFx + E1.31

```http
PATCH /api/v1/sync/config
Content-Type: application/json

{
  "sync": {
    "enabled": true,
    "mode": "ledfx_realtime",
    "role": "client",
    "inputProtocol": "e131",
    "e131UniverseStart": 1,
    "e131UniverseCount": 4,
    "sourceTimeoutMs": 1500
  }
}
```

### Cluster DUXMAN (emisor)

```http
PATCH /api/v1/sync/config
Content-Type: application/json

{
  "sync": {
    "enabled": true,
    "mode": "cluster_sync",
    "role": "server",
    "udpSyncPort": 21324,
    "groupMask": 1,
    "sourceTimeoutMs": 1500,
    "clockSmoothing": "soft"
  }
}
```

### Cluster DUXMAN (receptor)

```http
PATCH /api/v1/sync/config
Content-Type: application/json

{
  "sync": {
    "enabled": true,
    "mode": "cluster_sync",
    "role": "client",
    "udpSyncPort": 21324,
    "groupMask": 1,
    "sourceTimeoutMs": 1500,
    "clockSmoothing": "soft"
  }
}
```

## Diagnostico recomendado

Checklist de validacion cuando no entra video/frames:

1. Confirmar que el dispositivo y la fuente (LedFx u otra) estan en la misma red/VLAN.
2. Revisar `sync.mode`, `sync.role` e `sync.inputProtocol` en `GET /api/v1/sync/state`.
3. Verificar crecimiento de `stats.packetsReceived` en `GET /api/v1/sync/state`.
4. Si `sourceAlive=false`, revisar firewall, puerto y origen de paquetes UDP.
5. En E1.31, revisar que el rango de universos enviado coincida con `e131UniverseStart` y `e131UniverseCount`.
6. En DDP, verificar el puerto real de salida del emisor contra `ddpPort`.

## Alcance y limites actuales

- Este documento refleja el estado real del firmware en la rama principal.
- DDP y E1.31 son rutas de entrada soportadas para pixel streaming.
- MQTT y protocolo nativo WLED no forman parte del stack de sincronizacion actual.

## Referencias

- [Configuration-Schema](./Configuration-Schema)
- [API-v1](./API-v1)
- [Architecture](./Architecture)
