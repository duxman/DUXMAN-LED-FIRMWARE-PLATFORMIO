# Changelog

All notable changes to this project are documented in this file.

The format is based on Keep a Changelog.

## [Unreleased]

### Added
- Changelog governance with required `CHANGELOG.md` updates in pull requests.
- Release pipeline publishes notes from versioned changelog sections.
- Fast operational release checklist in `docs/RELEASE_CHECKLIST.md`.

### Docs
- New sync compatibility guide for LedFx/WLED/E1.31/MQTT at `docs/wiki/Sync-Compatibility.md`.

## [0.6.5-beta] - 2026-07-28

### Changed
- Firmware version markers bumped to `0.6.5-beta` (`platformio.ini`, `release-info.json`).
- Shared UI CSS injection changed from large inline replacement to external stylesheet link (`/ui/common.css`) to reduce heap pressure on large templates.

### Fixed
- Home page no longer renders literal `__CSS__` marker when `home.html` is loaded from LittleFS.

### Docs
- Updated root README and wiki Home version references to `0.6.5-beta`.
- Added release-history timeline entry for `0.6.5-beta` and documented the Home CSS delivery fix.

## [0.6.4-beta] - 2026-07-27

### Added
- Beta validation release for automated artifact generation and publication pipeline.

### Changed
- Firmware release marker updated from alpha to beta for first release workflow validation.

### Notes
- Includes all changes listed under `0.6.4-alpha`.

## [0.6.4-alpha] - 2026-07-27

### Added
- Automated firmware CI/CD workflow for `esp32c3supermini`, `esp32dev`, `esp32s3`.
- Automatic release artifacts (`.zip`) and `SHA256SUMS.txt` on tag/release events.
- Local wiki mirror tooling with sync scripts and VS Code tasks.
- Wiki sync drift CI check.
- Pull request template with wiki sync checklist.
- Canonical configuration schema document at `docs/wiki/Configuration-Schema.md`.

### Changed
- Unified canonical configuration contract to `network`, `gpio`, `microphone`, `general`, `sync`.
- API and storage flows aligned to the canonical general configuration schema.
- Root README updated with CI/CD and canonical configuration documentation.

### Docs
- Updated wiki pages: API-v1, Architecture, GPIO-Profiles, Home.

## [0.6.3-alpha] - 2026-07-23

### Notes
- Baseline firmware version in `platformio.ini` before changelog enforcement.

## Historical Releases (Imported)

Legacy release history from monorepo era was consolidated from:

- `docs/old/CHANGELOG.md`
- `docs/old/RELEASE_NOTES.md`
- `docs/old/GITHUB_RELEASE_0.6.3-alpha.es.md`

Quick index (see full summary in `docs/wiki/Release-History.md`):

- `0.6.3-alpha` (2026-05-02): Sync stack S1-S6 completed (DDP, E1.31/sACN, cluster sync, shared clock).
- `0.6.2-alpha` (2026-05-03): Initial sync sprint with `SyncConfig`, `SyncService`, and `/api/v1/sync/*` endpoints.
- `0.6.1-alpha` (2026-05-02): UI navigation hardening and global version alignment.
- `0.6.0-alpha` (2026-04-30): Full i18n and move from debug config semantics to general config.
- `0.5.0-alpha` (2026-04-30): `VoltageOptimizer` and advanced power/thermal config.
- `0.4.2-beta` to `0.3.2-beta` (2026-04): UI migration to LittleFS templates, new audio-reactive effects, palette services, and API/architecture stabilization.
