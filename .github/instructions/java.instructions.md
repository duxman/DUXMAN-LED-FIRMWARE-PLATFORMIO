---
name: "Java: documentación y cambios seguros"
description: "Use when editing or creating Java code, Maven, Gradle, Spring, JUnit, Mockito, Selenium, JMeter, load tests, design patterns, or Java project files. Obligatorio: documentar todo, proponer pruebas, cambios pequeños, avisar antes de borrar código, pedir autorización si hay riesgo de perder funcionalidad, y aplicar las instrucciones ya extraídas de los PDF, no volver a leer el PDF."
applyTo: "**/*.{java,gradle,kts,xml,properties,yml,yaml}"
---

# Java en este repositorio

Estas reglas aplican al crear o modificar código y configuración Java. No se relajan por prisa ni por un pedido amplio.

## Premisas

- Todo queda escrito: Javadoc en tipos y miembros públicos o de API; comentario breve solo para el porqué que el código no muestra; resumen del cambio (qué, por qué, archivos, documentación añadida). Sin comportamiento implícito ni TODOs vacíos.
- Con cada desarrollo es obligatorio actualizar el RELEASE_NOTES (o CHANGELOG), el README y toda la documentación necesaria del proyecto. Un desarrollo no está terminado si no se actualizan estos documentos.
- Antes de eliminar código, avisar qué se quita, por qué y qué deja de existir. No borrar hasta autorización explícita.
- División en fases con cambios atómicos y pequeños. Todo desarrollo se descompone obligatoriamente en fases e incrementos atómicos para que cada parte se pueda probar de forma sencilla e independiente. Un pedido grande se parte. No reordenar, renombrar ni limpiar lo que no se pidió.
- Versionado y trazabilidad para auditoría en código y configuración. Todo elemento de código o configuración debe ir correctamente versionado y documentado mediante comentarios que no alteren la ejecución (encabezado Javadoc/comentario de archivo/bloque de configuración). Es obligatorio incluir la fecha (AAAA-MM-DD), versión y el detalle de los cambios realizados para auditoría. Toda versión válida autoincrementa su versión siguiendo la convención de software (SemVer: MAJOR.MINOR.PATCH).
- Registro y logging adecuado. El logging debe usar los niveles adecuados: INFO (pasos importantes del desarrollo y negocio), DEBUG (trazabilidad de pasos de ejecución) y TRACE (monitorización exhaustiva paso a paso). Es obligatorio que quede trazado en fichero con rotación diaria y tamaño máximo de fichero (SLF4J + Logback).
- Si un cambio puede quitar funcionalidad (firma, condición, valor por defecto, consulta, mapeo, flujo), parar, describir el riesgo y esperar autorización.
- Ningún comando sin autorización explícita, tampoco compilar o lanzar tests. Proponer el comando exacto, su efecto, y esperar un sí.
- Identificadores en inglés y convenciones habituales de Java, salvo que el código existente ya use otro criterio. Comunicación y documentación en el idioma del usuario.

## Prácticas declaradas

Añadir aquí cada práctica nueva que declare el usuario, con fecha. No inventar prácticas en esta lista. Si una práctica nueva contradice otra, preguntar cuál manda y escribir la resolución.

- 2026-10-31: Documentar siempre. Todo comportamiento y toda decisión de implementación quedan escritos en código y en el resumen del cambio.
- 2026-10-31: Seguir las reglas de programación y las buenas prácticas que el usuario vaya declarando. Hasta que estén en esta lista, no se dan por hechas.
- 2026-10-31: Avisar antes de eliminar código. El borrado requiere autorización.
- 2026-10-31: No hacer cambios grandes de una vez.
- 2026-10-31: Si hay riesgo de borrar funcionalidad, pedir autorización antes de editar.
- 2026-10-31: Ejecutar comandos solo con autorización explícita.
- 2026-10-31: Los PDF no se guardan como referencia viva. De cada PDF que el usuario indique se extraen solo las reglas necesarias y se escriben en `.github/instructions/`. No se copia el documento ni se vuelve a consultar para programar.
- 2026-10-31: Los patrones de diseño son obligatorios como criterio de calidad. Están en [java-patrones.instructions.md](java-patrones.instructions.md). Se usa un patrón solo si el problema encaja; si no, se dice que ninguno aplica.
- 2026-10-31: Lo básico de Java es obligatorio para la calidad del código, aunque ya se conozca. Está en [java-basico.instructions.md](java-basico.instructions.md).
- 2026-10-31: El último LTS es Java 25 y es lo que se prefiere en código nuevo. Java 21 es el estándar mínimo, no el techo. No se genera código obsoleto ni inseguro. Está en [java-version.instructions.md](java-version.instructions.md). No se cambia el `release` de un proyecto ya fijado sin autorización.
- 2026-10-31: El objetivo es Spring Boot 4, no los tutoriales viejos de Spring. El criterio está en [java-spring.instructions.md](java-spring.instructions.md) y [java-spring-data.instructions.md](java-spring-data.instructions.md). No se cambia la versión de Boot de un proyecto ya fijado sin autorización.
- 2026-11-01: En el ecosistema Spring, Spring Boot es siempre lo preferido por defecto frente a Spring suelto, por ser más sencillo y óptimo. No se montan aplicaciones con Spring Framework desnudo ni infraestructura manual que Boot resuelva con autoconfiguración y starters.
- 2026-11-01: Todo desarrollo propuesto debe dividirse obligatoriamente en fases compuestas por cambios atómicos y pequeños. Es requisito indispensable para que las piezas sean atómicas y se puedan probar de manera fácil, aislada y progresiva.
- 2026-11-01: Versionado obligatorio para auditoría. Todo archivo de código o configuración debe incluir en sus comentarios de cabecera la versión (con autoincremento semántico SemVer en cada versión válida), la fecha del cambio y la descripción de los cambios realizados, garantizando trazabilidad y auditoría sin afectar la ejecución.
- 2026-11-01: Con cada desarrollo es obligatorio actualizar el RELEASE_NOTES (o CHANGELOG), el README y toda la documentación necesaria del proyecto. Un desarrollo no se da por cerrado si no se actualizan estos documentos.
- 2026-11-01: Logging estructurado en niveles y trazabilidad en fichero. Uso estricto de INFO (pasos importantes del desarrollo), DEBUG (trazado de pasos) y TRACE (monitorización detallada de cada paso). Obligatorio configurar salida y trazado en fichero con rotación diaria y límite de tamaño máximo. Detalle en [java-logging.instructions.md](java-logging.instructions.md).
- 2026-11-01: Log especial de auditoría de rendimiento y tiempos. Cuando el usuario lo solicite para estudiar retardos, velocidad o puntos críticos, se implementará un registro específico de auditoría con nombre de la función/desarrollo, tiempo total y desglose por pasos, sin alterar la lógica de negocio.
- 2026-11-01: Registro de decisiones y el porqué (ADR / Memoria). Es obligatorio registrar en la memoria del repositorio y en `doc/decisiones-arquitectura.md` cada decisión tomada, su justificación y su impacto técnico, para permitir su reutilización, optimizar futuros agentes y auditar la evolución de las reglas.
- 2026-11-01: Las pruebas son una fase del cambio. Se proponen y se documentan siempre; la de carga se propone aparte y no se omite en silencio. Unitarias con JUnit Jupiter 6 y Mockito 5; navegador con Selenium 4; carga con JMeter 5.6 en consola. Está en [java-pruebas.instructions.md](java-pruebas.instructions.md), [java-junit.instructions.md](java-junit.instructions.md) y [java-pruebas-externas.instructions.md](java-pruebas-externas.instructions.md). No se ejecutan sin un sí.

## Cómo procesar un PDF nuevo

Cuando el usuario señale un PDF:

1. Extraer solo las reglas que cambian cómo se escribe código de calidad.
2. Redactarlas en palabras propias, accionables y cortas, en un archivo `.github/instructions/java-<tema>.instructions.md` con `applyTo` de archivos Java.
3. Registrar aquí la fecha, el tema y el archivo. No dejar el PDF como fuente de programación.
4. No copiar capítulos, no guardar otra copia del PDF y no dejar un volcado de texto en el repositorio.
5. Si el documento está desfasado, conservar el criterio de calidad y marcar las APIs que no deben generarse.

## Conocimiento ya extraído

- 2026-10-31: Patrones de diseño, desde *Java Design Patterns*. Archivo: `java-patrones.instructions.md`.
- 2026-10-31: Java básico y calidad, desde *Java desde Cero*. Archivo: `java-basico.instructions.md`.
- 2026-10-31: Versión mínima Java 21 y último LTS Java 25, que se prefiere. Archivo: `java-version.instructions.md`.
- 2026-10-31: Spring Boot 4, desde *Spring in Action* (3.ª ed., 2011), *Spring Framework Cookbook* (2017) y *Spring Interview Questions* (2014). Criterio sí; XML, `javax` y el arranque de esa época no. Archivo: `java-spring.instructions.md`.
- 2026-10-31: Spring Data, desde *Spring Data Programming Cookbook* (2016) y la parte de acceso a datos del cookbook de Spring. Archivo: `java-spring-data.instructions.md`.
- 2026-11-01: Fase de pruebas, propuesta y documentación, sobre todo la de carga. Archivo: `java-pruebas.instructions.md`.
- 2026-11-01: JUnit y Mockito, desde *JUnit Programming Cookbook* y *Mockito Programming Cookbook* (2017). Criterio sí; JUnit 4 y `initMocks` no. Archivo: `java-junit.instructions.md`.
- 2026-11-01: Selenium y JMeter, desde *Selenium Programming Cookbook* y *JMeter Tutorial*. Criterio sí; IDE grabado, Selenium RC y JMeter 2.11 no. Archivo: `java-pruebas-externas.instructions.md`.
- 2026-11-01: Niveles de log (INFO, DEBUG, TRACE) y trazado en fichero con rotación diaria y tamaño máximo. Archivo: `java-logging.instructions.md`.
- 2026-11-01: Registro de decisiones de arquitectura y su justificación (el porqué). Archivo: `doc/decisiones-arquitectura.md`.
