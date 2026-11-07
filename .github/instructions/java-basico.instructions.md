---
name: "Java: básico y calidad"
description: "Use when writing or reviewing Java code quality: clases, encapsulamiento, herencia, constructores, control de flujo, excepciones, hilos, documentación y ausencia de estado global. Obligatorio aunque las prácticas ya se conozcan. Extraído de Java desde Cero; no consultar el PDF."
applyTo: "**/*.{java,gradle,kts,xml,properties,yml,yaml}"
---

# Java básico, siempre presente

Extraído el 2026-10-31 desde *Java desde Cero*. Son criterios de calidad, no un tutorial. Se aplican en cada cambio aunque ya se conozcan. Texto propio. No se vuelve a leer el PDF.

El curso es de 1999 y enseña JDK 1.0, applets y AWT. Se conserva el criterio. No se genera código de esa época. Si choca con [java-version.instructions.md](java-version.instructions.md), manda la versión: mínimo Java 21, preferir lo estable de Java 25, sin APIs obsoletas ni inseguras.

## Objetos

- Modelar con clases e instancias. Los objetos se hablan por mensajes, es decir, llamadas a métodos, no metiendo mano en los datos de otro.
- Encapsular: los datos y los métodos que los usan viven en la misma clase. Campos privados, salvo una razón escrita.
- No hay variables globales. No crear clases de constantes o holders estáticos para simularlas. El estado vive en el objeto que lo posee.
- La herencia sirve para especializar, no para reutilizar tres líneas. La hija incorpora el contrato de la madre y redefine solo lo que cambia. Si no hay relación "es un", no heredar: componer. Si las variantes explotan en subclases, mirar Strategy o Decorator en las instrucciones de patrones.
- Un archivo, una clase pública. El nombre del archivo coincide con el de la clase, mayúsculas incluidas.
- El constructor deja el objeto en estado válido. No leer una variable local sin inicializar. No dejar un campo público para que "alguien lo rellene después" si el objeto no funciona sin ese dato.
- La sobrecarga se resuelve por número y tipo de argumentos. No sobrecargar para disfrazar operaciones distintas. Nombrar distinto si el efecto no es el mismo.
- Los primitivos (`int`, `boolean`, `long` y el resto) no son objetos. No envolverlos sin motivo. Para identidad y caché de envoltorios, vale lo dicho de Flyweight: no depender de `==` entre `Integer` creados con `new`.

## Control de flujo

- No existe `goto`. No simularlo con banderas opacas.
- `if`, `while`, `do-while` y `for` llevan llaves, aunque el cuerpo sea una línea.
- En `switch`, cada rama termina en `break`, `return` o `throw`, salvo una caída deliberada y comentada. Incluir `default` si un valor no previsto es un error.
- Un bucle tiene una condición que termina. No usar un bucle para ocultar una recursión o una máquina de estados que ya pide el patrón State.

## Errores

- Capturar la excepción concreta. No tragarla. No declarar `throws Exception` para no pensar.
- Si el llamador puede recuperarse, el `catch` deja el programa en un estado claro y dice qué pasó. Si no puede, propagar una excepción con contexto, no un mensaje suelto a la consola como única respuesta.
- El código que sigue a un fallo no se ejecuta. Liberar recursos en `finally` o, en Java actual, con try-with-resources. No dejar un stream, un lock o un hilo abierto porque la excepción "casi nunca sale".
- No usar `throws` en `main` como forma de ignorar un fallo de entrada o de fichero.

## Hilos

- Un hilo es un camino de ejecución, no otro programa. Compartir memoria exige una estrategia escrita.
- Dos hilos que leen y escriben el mismo dato pueden perder actualizaciones. No tocar estado mutable compartido sin sincronización, confinamiento o una estructura concurrente. Nombrar cuál de las tres se usa.
- Si la clase ya tiene otra responsabilidad, implementar `Runnable`. Extender `Thread` solo cuando el tipo es el hilo.
- `synchronized` protege el bloque que lee y escribe el estado, no un sitio lejano. No sincronizar "por si acaso" un método entero si el estado crítico es una línea: acotar y documentar el invariante.

## Documentación

- El lenguaje trae la herramienta de documentación. Toda clase pública y todo miembro de API llevan Javadoc: qué hace, qué estado exige y qué excepción sale. El comentario dice el porqué, no repite el nombre del método.
- Un cambio que no queda escrito en código y en el resumen no está terminado.

## No generar, aunque el curso lo enseñe

Applets, `java.applet`, AWT como interfaz por defecto (`Frame`, `Button`, `handleEvent`), `DataInputStream.readLine`, `Thread.stop`, `java.util.Observable` y el estilo de JDK 1.0. El criterio de calidad de esas lecciones sí se aplica, con las APIs actuales del proyecto.
