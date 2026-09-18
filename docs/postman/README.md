# Pruebas API con Postman

## Importar

1. En Postman, selecciona **Import**.
2. Abre `DUXMAN-LED-API-F1.postman_collection.json`.
3. Edita la variable `baseUrl` de la coleccion con la URL del dispositivo, por ejemplo `http://192.168.4.1`.

## Orden recomendado

1. Ejecuta `00 - Descubrimiento` para comprobar conectividad, metadatos y catalogos.
2. Ejecuta `01 - F2 Paridad home` en orden. La respuesta de alta de secuencia guarda automaticamente `sequenceEntryId`.
3. Ejecuta `02 - Configuracion y diagnostico` para verificar las lecturas basicas.
4. Ejecuta `03 - Validacion de errores` para comprobar `invalid_parameter` e `invalid_json`.
5. Ejecuta `04 - API completa: configuracion` para probar mutaciones de red, microfono, GPIO, general, debug y configuracion completa.
6. Ejecuta `05 - API completa: sincronizacion` para probar estado, nodos, configuracion y modo.
7. Ejecuta `06 - API completa: perfiles` para probar listar, consultar, guardar, aplicar, establecer default, clonar y eliminar.
8. Ejecuta `07 - API completa: paletas, metricas y sistema` para probar CRUD de paletas, reset de metricas y reinicio.

Variables disponibles:

- `baseUrl`: URL base del dispositivo.
- `effect`: clave de efecto existente.
- `palette`: clave o identificador de paleta existente.
- `profileId`: reservado para las pruebas de perfiles.
- `sequenceEntryId`: ID de entrada usado por la eliminacion de secuencia.
- `savedPaletteKey`: reservado para las pruebas de paletas de usuario.

La coleccion cubre todas las rutas API v1 documentadas: estado, sistema,
configuracion, sincronizacion, perfiles, efectos, paletas, diagnostico,
metricas y metadatos. Incluye solicitudes representativas de los aliases
`POST` y `PATCH` contractuales, y la carpeta de errores comprueba el sobre
de respuesta comun.

Las carpetas 04, 06 y 07 modifican configuracion persistente. La peticion de
red puede reaplicar WiFi y `Reiniciar dispositivo (DISRUPTIVO)` corta la
conexion inmediatamente; ejecuta esas peticiones solo con hardware de
laboratorio y con valores revisados.

La coleccion usa scripts de prueba de Postman para validar HTTP, `ok`,
`updated` y respuestas de error.
