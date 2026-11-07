---
name: "Spring Data en Boot 4"
description: "Use when writing Spring Data, JPA, repositories, JDBC, MongoDB, Redis, Elasticsearch, transactions, or persistence queries. Spring Data sobre Spring Boot 4. No generar DAO XML, HibernateTemplate ni consultas concatenadas."
applyTo: "**/*.{java,gradle,kts,xml,properties,yml,yaml}"
---

# Spring Data

Extraído el 2026-10-31 desde *Spring Data Programming Cookbook* (2016) y las reglas de acceso a datos de *Spring Framework Cookbook* (2017). Esos PDF son recetas de Spring 4 con XML, JPA antiguo y clientes que Boot 4 ya sustituyó. Se conserva el criterio: el acceso a datos no se escribe a mano si el repositorio lo expresa. No se vuelve a leer el PDF.

En un proyecto nuevo se usa el starter de Spring Data que corresponda, en la línea de Spring Boot 4 (Spring Data 2025.1). No se declaran a la vez todos los almacenes del cookbook.

## Cómo se escribe

- El caso de uso llama a un repositorio. El repositorio no contiene la regla de negocio ni decide la respuesta HTTP.
- Si la operación es alta, baja, búsqueda o una consulta con nombre estable, se declara en una interfaz de Spring Data. No se escribe un DAO que abra la conexión a mano.
- El método de consulta dice lo que busca. Si el nombre derivado miente o se vuelve ilegible, se usa `@Query` con parámetros. No se concatena texto del usuario en JPQL, SQL ni en un filtro del almacén.
- Una lista que puede crecer se pagina. No devolver `findAll()` como API pública si el volumen no está acotado.
- La entidad persistente no es, por defecto, el contrato HTTP. El controlador o un mapper entrega el tipo de la API. No se abre la sesión en la vista para tapar un lazy load: la consulta trae lo que el caso de uso necesita, dentro de la transacción del servicio.
- JDBC que no cabe en un repositorio usa `JdbcClient`. `JdbcTemplate` o `NamedParameterJdbcTemplate` solo si el proyecto ya los usa. No se pide una conexión al `DataSource` para repetir el cierre y la excepción.
- El esquema de un proyecto que va a durar se versiona con Flyway o Liquibase, mediante su starter. `schema.sql` suelto solo si el usuario pide un ejemplo desechable, y se dice.
- Un test de persistencia comprueba que el SQL llega al almacén. No fiarse de un test transaccional que hace rollback antes del flush: forzar el flush o usar el slice de test de Spring Boot y afirmar el estado. El rollback del test no sustituye esa comprobación.

## Qué no se genera

- `LocalContainerEntityManagerFactoryBean` y el contexto XML de JPA como arranque. Boot configura el almacén. Si hace falta un bean propio, se documenta qué no cubre la autoconfiguración.
- `EntityManager` o `Session` inyectados para un CRUD que la interfaz de repositorio ya expresa.
- Consultas con el nombre de entidad en un string suelto cuando hay un método derivado o una consulta con nombre.
- El cliente antiguo de Elasticsearch (`elasticsearch-rest-client` y su sniffer). Boot 4 configura el cliente actual.
- Un UUID o un `BigDecimal` de MongoDB sin representación explícita. Boot 4 no elige una por defecto: se fija la propiedad y se deja escrito el motivo.
- Módulos de ejemplo del cookbook (Solr, Couchbase, Neo4j, el REST exporter) solo porque el PDF los trae. Se añade el almacén que el usuario pida.
