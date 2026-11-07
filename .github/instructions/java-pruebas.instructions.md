---
name: "Java: fase de pruebas"
description: "Use when editing or creating Java code, tests, JUnit, Mockito, Selenium, JMeter, load tests, or Spring tests. Obligatorio proponer y documentar pruebas, sobre todo las de carga, antes de dar un cambio por cerrado."
applyTo: "**/*.{java,gradle,kts,xml,properties,yml,yaml,jmx}"
---

# Fase de pruebas

Las pruebas son una fase del cambio, no un añadido opcional al final. Un comportamiento nuevo o modificado no está terminado si no se ha propuesto, por escrito, cómo se comprueba.

El detalle de cada herramienta está en [java-junit.instructions.md](java-junit.instructions.md) y [java-pruebas-externas.instructions.md](java-pruebas-externas.instructions.md).

## Qué se propone siempre

Al planear un cambio que altera comportamiento, el plan incluye este bloque. Si un nivel no aplica, se dice y se da el motivo. No se omite en silencio.

- Unitaria: qué comportamiento, qué colaborador se sustituye y qué afirmación lo demuestra.
- Slice o integración: solo si la unitaria no puede probar el contrato (HTTP, persistencia, transacción).
- Navegador: solo si hay un flujo visible que ni la unitaria ni el slice cubren. No es una prueba de carga.
- Carga: escenario, datos, hilos, rampa, duración, criterio de éxito y entorno. Obligatoria de proponer si el cambio añade o altera un endpoint HTTP, un lote, una consulta que puede crecer, una escritura concurrente o un recurso compartido. Si no hay nada de eso, se escribe «carga: no aplica» y por qué.

## Qué entra en el incremento

- La unitaria del comportamiento que se acaba de escribir forma parte del mismo incremento, salvo que el usuario diga que no.
- Selenium y JMeter se proponen y no se añaden ni se ejecutan sin autorización. Una de carga puede tumbar un entorno. Una de navegador necesita un blanco desplegado y un navegador.
- Lanzar cualquier prueba es un comando. Se propone el comando exacto y se espera un sí. El silencio no autoriza.

## Cómo queda documentada

- Cada prueba dice qué comportamiento cubre y qué no cubre. En unitarias, el nombre del método y, si hace falta, `@DisplayName`. En la clase, un Javadoc corto con el alcance.
- Una prueba de carga, si se autoriza a crearla, lleva junto al plan un texto con objetivo, entorno, carga, criterio de éxito y qué no mide. No basta con dejarlo en el chat.
- El resumen del cambio lista las pruebas añadidas, las solo propuestas y el comando que las ejecutaría.
- No se da por medida una capacidad a partir de una ejecución en la GUI ni de un portátil. Si no se ha ejecutado, se dice que es una propuesta.
