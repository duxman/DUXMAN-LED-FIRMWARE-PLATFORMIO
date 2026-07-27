# Context Transfer From Monorepo

Este documento preserva el contexto operativo y técnico aprendido durante la separación de repositorios para que el equipo continúe sin pérdida de conocimiento.

## Decisiones tomadas

- El antiguo monorepo se dividió en repos especializados por dominio.
- El firmware de ESP32 es el repositorio principal y prioritario.
- El repositorio original queda como histórico/deprecado y archivado.

## Repositorios resultantes

- Firmware: https://github.com/duxman/DUXMAN-LED-FIRMWARE-PLATFORMIO
- Android: https://github.com/duxman/DUXMAN-LED-ANDROID-APP
- Mockup Python: https://github.com/duxman/DUXMAN-LED-MOCKUP-PYTHON
- Histórico/deprecado: https://github.com/duxman/DUXMAN-LED-NEXT

## Método de separación validado

1. Ejecutar split por carpeta con git subtree split.
2. Clonar cada rama split en carpeta nueva con single-branch.
3. Renombrar rama activa a main en cada repo nuevo.
4. Crear repos remotos dedicados y subir main.
5. Limpiar ramas split locales del repositorio histórico.

## Lecciones importantes

- Al archivar un repo en GitHub, queda en solo lectura.
- Si hay que aplicar una nota final (por ejemplo deprecación), hay que desarchivar temporalmente, publicar cambios y volver a archivar.
- Conviene mantener enlaces cruzados en README para guiar a contribuidores al repo correcto.

## Convenciones de mantenimiento en firmware

- Todo cambio funcional entra por ramas de feature o fix y merge a main.
- Mantener actualizada la memoria técnica en docs/MEMORIA_PROYECTO.md.
- Mantener este documento si hay cambios de estructura de repos o de ownership.

## Prioridades de continuidad

- Estabilidad de pipeline audio y sincronización.
- Robustez de configuración, perfiles y paletas.
- Pruebas de regresión para API v1 y flujo de arranque.
