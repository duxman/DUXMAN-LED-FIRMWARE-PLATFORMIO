/*
 * duxman-led next - v0.3.7-beta
 * Licensed under the Apache License 2.0
 * File: firmware/src/effects/EffectDiagnostic.cpp
 * Last commit: 2c35a63 - 2026-04-28
 */

#include "effects/visual-only/EffectDiagnostic.h"

#include "effects/EffectCanvas1D.h"

bool EffectDiagnostic::supports(uint8_t effectId) const {
  return effectId == EffectRegistry::kEffectDiagnostic;
}

void EffectDiagnostic::renderFrame() {
  CoreState &currentState = state();
  LedDriver &ledDriver = driver();
  const uint32_t activeColor = scaleColor(0xFF0000UL, currentState.brightness);

  for (uint8_t outputIndex = 0; outputIndex < ledDriver.outputCount(); ++outputIndex) {
    const LedDriverOutputConfig &output = ledDriver.outputConfig(outputIndex);
    if (!output.enabled) {
      continue;
    }

    if (outputIndex == 0) {
      if (!ledDriver.supportsPerPixelColor(outputIndex) || output.ledCount <= 1) {
        ledDriver.setOutputColor(outputIndex, activeColor);
        continue;
      }

      EffectCanvas1D canvas;
      canvas.allocate(output.ledCount);
      canvas.clear(activeColor);
      canvas.flushToDriver(ledDriver, outputIndex);
      continue;
    }

    if (!ledDriver.supportsPerPixelColor(outputIndex) || output.ledCount <= 1) {
      ledDriver.setOutputColor(outputIndex, 0);
      continue;
    }

    EffectCanvas1D canvas;
    canvas.allocate(output.ledCount);
    canvas.clear(0);
    canvas.flushToDriver(ledDriver, outputIndex);
  }
  ledDriver.show();
}