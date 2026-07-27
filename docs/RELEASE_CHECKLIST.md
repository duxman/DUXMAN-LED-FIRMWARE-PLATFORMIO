# Release Checklist (2 minutos)

Esta checklist minimiza errores al publicar nuevas versiones y garantiza trazabilidad.

## 1) Preparar version

- [ ] Actualizar version visible en firmware (`DUX_FW_VERSION`) en `platformio.ini`.
- [ ] Actualizar fecha (`DUX_FW_DATE`) en `platformio.ini`.
- [ ] Revisar version mostrada en `README.md`.

## 2) Actualizar changelog

- [ ] Mover cambios relevantes desde `Unreleased` a una nueva seccion versionada.
- [ ] Crear cabecera con formato exacto:

```markdown
## [0.6.5-beta] - 2026-07-27
```

- [ ] Incluir al menos: `Added`, `Changed`, `Fixed` (si aplica).
- [ ] Confirmar que la seccion de version coincide con el tag a publicar (sin prefijo `v`).

## 3) Commit y push

- [ ] `git add CHANGELOG.md platformio.ini README.md`
- [ ] `git commit -m "release: prepare v0.6.5-beta"`
- [ ] `git push origin main`

## 4) Crear tag de release

- [ ] `git tag -a v0.6.5-beta -m "v0.6.5-beta"`
- [ ] `git push origin v0.6.5-beta`

## 5) Verificar GitHub Actions

- [ ] Workflow esperado: `.github/workflows/firmware-build-release.yml`
- [ ] Jobs `build` en verde para:
  - `esp32c3supermini`
  - `esp32dev`
  - `esp32s3`
- [ ] Job `release` en verde.

## 6) Verificar assets de release

- [ ] Existen 3 zips (uno por entorno) en la release.
- [ ] Existe `SHA256SUMS.txt`.
- [ ] El cuerpo de la release coincide con la seccion de `CHANGELOG.md`.

## 7) Post-release rapido

- [ ] Crear nueva seccion `Unreleased` para ciclo siguiente (si se vacio).
- [ ] Comunicar version publicada al equipo.
