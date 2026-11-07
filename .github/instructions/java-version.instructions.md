---
name: "Java: mínimo 21, preferir 25"
description: "Use when choosing Java version, JDK APIs, compiler release, Maven or Gradle source level, or avoiding obsolete and insecure Java. El mínimo es el estándar Java 21. El último LTS es Java 25 y en código nuevo se prefiere lo nuevo y estable. No generar código obsoleto ni inseguro."
applyTo: "**/*.{java,gradle,kts,xml,properties,yml,yaml}"
---

# Versión de Java

Declarado el 2026-10-31 y corregido el mismo día. El último LTS es Java 25. Java 21 es el mínimo, no el techo. En código nuevo se usa lo nuevo cuando es estable y encaja mejor.

## Qué se escribe

- El estándar mínimo es Java 21. No generar estilo ni APIs de Java 8, 11 o 17 si 21 ya tiene un reemplazo: `record`, clases selladas, `switch` con patrones, bloques de texto, `instanceof` con patrón, `java.time`, try-with-resources, colecciones secuenciadas y, si el problema es mucha E/S bloqueante, hilos virtuales.
- Preferir lo estable de Java 25 frente al equivalente más viejo. Si una API o sintaxis de 22, 23, 24 o 25 es estable y expresa mejor el problema, usarla. No rebajar el código a 21 para evitar lo nuevo.
- No usar APIs en preview ni incubadoras, salvo que el usuario lo pida. Si no está claro que una API es estable en 25, decirlo y comprobar la documentación oficial del JDK 25 antes de usarla. No inventar la firma.
- En un proyecto nuevo, el compilador se fija en 25 (`release` 25). No dejar el valor por defecto del JDK de la máquina y no fijarlo en 21 si el proyecto puede ser 25.
- Si el proyecto ya fija otra versión, no cambiarla sin autorización: subir o bajar el `release` puede romper la compilación. Si está por debajo de 21, decirlo. El mínimo declarado es 21.

## Qué no se escribe

Obsoleto o inseguro no entra aunque todavía compile. Si el código existente ya lo usa, no se borra sin avisar.

- Applets, AWT como interfaz por defecto, `SecurityManager`, `finalize`, `Thread.stop`, `suspend` y `resume`, `Observable` y `Observer`.
- `java.util.Date` y `Calendar` en código nuevo. Fechas con `java.time`.
- `new Integer`, `new Long` y el resto de constructores de envoltorios. Usar `valueOf` o el autoboxing, sin depender de `==`.
- `Vector`, `Hashtable` y `Stack` en código nuevo, salvo una razón de compatibilidad escrita.
- APIs internas `sun.*` y `com.sun.*`.
- Criptografía débil para seguridad: MD5, SHA-1, DES, 3DES, RC4 y cifrado en modo ECB. Para contraseñas, un hash adaptativo (PBKDF2, bcrypt, scrypt o Argon2), no un hash rápido solo. Para tokens y claves, `SecureRandom`, no `Random` ni `Math.random`.
- TLS 1.0 y 1.1. No desactivar la verificación del certificado ni aceptar cualquier nombre de host.
- Secretos, contraseñas o claves en el código o en el repositorio.
- `ObjectInputStream` sobre datos que no sean de confianza. La deserialización nativa de Java no es un formato de entrada.
- Tragar excepciones de seguridad o dejar un fallo solo en `System.out`.

Si una API está retirada o marcada para retirada en Java 25, no se usa en código nuevo. Se elige el reemplazo estable de 25. Si el proyecto todavía no puede compilar con 25, el reemplazo tiene que existir como mínimo en el estándar de 21.
