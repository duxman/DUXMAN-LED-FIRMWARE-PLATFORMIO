---
name: "Spring Boot 4"
description: "Use when writing Spring, Spring Boot, Spring MVC, dependency injection, beans, transactions, configuration, starters, or tests. El objetivo es Spring Boot 4, no los tutoriales viejos. Mínimo Java 21, preferir Java 25. No generar XML de aplicación, javax ni APIs retiradas."
applyTo: "**/*.{java,gradle,kts,xml,properties,yml,yaml}"
---

# Spring Boot 4

Extraído el 2026-10-31 desde *Spring in Action, Third Edition* (2011), *Spring Framework Cookbook* (2017) y *Spring Interview Questions* (2014). Esos PDF enseñan Spring 3 y 4 con XML. Se conserva el criterio. No se genera su forma de arrancar una aplicación. No se vuelve a leer el PDF.

El objetivo de un proyecto nuevo es la última línea de mantenimiento de Spring Boot 4, sobre Spring Framework 7. Boot 4 admite Java 17; aquí el mínimo sigue siendo Java 21 y se prefiere Java 25. No fijar Boot 2 ni Boot 3. Si el proyecto ya fija otra versión, no cambiarla sin autorización.

## Preferencia por Spring Boot

Siempre que se trabaje en el ecosistema Spring, **Spring Boot es la opción por defecto y preferida** frente a Spring Framework suelto ("a pelo"):
- Es más sencillo, rápido de desplegar y óptimo para el ciclo de vida de la aplicación.
- No se monta infraestructura, servidores web externos ni cableado manual de contexto que Spring Boot ya resuelva mediante starters modulares y autoconfiguración orientada a producción.
- No se crea un proyecto Spring desnudo (sin Boot) salvo que el usuario lo pida explícitamente.

## Cómo se compone

- El contenedor crea los colaboradores y los inyecta. Una clase no hace `new` de otro bean de Spring.
- Inyección por constructor para lo que el objeto necesita para existir. No inyectar por campo. No añadir un setter solo para el test: el test pasa el doble por el constructor.
- Un bean singleton no es seguro por ser singleton. No guardar en sus campos el estado de una petición.
- El alcance por defecto es singleton. `prototype`, `request` o `session` solo si cada uso necesita otra instancia, y se deja escrito por qué.
- Estereotipos según el papel: `@Controller` o `@RestController` en el borde web, `@Service` en el caso de uso, `@Repository` en el acceso a datos. `@Component` solo si no hay un estereotipo mejor.
- Las anotaciones de Spring van en la clase concreta y en sus métodos, no en la interfaz. Java no hereda esas anotaciones al implementar.
- La configuración de aplicación es Java y propiedades (`application.yml` o `application.properties`), no `applicationContext.xml`. Un `pom.xml` o `build.gradle` no es configuración de beans.
- Los valores que cambian por entorno salen del código: propiedades o variables de entorno. No dejar URL, usuario ni secreto en una clase ni en el repositorio.
- Un controlador agrupa un caso de uso, no toda la aplicación. Traduce la petición, llama al servicio y arma la respuesta. La regla de negocio no vive en el controlador.
- La transacción se abre en el servicio, en métodos públicos. Una consulta se marca `readOnly`. No anotar el controlador ni abrir la transacción en el repositorio si ya hay servicio. Una llamada interna del mismo bean no abre transacción.
- El servicio lanza excepciones no comprobadas de dominio cuando el fallo debe deshacer la transacción. No declarar `throws Exception` para evitar decidir. No tragar la excepción.
- Lo transversal (transacción, seguridad, métrica) no se copia en cada método. Si hace falta un aspecto propio, se nombra y se documenta el punto de corte. No envolver todo en un aspecto por costumbre.

## Qué no se genera

Aunque el PDF lo enseñe, no entra en código nuevo. Si el código existente ya lo usa, no se borra sin avisar.

- Arranque con `ClassPathXmlApplicationContext`, `BeanFactory` o `XmlBeanFactory`. En Boot, la aplicación arranca por su clase `@SpringBootApplication`.
- `applicationContext.xml`, `<context:component-scan>` y el espacio de nombres XML como forma de definir la aplicación.
- `javax.*` de Java EE (`javax.persistence`, `javax.servlet`, `javax.validation`, `javax.inject`). El paquete es `jakarta.*`.
- `@Required`, `@Autowired` en campo, inyección XML `byName` o `byType`, y `default-autowire`.
- `DriverManagerDataSource` fuera de un ejemplo desechable. El pool por defecto de Boot es HikariCP.
- `HibernateTemplate` y `HibernateDaoSupport`.
- `log4j.xml` de Log4j 1. El log va por la configuración de Boot.
- JSP, Spring Web Flow, RMI, Hessian, Burlap y JMX como interfaz por defecto.
- `WebSecurityConfigurerAdapter`. La seguridad nueva es un bean `SecurityFilterChain`.
- `spring-boot-starter-web`. El starter web actual es `spring-boot-starter-webmvc`. Los starters `oauth2-*` sin el prefijo `security-` están en desuso.
- Starters classic (`spring-boot-starter-classic`) en un proyecto nuevo. Cada tecnología lleva su starter y, en test, el starter de test de esa tecnología.
- Undertow. Boot 4 exige Servlet 6.1 y no lo soporta.
- Jackson 2 como mapper de la aplicación (`com.fasterxml.jackson.databind.ObjectMapper`). El mapper nuevo es Jackson 3, `tools.jackson`. Las anotaciones siguen en `com.fasterxml.jackson.annotation`.
- `@MockBean` y `@SpyBean`. En test se usa `@MockitoBean` y `@MockitoSpyBean`.
- `RestTemplate` en código nuevo. Para un cliente HTTP propio, `RestClient` o una interfaz `@HttpExchange`. `JmsTemplate` solo si el proyecto ya lo usa; si no, `JmsClient`.
- Reintentos con Spring Retry si Spring Framework 7 ya cubre el caso con `org.springframework.core.retry`. No añadir la dependencia vieja sin decirlo.

## Referencias oficiales y documentación viva

Para consultar cualquier duda de diseño, starter o API en Spring Boot 4, las fuentes oficiales son:

1. **Documentación de referencia**:
   - Índice general: `https://docs.spring.io/spring-boot/index.html`
   - Dependencias gestionadas (BOM coordinadas): `https://docs.spring.io/spring-boot/appendix/dependency-versions/coordinates.html`
   - Propiedades de configuración comunes: `https://docs.spring.io/spring-boot/appendix/application-properties/index.html`
2. **Guías de migración y notas de versión (GitHub Wiki)**:
   - Notas de versión Boot 4.0: `https://github.com/spring-projects/spring-boot/wiki/Spring-Boot-4.0-Release-Notes`
   - Guía de migración Boot 4.0: `https://github.com/spring-projects/spring-boot/wiki/Spring-Boot-4.0-Migration-Guide`
   - Spring Framework 7 Release Notes: `https://github.com/spring-projects/spring-framework/wiki/Spring-Framework-7.0-Release-Notes`
3. **Pila base de Spring Boot 4**:
   - Spring Framework 7 (núcleo) y Spring Data 2025.1+ / 4.x.
   - Jakarta EE 11 (Servlet 6.1, Persistence 3.2, Validation 3.1). Todo bajo paquete `jakarta.*`.
   - Jackson 3 (`tools.jackson.core:*` 3.x para databind/core; anotaciones en `com.fasterxml.jackson.annotation`).
   - Servidores embebidos: Apache Tomcat 11 y Jetty 12 (ee11). Undertow no soportado.
   - Pool de conexiones JDBC: HikariCP 7.x.
   - HTTP Client: `RestClient` y `@HttpExchange` (no usar `RestTemplate`).
   - Pruebas: `@MockitoBean` / `@MockitoSpyBean` (`org.springframework.test.context.bean.override.mockito`), `RestTestClient` con `@AutoConfigureRestTestClient`.

