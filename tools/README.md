# Tools

Scripts utilitarios para build, checks y automatizaciones futuras.

## flash.ps1

Script para compilar, subir y monitorizar firmware con deteccion automatica de puerto serie.
La deteccion combina `Win32_SerialPort` y `Get-PnpDevice -Class Ports`, porque algunos adaptadores USB-serie pueden no aparecer siempre en ambas fuentes. La autodeteccion prioriza puertos con estado `OK` y adaptadores reconocibles como `CH340`, `CP210x`, `FTDI` o `UART`.

Ejemplos desde la raiz del repo:

1. Listar puertos:
	powershell -ExecutionPolicy Bypass -File .\tools\flash.ps1 -ListPorts
2. Compilar perfil C3:
	powershell -ExecutionPolicy Bypass -File .\tools\flash.ps1 -Profile esp32c3supermini -Action build
3. Subir detectando puerto automaticamente:
	powershell -ExecutionPolicy Bypass -File .\tools\flash.ps1 -Profile esp32dev -Action upload
4. Subir y abrir monitor:
	powershell -ExecutionPolicy Bypass -File .\tools\flash.ps1 -Profile esp32s3 -Action upload-monitor
5. Forzar puerto manual:
	powershell -ExecutionPolicy Bypass -File .\tools\flash.ps1 -Profile esp32dev -Action upload -Port COM12

## LittleFS / plantillas UI

La UI embebida usa plantillas en `data/ui` cargadas desde LittleFS. Si cambias estos archivos, debes volver a subir el filesystem además del firmware cuando corresponda.

Ejemplo:
	platformio run -e esp32dev -t uploadfs

## esptool

Tambien se puede consultar la placa conectada directamente con `esptool` usando el Python del sistema.

Ejemplos:

1. Identificar chip:
	python -m esptool --chip auto --port COM7 chip-id
2. Leer flash detectada:
	python -m esptool --chip auto --port COM7 flash-id

## wiki-sync.ps1

Sincroniza la Wiki de GitHub con `docs/wiki`. El directorio `docs/wiki` es la fuente de verdad: al publicar, su contenido reemplaza el contenido remoto de la Wiki.

Ejemplos:

1. Traer contenido desde la wiki remota a local (solo para recuperar contenido):
	powershell -ExecutionPolicy Bypass -File .\tools\wiki-sync.ps1 -Mode pull
2. Enviar cambios locales a la wiki remota:
	powershell -ExecutionPolicy Bypass -File .\tools\wiki-sync.ps1 -Mode push
3. Commit local sin push:
	powershell -ExecutionPolicy Bypass -File .\tools\wiki-sync.ps1 -Mode push -NoPush
4. Forzar URL de wiki:
	powershell -ExecutionPolicy Bypass -File .\tools\wiki-sync.ps1 -Mode pull -WikiUrl https://github.com/<owner>/<repo>.wiki.git

Integracion adicional:

- Tasks de VS Code en `.vscode/tasks.json` para pull/push/check.
- El workflow `.github/workflows/wiki-sync-check.yml` publica `docs/wiki` a la wiki remota en cada push a `main` y al ejecutarse manualmente.

## wiki-check-drift.ps1

Ejecuta `wiki-sync.ps1 -Mode pull` y compara la wiki remota con `docs/wiki`. Es una herramienta manual; no la usa el workflow de publicación.

Ejemplo:
	powershell -ExecutionPolicy Bypass -File .\tools\wiki-check-drift.ps1

## test-api.py & test-api.ps1 (Suite de Pruebas de Integración)

Scripts de verificación automatizada extremo a extremo de toda la API REST v1 del firmware.
Valida: descubrimiento, metadatos, diagnóstico, `/api/v1/marcalog`, control en tiempo real, CRUD de paletas, CRUD de perfiles, configuración modular, pruebas interactivas (Web UI y LedFx) y manejo de errores (400, 405).
Envía marcas en tiempo real vía `/api/v1/marcalog` para que el monitor serie refleje la prueba exacta en ejecución.
Guarda automáticamente todo el historial de peticiones, respuestas y resultados en la carpeta `log/`:
- `log/test-api-YYYYMMDD-HHMMSS.log` (historial fechado).
- `log/test-api-latest.log` (acceso directo a la última ejecución).

Ejemplos en Python (sin librerías externas):
	python .\tools\test-api.py --url http://192.168.4.1
	python .\tools\test-api.py --url http://192.168.1.150 --verbose
	python .\tools\test-api.py --url http://192.168.1.150 --non-interactive

Ejemplos en PowerShell:
	powershell -ExecutionPolicy Bypass -File .\tools\test-api.ps1 -BaseUrl "http://192.168.4.1"
	powershell -ExecutionPolicy Bypass -File .\tools\test-api.ps1 -BaseUrl "http://192.168.1.150" -NonInteractive

## Registro del Monitor Serie en log/

Configurado en `platformio.ini` mediante:
- `monitor_dir = monitor`
- `monitor_filters = time, log2file`
Guarda automáticamente todo el tráfico del puerto serie en:
- `log/device-monitor-YYYYMMDD-HHMMSS.log`
- `log/device-monitor-latest.log`
