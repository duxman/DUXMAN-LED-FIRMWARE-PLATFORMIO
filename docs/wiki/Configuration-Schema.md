# Configuration Schema

Version: v0.6.3-alpha

La configuracion persistida se guarda en un unico fichero (`/config.json`) con estas secciones de primer nivel:

- `network`
- `gpio`
- `microphone`
- `general`
- `sync`

## Root JSON

```json
{
  "network": { ... },
  "gpio": { ... },
  "microphone": { ... },
  "general": { ... },
  "sync": { ... }
}
```

## network

```json
{
  "network": {
    "wifi": {
      "mode": "ap|sta|ap_sta",
      "apAvailability": "always|untilStaConnected",
      "connection": {
        "ssid": "string",
        "password": "string"
      }
    },
    "ip": {
      "ap": {
        "mode": "dhcp|static",
        "address": "ipv4",
        "gateway": "ipv4",
        "subnet": "ipv4"
      },
      "sta": {
        "mode": "dhcp|static",
        "address": "ipv4",
        "gateway": "ipv4",
        "subnet": "ipv4",
        "primaryDns": "ipv4|\"\"",
        "secondaryDns": "ipv4|\"\""
      }
    },
    "dns": {
      "hostname": "string"
    },
    "time": {
      "syncOnBoot": true,
      "ntpServer": "string"
    }
  }
}
```

## gpio

```json
{
  "gpio": {
    "outputs": [
      {
        "id": 0,
        "pin": 8,
        "ledCount": 60,
        "ledType": "digital|ws2812b|ws2811|ws2813|ws2815|sk6812|tm1814",
        "colorOrder": "R|G|B|W|RGB|GRB|BRG|RBG|GBR|BGR|RGBW|GRBW"
      }
    ],
    "power": {
      "powerLimitEnabled": false,
      "maxTotalCurrentmA": 5000,
      "milliAmpsPerLedBase": 60,
      "voltageSagCorrectionEnabled": false,
      "cableResistanceOhms": 0.1,
      "supplyVoltageNominal": 5.0,
      "minAcceptableVoltage": 4.5,
      "thermalThrottlingEnabled": false,
      "temperatureSensorPin": -1,
      "tempThrottleStartC": 50,
      "tempThrottleMaxC": 70,
      "smartDimmingEnabled": false,
      "preserveBlueFrequency": true,
      "priorityMode": 0
    }
  }
}
```

## microphone

```json
{
  "microphone": {
    "enabled": false,
    "source": "generic_i2c",
    "profileId": "DEFAULT|gledopto_gl_c_017wl_d",
    "sampleRate": 16000,
    "fftSize": 512,
    "gainPercent": 100,
    "noiseFloorPercent": 8,
    "noiseGateKnee": 35,
    "agcResponsePercent": 100,
    "pins": {
      "bclk": 21,
      "ws": 5,
      "din": 26
    }
  }
}
```

## general

```json
{
  "general": {
    "language": "en|es|fr|de|it",
    "regionCode": "US|ES|...",
    "debugEnabled": false,
    "heartbeatMs": 5000
  }
}
```

## sync

```json
{
  "sync": {
    "enabled": false,
    "mode": "off|local_effects|ledfx_realtime|cluster_sync",
    "role": "client|server",
    "inputProtocol": "ddp|e131",
    "ddpPort": 4048,
    "e131UniverseStart": 1,
    "e131UniverseCount": 1,
    "udpSyncPort": 21324,
    "groupMask": 1,
    "sourceTimeoutMs": 1500,
    "clockSmoothing": "off|soft"
  }
}
```

## Endpoints relacionados

- `GET /api/v1/config/all`
- `POST /api/v1/config/all`
- `GET/PATCH /api/v1/config/network`
- `GET/PATCH /api/v1/config/gpio`
- `GET/PATCH /api/v1/config/microphone`
- `GET/PATCH /api/v1/config/general`
- `GET/PATCH /api/v1/sync/config`
