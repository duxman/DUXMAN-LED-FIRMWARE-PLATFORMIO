---
name: "Java: registro y logging (SLF4J y Logback)"
description: "Use when writing or configuring Java logging, SLF4J, Logback, log levels, file appenders, rolling policies, info, debug, or trace. Niveles correctos y trazabilidad en fichero con rotación diaria y tamaño máximo."
applyTo: "**/*.{java,gradle,kts,xml,properties,yml,yaml}"
---

# Registro y logging

Práctica declarada el 2026-11-01. El logging en Java y Spring Boot es un requisito de observabilidad, trazabilidad y diagnóstico en producción. No se usan llamadas directas a consola (`System.out.println` o `System.err.println`) ni `e.printStackTrace()`.

## API estándar

- Se utiliza **SLF4J** como interfaz de logging (`org.slf4j.Logger`, `org.slf4j.LoggerFactory`).
- Si se usa Lombok y está autorizado en el proyecto: `@Slf4j`. Si no, se instancia de forma estándar y privada:
  ```java
  private static final Logger log = LoggerFactory.getLogger(MiClase.class);
  ```
- Implementación de logging recomendada y por defecto en Spring Boot: **Logback**.

## Uso estricto de niveles de log

Cada nivel tiene un propósito concreto y debe respetarse para evitar ruido o pérdida de información:

- **INFO**: Hitos y pasos importantes del negocio y del ciclo de vida de la aplicación.
  - Ejemplos: arranque/parada de servicios, inicio y finalización de procesos por lotes, procesamiento exitoso de una transacción principal, integraciones con sistemas externos establecidas.
- **DEBUG**: Trazabilidad detallada de los pasos de ejecución, ramas de lógica y flujo de datos intermedio.
  - Ejemplos: decisiones condicionales tomadas, IDs y cantidades de entidades procesadas, datos de entrada antes de invocar un servicio, tiempo de ejecución de pasos clave.
- **TRACE**: Monitorización exhaustiva de cada uno de los pasos a nivel granular.
  - Ejemplos: recorrido minucioso de bucles internos, payloads completos de peticiones/respuestas (sin datos sensibles), estado detallado de variables en cada paso interno del algoritmo.
  - En expresiones costosas o con concatenación de cadenas, comprobar siempre `if (log.isTraceEnabled())` o usar parámetros parametrizados `{}` de SLF4J.
- **WARN**: Situaciones inesperadas o degradadas que la aplicación ha podido gestionar sin abortar el flujo principal.
- **ERROR**: Fallos que impiden completar una operación. Debe registrarse el mensaje explicativo y la excepción completa como último parámetro (`log.error("Fallo al procesar pedido {}: {}", id, e.getMessage(), e);`).

## Trazabilidad obligatoria en fichero

El log **siempre tiene que quedar trazado en fichero**, independientemente de que además salga por consola.

La configuración del archivo de log debe garantizar:
1. **Rotación diaria**: un archivo de log nuevo por cada día natural (patrón `%d{yyyy-MM-dd}`).
2. **Límite máximo de tamaño de fichero**: corte y rotación por tamaño para evitar desbordar disco (ej. `10MB` o `50MB` por fichero).
3. **Límite total o retención de historial**: política de retención de días acumulados (ej. `maxHistory` de 30 días) y tamaño total acotado (`totalSizeCap`).

### Configuración en Spring Boot (`application.yml` o `logback-spring.xml`)

En `application.yml` básico:
```yaml
logging:
  file:
    name: logs/application.log
  logback:
    rollingpolicy:
      file-name-pattern: logs/application-%d{yyyy-MM-dd}.%i.log
      max-file-size: 10MB
      max-history: 30
      total-size-cap: 1GB
  level:
    root: INFO
    com.miempresa.miproyecto: DEBUG
```

Para control fino o entornos avanzados con appenders específicos para fichero, se define `logback-spring.xml` utilizando `RollingFileAppender` con `SizeAndTimeBasedRollingPolicy`:
```xml
<rollingPolicy class="ch.qos.logback.core.rolling.SizeAndTimeBasedRollingPolicy">
    <!-- Rotación diaria y numeración por tamaño -->
    <fileNamePattern>logs/app-%d{yyyy-MM-dd}.%i.log</fileNamePattern>
    <!-- Máximo tamaño por archivo -->
    <maxFileSize>10MB</maxFileSize>
    <!-- Retención máxima de días -->
    <maxHistory>30</maxHistory>
    <totalSizeCap>1GB</totalSizeCap>
</rollingPolicy>
```

## Reglas de seguridad y buenas prácticas

- **Sin secretos en logs**: Nunca imprimir contraseñas, tokens JWT, números de tarjeta (PAN), claves privadas ni PII (información de identificación personal sensible).
- **Siempre parametrizado**: Usar marcadores `{}` de SLF4J en lugar de concatenación con `+` para evitar cálculo innecesario de cadenas cuando el nivel está desactivado.

## Log especial de auditoría de rendimiento y tiempos (cuando se solicite)

Cuando el usuario lo indique para auditar velocidad, identificar cuellos de botella o analizar retardos en desarrollos críticos:
- **Logger o canal específico de rendimiento**: se utiliza un logger dedicado (ej. `AUDIT_PERFORMANCE` o `org.slf4j.LoggerFactory.getLogger("audit.performance")`) o un archivo separado (`logs/performance-audit.log`) para no mezclar métricas de tiempo con el log operativo general.
- **Formato del registro**:
  - Nombre de la función, endpoint o desarrollo auditado.
  - Tiempo total de ejecución (en ms o µs).
  - Desglose de tiempos por pasos o subtareas intermedias cuando aplique.
- **Mecanismos limpios**:
  - `System.nanoTime()` / `Instant` o `org.springframework.util.StopWatch` para medir tiempos sin dependencias pesadas.
  - Para métodos completos sin ensuciar la lógica de negocio, se puede emplear un interceptor o aspecto `@Around` (AOP) que capture nombre del método y duración total.
- **Ejemplo de salida de auditoría**:
  ```text
  [PERF-AUDIT] operacion=procesarOrden | paso_1_validacion=12ms | paso_2_consulta_bd=45ms | paso_3_pasarela_pago=180ms | total=237ms
  ```
- **Sin impacto en negocio**: Las mediciones de tiempo y auditoría de rendimiento no deben frenar el flujo principal ni modificar el resultado de las operaciones.

