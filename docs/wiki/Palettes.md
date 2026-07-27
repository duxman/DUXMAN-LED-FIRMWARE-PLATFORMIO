# Paletas de color

## Paletas predefinidas

Catalogo curado de 12+ paletas de 3 colores principales, clasificadas por estilo:
`warm`, `cold`, `neon`, `pastel`, `high-contrast`, `party`.

Son read-only y no pueden eliminarse.

## Paletas de usuario

- Creadas, editadas y eliminadas desde la UI o API.
- Persisten en `/user-palettes.json` (LittleFS).
- Aplicables en caliente en cualquier efecto.

## API de paletas

| Metodo | Ruta | Descripcion |
|---|---|---|
| GET | `/api/v1/palettes` | Listar todas las paletas (sistema + usuario) |
| POST, PATCH | `/api/v1/palettes/apply` | Aplicar paleta al efecto activo |
| POST, PATCH | `/api/v1/palettes/save` | Guardar o editar paleta de usuario |
| POST, PATCH | `/api/v1/palettes/delete` | Eliminar paleta de usuario |

## Ejemplo de paleta de usuario

```json
{
  "id": -101,
  "label": "Mi paleta",
  "primaryColors": ["#123456", "#654321", "#ABCDEF"],
  "style": "neon",
  "description": "Paleta personalizada"
}
```

Los ids de paleta de usuario son negativos; los de sistema son positivos.
