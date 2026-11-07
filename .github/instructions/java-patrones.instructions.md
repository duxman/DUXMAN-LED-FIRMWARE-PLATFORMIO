---
name: "Java: patrones de diseño"
description: "Use when designing, creating, or changing Java object structure, creational, structural, or behavioral design patterns, factories, builders, adapters, observers, strategies, or class collaboration. Obligatorio para calidad de diseño. No consultar el PDF: estas reglas ya están extraídas."
applyTo: "**/*.{java,gradle,kts,xml,properties,yml,yaml}"
---

# Patrones de diseño

Extraído el 2026-10-31 desde *Java Design Patterns* (Rohit Joshi). Texto propio para programar. No se vuelve a leer el PDF. No se copia el libro.

Un patrón es una solución reutilizable a un problema recurrente, no un diseño terminado que se pegue tal cual. Se usa solo si el problema encaja con la regla de abajo y con el código existente. Si no encaja, se dice que ninguno aplica y se escribe el código más simple. No se mete un patrón en un cambio pequeño que no lo necesita.

Al usarlo, nombrarlo en el plan y en el Javadoc del tipo que lo encarna, con el problema que resuelve.

## Cómo elegir

1. Clasificar el problema: creación de objetos, composición de estructura, o reparto de comportamiento.
2. Quedarse con el patrón cuya condición de uso encaje. Si dos encajan, elegir el que menos clases nuevas añada y decir el descartado.
3. Si la condición de uso y un ejemplo general chocan, manda la condición de uso.
4. Preferir la facilidad equivalente del JDK cuando ya resuelve el caso. No reimplementarla.

## Creación

- **Singleton.** Hace falta exactamente una instancia y un punto de acceso conocido. Si además debe poder especializarse sin que el cliente cambie, dejarlo escrito. Implementación preferida: un enum. No usarlo como bolsa global de estado. No usarlo solo porque haya muchos objetos parecidos: eso es Flyweight.
- **Builder.** El algoritmo de construcción es independiente de las piezas, o la misma construcción debe producir representaciones distintas. No usarlo para un objeto de dos campos. En el JDK: `StringBuilder`, `StringBuffer`, `Appendable`, los `put` de `ByteBuffer` y afines.
- **Factory Method.** La clase no puede anticipar la clase concreta que debe crear, o quiere que una subclase decida el producto. En el JDK: `Calendar.getInstance`, `NumberFormat.getInstance`, `ResourceBundle.getBundle`, `Charset.forName`.
- **Abstract Factory.** El sistema no debe depender de cómo se crean los productos, y una familia de productos relacionados debe usarse junta. No usarlo para un solo producto.
- **Prototype.** La clase concreta se elige en ejecución, clonar unos pocos estados conocidos evita una jerarquía de fábricas paralela, o hace falta independizar al sistema de cómo se crean los productos. En el JDK: `Cloneable` y `Object.clone`. Si `clone` rompe el encapsulamiento, preferir un constructor de copia y decirlo.

## Estructura

- **Adapter.** Ya existe una clase cuya interfaz no es la que el cliente necesita, o adaptar cada subclase por herencia es impracticable. Preferir el adapter de objeto. Java no tiene herencia múltiple: no simular un class adapter con ella.
- **Facade.** Hace falta una entrada simple a un subsistema complejo, o hay que desacoplar clientes de muchas clases de implementación. La fachada no esconde el subsistema: quien necesite el detalle sigue pudiendo usarlo. No añadir comportamiento de negocio dentro de la fachada.
- **Composite.** Hay una jerarquía parte-todo y el cliente debe tratar igual la hoja y la composición.
- **Bridge.** La abstracción y la implementación no deben quedar atadas para siempre; ambas deben poder extenderse por separado, y un cambio de implementación no debe obligar a recompilar clientes. También cuando la implementación se elige o se cambia en ejecución.
- **Proxy.** Hace falta una referencia más sofisticada que un puntero: representante remoto, creación perezosa de un objeto caro, o control de acceso. No envolver un objeto local barato.
- **Decorator.** Hay que añadir o quitar responsabilidades a un objeto en ejecución, sin afectar a los demás, y la herencia produciría una explosión de subclases. En el JDK: `BufferedInputStream`, `DataInputStream`, `BufferedOutputStream`, `ZipOutputStream`.
- **Flyweight.** Hay muchísimos objetos, el coste de memoria es alto, casi todo el estado puede ser extrínseco y la identidad del objeto no importa. No usarlo con pocos objetos. En el JDK: `Integer.valueOf` y los `valueOf` de `Boolean`, `Byte`, `Character`, `Short` y `Long`.

## Comportamiento

- **Observer.** Un cambio en un objeto debe avisar a otros cuyo número y tipo no se conocen, sin acoplarlos. No usar `java.util.Observer` ni `Observable`: están obsoletos. Una interfaz de oyente pequeña, o el mecanismo que el proyecto ya use.
- **Mediator.** Varios objetos se hablan de forma enredada y cuesta reutilizarlos porque cada uno conoce a muchos. Centralizar el protocolo. No usarlo si el diálogo es de dos objetos.
- **Chain of Responsibility.** Más de un objeto puede atender la petición, el receptor no se conoce de antemano y el conjunto de manejadores se arma en ejecución. En el JDK: `Logger.log`, filtros `doFilter`.
- **Memento.** Hay que guardar y restaurar estado sin abrir el encapsulamiento del objeto. No exponer los campos internos para "guardar fácil".
- **Template Method.** Las partes fijas de un algoritmo se escriben una vez y las variables quedan en subclases, o hay que limitar las extensiones a puntos de enganche. Si lo que varía es el algoritmo entero, es Strategy, no Template. En el JDK: `Collections.sort`, `InputStream.read` y `skip`.
- **State.** El comportamiento depende del estado y cambia en ejecución, o el mismo `switch` de estado se repite en varias operaciones. Cada rama pasa a un objeto de estado.
- **Strategy.** Hay variantes de un algoritmo, o una clase acumula comportamientos como condicionales. El cliente no debe conocer los datos internos del algoritmo. En el JDK: `Comparator`.
- **Command.** La acción debe ser un objeto: para parametrizar, encolar, ejecutar más tarde, deshacer o registrar y repetir. El deshacer exige una operación inversa y un historial; no prometer deshacer si no se escribe.
- **Interpreter.** Hay un lenguaje pequeño que se representa como árbol. Si la gramática es grande o el rendimiento importa, no interpretar el árbol a mano: decirlo y no forzar el patrón.
- **Iterator.** Hay que recorrer un agregado sin mostrar su representación, o unificar el recorrido de estructuras distintas. Usar `Iterator` o el for mejorado. No escribir un iterador propio si la estructura ya es una colección estándar.
- **Visitor.** Hay muchas operaciones distintas sobre una estructura de tipos que casi no cambia, y no se quiere llenar esas clases con operaciones ajenas. Si la estructura cambia a menudo, no usarlo: cada cambio obliga a tocar todos los visitantes.
