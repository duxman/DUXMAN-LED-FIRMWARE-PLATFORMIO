---
applyTo: '**'
description: 'Guía operativa de Graphify para memoria, arquitectura y coordinación de agentes. Debe usarse como fuente de contexto antes de decidir cambios de código o arquitectura.'
---

# Instrucciones de Graphify para memoria arquitectónica y agentes

## 1. Propósito del grafo de conocimiento

El grafo de conocimiento de Graphify es la memoria estructural del repositorio. Debe considerarse una fuente de verdad del sistema antes de programar, refactorizar o responder sobre la arquitectura.

Cuando se ejecuta `graphify update .`, la herramienta procesa el AST del proyecto y genera en `graphify-out/`:

- `graph.json`: representación del grafo estructural (nodos, aristas, dependencias, herencia y llamadas).
- `GRAPH_REPORT.md`: resumen topológico optimizado para inyectarse como contexto de sistema o memoria de proyecto.
- `graph.html`: vista visual para inspeccionar acoplamientos y clusterización de módulos.

## 2. Regla operativa obligatoria

- Antes de generar código, refactorizar o responder sobre la estructura del proyecto, consulta siempre `graphify-out/GRAPH_REPORT.md` y, si hace falta, `graphify-out/graph.json`.
- Si el cambio toca arquitectura, dependencias, APIs, flujo, módulos, hardware, configuración o lógica de negocio, debe ejecutarse `graphify update .` y reflejarse en `graphify-out/` antes de cerrar la tarea.
- La memoria viva del proyecto no puede quedarse desactualizada mientras el código cambia.
- Cualquier decisión importante debe quedar registrada también en `README.md` y en la memoria del repositorio.

## 3. Patrón de automatización del grafo

Para evitar que la memoria se desactualice, el agente o el pipeline debe actualizar el grafo tras cambios relevantes en el disco.

Ejemplo mínimo:

```python
import subprocess


def sync_codebase_graph() -> str:
    """Actualiza el grafo del repositorio tras cambios de código."""
    try:
        subprocess.run(["graphify", "update", "."], check=True)
        return "Grafo de conocimiento actualizado correctamente."
    except Exception as exc:
        return f"Error al actualizar el grafo: {exc}"
```

## 4. Inyección de memoria en prompts de agentes

Antes de pedir un plan o una propuesta de cambios, se debe inyectar el contenido de `graphify-out/GRAPH_REPORT.md` como contexto de sistema o memoria de arquitectura.

Ejemplo:

```python
from pathlib import Path


def get_agent_system_prompt() -> str:
    graph_report = Path("graphify-out/GRAPH_REPORT.md").read_text(encoding="utf-8")
    return f"""Eres un agente experto en arquitectura de software.

Tienes acceso al Grafo de Conocimiento del proyecto:

--- INICIO DEL GRAFO ---
{graph_report}
--- FIN DEL GRAFO ---

REGLAS:
1. Revisa dependencias antes de proponer cambios.
2. Mantén compatibilidad con módulos con múltiples consumidores.
3. Reutiliza componentes existentes antes de duplicar lógica.
"""
```

## 5. Búsqueda semántica y consulta de dependencias

Para análisis local sin saturar el prompt principal, el agente puede consultar `graphify-out/graph.json` para obtener dependencias directas e indirectas de un nodo o módulo.

Ejemplo:

```python
import json


def get_node_dependencies(node_name: str) -> str:
    with open("graphify-out/graph.json", "r", encoding="utf-8") as handle:
        data = json.load(handle)

    edges = [
        edge
        for edge in data.get("links", [])
        if edge.get("source") == node_name or edge.get("target") == node_name
    ]
    return json.dumps(edges, indent=2)
```

## 6. Patrón recomendado para multi-agente

En flujos con múltiples agentes, la arquitectura debe organizarse así:

- Agente Planificador: consume `GRAPH_REPORT.md` y estima impacto, dependencias y alcance antes de tocar código.
- Agente Codificador: trabaja con el subgrafo de los módulos afectados, sin tener que cargar todo el código del repositorio en el prompt.
- Agente Revisor: ejecuta `graphify update .` tras los cambios y compara la nueva topología con la anterior para detectar acoplamientos no deseados, dependencias circulares o cambios de diseño que rompan compatibilidad.

## 7. Regla final del repositorio

Cada vez que se haga una modificación relevante, el agente debe dejar escrito en la memoria y en la documentación del proyecto:

- qué decisión se tomó,
- por qué se tomó,
- qué impacto tiene sobre el sistema,
- qué archivos o módulos se vieron afectados,
- y si el grafo se actualizó para reflejar el nuevo estado.

Si no se puede demostrar que Graphify está actualizado, la tarea no está completamente cerrada.
