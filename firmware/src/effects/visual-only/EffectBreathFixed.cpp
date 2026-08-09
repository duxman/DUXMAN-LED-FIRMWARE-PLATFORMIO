/*
 * duxman-led next - v0.3.7-beta
 * Licensed under the Apache License 2.0
 * File: firmware/src/effects/EffectBreathFixed.cpp
 * Last commit: 2c35a63 - 2026-04-28
 */

#include "effects/visual-only/EffectBreathFixed.h"

#include "effects/EffectCanvas1D.h"
#include "effects/EffectRegistry.h"
#include <math.h>

bool EffectBreathFixed::supports(uint8_t effectId) const {
  return effectId == EffectRegistry::kEffectBreathFixed;
}

void EffectBreathFixed::renderFrame() {
  CoreState &s = state();
  LedDriver &led = driver();

  const float t = normalizedTimeSec();
  const float period = 8.0f - 7.5f * speed01(s.effectSpeed);
  const float breathe = 0.5f + 0.5f * sinf(2.0f * PI * t / period);
  const float depth = level01(s.effectLevel);
  const float envelope = (1.0f - depth) + depth * breathe;
  const float finalGain = clamp01(envelope * (s.brightness / 255.0f));

  for (uint8_t outIdx = 0; outIdx < led.outputCount(); ++outIdx) {
    const LedDriverOutputConfig &out = led.outputConfig(outIdx);
    if (!out.enabled) {
      continue;
    }

    if (!led.supportsPerPixelColor(outIdx) || out.ledCount <= 1) {
      led.setOutputColor(outIdx, scaleColorFloat(s.primaryColors[outIdx % 3], finalGain));
      continue;
    }

    EffectCanvas1D canvas;
    canvas.allocate(out.ledCount);
    canvas.clear(0);

    const uint16_t sectionSize = resolveSectionSize(out.ledCount, s.sectionCount);
    for (uint16_t px = 0; px < out.ledCount; ++px) {
      const uint8_t sectionIdx = static_cast<uint8_t>((px / sectionSize) % 3);
      canvas.setPixel(px, scaleColorFloat(s.primaryColors[sectionIdx], finalGain));
    }
    canvas.flushToDriver(led, outIdx);
  }
  led.show();
}
