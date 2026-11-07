---
name: "Java: Selenium y JMeter"
description: "Use when proposing or editing Selenium browser tests, JMeter load tests, .jmx plans, or external Java tests. Selenium 4 para el navegador; JMeter 5.6 en CLI para carga. No usar el IDE grabado, Selenium RC ni JMeter 2 como suite."
applyTo: "**/*.{java,gradle,kts,xml,properties,yml,yaml,jmx}"
---

# Pruebas externas y de carga

Criterio extraído el 2026-11-01 de *Selenium Programming Cookbook* y *JMeter Tutorial* (Java Code Geeks). El de Selenium enseña el IDE, grabar y exportar, y el servidor de aquella época. El de JMeter está hecho con JMeter 2.11 y Java 8. Se conserva la idea: un plan escrito, pruebas independientes, la carga se mide con un criterio, no a ojo. No se genera esa época.

Estas pruebas se proponen. No se añaden ni se ejecutan sin autorización. Nunca contra producción salvo un sí explícito que nombre el entorno.

## Selenium

Sirve para un flujo que el usuario ve en el navegador y que una unitaria o un slice no puede demostrar. No sirve para carga, ni para medir rendimiento, ni para sustituir la unitaria.

- Selenium 4, WebDriver y el protocolo W3C. Fuera de Spring Boot, la línea 4 actual: el 2026-10-09 la publicación Java era la 4.51.0. Dentro de Boot no se pisa el BOM: Boot 4.1 gestiona Selenium 4.43.0.
- El driver lo resuelve Selenium Manager. No se descarga un driver a mano ni se documenta una ruta fija como paso normal.
- Una prueba, un navegador nuevo, sin estado compartido con otra prueba. Al terminar se cierra el driver aunque la prueba falle.
- La pantalla se representa con un page object. El test dice el flujo; el page object dice cómo se localiza.
- Localizadores estables: id, CSS o nombre accesible. No un XPath copiado de una grabación.
- La espera es explícita (`WebDriverWait`) sobre una condición visible. No `Thread.sleep`. No se mezcla una espera implícita con una explícita.
- El plan dice qué flujo cubre, en qué navegador y qué datos necesita. Si no hay aplicación desplegada, la propuesta queda escrita y se dice que no se puede ejecutar.

No se automatiza un captcha, un segundo factor, el login de un tercero (correo, red social) ni se usa Selenium para recorrer todos los enlaces. El código HTTP no es la afirmación principal: lo es lo que el usuario ve.

## JMeter

Es la prueba de carga. No se sustituye por Selenium ni por una unitaria. Se propone siempre que el cambio tenga un endpoint, un lote, una consulta que puede crecer o un camino concurrente.

La propuesta, antes de crear el plan, dice:

- objetivo y qué pregunta responde (¿aguanta N usuarios?, ¿el p95 se mantiene?);
- entorno: local o el que el usuario nombre; nunca producción sin un sí que lo diga;
- escenario: método, ruta, cuerpo y datos;
- hilos, rampa y duración;
- criterio de éxito: tasa de error, un percentil (p95 o p99) y, si aplica, throughput;
- qué no mide;
- el comando de consola, pendiente de autorización.

Si se autoriza a crear el plan:

- El plan vive en el repositorio (`.jmx`), no solo en una sesión de la GUI. Junto a él, un texto con objetivo, carga, criterio y qué no mide.
- Herramienta: la última 5.6 publicada. El 2026-11-01 la página de cambios de `jmeter.apache.org` seguía en 5.6.3. No se inventa una 5.7 ni se genera un plan de la 2.11. Para ejecutarla se recomienda Java 17 o superior.
- La ejecución real es en consola (`-n`), con resultados CSV y el informe HTML. La GUI solo sirve para diseñar. Un listener de depuración, en particular el árbol de resultados, no forma parte de la ejecución de carga.
- Host, hilos, rampas y credenciales son propiedades (`-J` o un CSV). No hay secretos dentro del plan ni en el repositorio.
- Pocas afirmaciones, y baratas. Un script intensivo es Groovy JSR223, con el script compilado en caché. Dentro de ese script no se interpola `${var}`; se lee la variable con la API. No BeanShell ni JavaScript para esa carga.
- No se edita `jmeter.properties`. Lo local va a `user.properties` o a propiedades de la ejecución.
- No se suben los hilos hasta que el propio inyector descarta trabajo y el resultado deja de valer. Si el resultado sale de un portátil o de la GUI, no se presenta como capacidad del sistema.

## No generar

Selenium RC, `DefaultSelenium`, un test exportado del IDE como suite, la instalación descrita en el PDF, un driver descargado a mano como paso normal, JMeter 2.11, Java 8 como runtime de JMeter, la GUI como medición de carga, BeanShell o Nashorn en el camino de carga, secretos en el `.jmx`.
