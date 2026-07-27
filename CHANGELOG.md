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
