---
name: "Java: JUnit y Mockito"
description: "Use when writing or editing Java unit tests, JUnit Jupiter, Mockito, Spring Boot tests, or test doubles. JUnit 6 y Mockito 5; no generar JUnit 4, Vintage, ni @MockBean."
applyTo: "**/*.{java,gradle,kts,xml,properties,yml,yaml}"
---

# Pruebas unitarias

Criterio extraído el 2026-11-01 de *JUnit Programming Cookbook* y *Mockito Programming Cookbook* (Java Code Geeks, 2017). Esos PDF enseñan JUnit 4, `@RunWith`, categorías, `initMocks` y grabación de tests. Se conserva la idea: prueba repetible, un comportamiento, el doble solo sustituye lo que no es el sujeto. No se genera su API.

## Versión

- Código Java sin Spring: JUnit Jupiter de la línea 6 actual. El 2026-11-01 la guía de usuario era la 6.1.3. No se fija una versión a ojo si el proyecto ya la declara.
- Proyecto Spring Boot: no se pisa el BOM. La documentación de Boot 4.1 gestiona `junit-jupiter` 6.0.3 y `mockito-core` 5.23.0. No se sube a mano a una Jupiter o un Mockito más nuevos.
- El motor Vintage está obsoleto. No se añade para escribir tests nuevos. Solo se menciona si hay una suite JUnit 3 o 4 que aún no se ha migrado, y esa migración es un incremento aparte.

## Cómo se escribe

- Una prueba, un comportamiento. No depende del orden ni de otra prueba.
- El nombre del método, en inglés, dice la condición y el resultado. `@DisplayName`, si se usa, va en el idioma del usuario.
- Se afirma el resultado observable. No se afirma un detalle interno que no forma parte del contrato.
- Varias entradas del mismo comportamiento van en una prueba parametrizada, no en copias del método.
- `@BeforeEach` y `@AfterEach` preparan y sueltan el estado de esa prueba. No hay estado mutable compartido entre pruebas.
- La excepción esperada se afirma con `assertThrows` (o el equivalente de AssertJ si el proyecto ya lo usa). No se usa un test que pasa porque lanzó cualquier error.
- No hay `Thread.sleep`, lectura de teclado ni `System.in` en una unitaria. Un comportamiento asíncrono se espera con una condición, no con una pausa fija. Añadir Awaitility es una dependencia y requiere autorización.
- Los recursos de la prueba se cierran con try-with-resources.

## Mockito

- Se sustituye el colaborador, nunca la clase que se está probando. Un objeto de valor o un record de datos no se mockea.
- El sujeto se construye en la prueba y recibe el doble por el constructor. No se añade un setter solo para poder inyectar el mock.
- `when` / `thenReturn` describe la respuesta que el comportamiento necesita. No se programa el doble para cada getter.
- `verify` solo la interacción que es el comportamiento. No se verifica cada llamada ni se usa `verifyNoMoreInteractions` por defecto: ata el test a un detalle.
- Un método void se prepara con `doThrow` o `doNothing` cuando esa reacción es el caso. No se finge un valor de retorno.
- `ArgumentCaptor` solo si el valor no se puede afirmar de otra forma más directa.
- `@Spy` solo cuando hace falta el objeto real y sustituir un método. Si basta un mock del colaborador, no hay spy.
- Inicialización: extensión de Jupiter (`MockitoExtension`) o mocks creados a mano. No `MockitoJUnitRunner`. `MockitoAnnotations.initMocks` no se genera; si hiciera falta el ciclo manual, es `openMocks` y se cierra.

## Spring Boot

- El slice más pequeño que prueba el contrato: web, datos o el slice de la tecnología, no un arranque completo para afirmar una regla de dominio.
- `@SpringBootTest` no configura solo MockMvc, WebClient ni TestRestTemplate. Para HTTP se usa `RestTestClient` y la autoconfiguración de prueba que corresponda.
- Doble en el contexto: `@MockitoBean` o `@MockitoSpyBean`. No `@MockBean` ni `@SpyBean`.
- Una prueba de persistencia tiene que llegar al almacén (flush o slice de Boot). Afirmar solo el objeto en memoria no prueba la consulta.

## No generar

`org.junit.Test`, `org.junit.Assert`, `@Before`, `@After`, `@RunWith`, `ExpectedException`, categorías o suites de JUnit 4, Vintage como destino, `concurrent-junit`, Hamcrest añadido solo porque el PDF de 2017 lo usa, `MockitoJUnitRunner`, `initMocks`, `@MockBean`, `@SpyBean`.
