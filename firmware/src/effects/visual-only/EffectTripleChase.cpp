/*
 * duxman-led next - v0.3.7-beta
 * Licensed under the Apache License 2.0
 * File: firmware/src/effects/EffectTripleChase.cpp
 * Last commit: 2c35a63 - 2026-04-28
 */

#include "effects/visual-only/EffectTripleChase.h"

#include "effects/EffectRegistry.h"

#include <math.h>

bool EffectTripleChase::supports(uint8_t effectId) const {
  return effectId == EffectRegistry::kEffectTripleChase;
}

void EffectTripleChase::renderFrame() {
  CoreState &s = state();
  LedDriver &led = driver();

  const float t = normalizedTimeSec();
  const float speedNorm = speed01(s.effectSpeed);
  const float levelNorm = level01(s.effectLevel);
  const float levelCurve = powf(levelNorm, 1.25f);
  const float repeats = static_cast<float>(max<uint8_t>(1, s.sectionCount));
  const float speedHz = 0.08f + speedNorm * 3.2f;
  const float phase = t * speedHz;

  // level bajo: tren fino. level alto: tren ancho y mas brillante.
  const float trainWidth = 0.05f + levelCurve * 0.30f;
  const float globalGain = s.brightness / 255.0f;

  for (uint8_t outIdx = 0; outIdx < led.outputCount(); ++outIdx) {
    const LedDriverOutputConfig &out = led.outputConfig(outIdx);
    if (!out.enabled) {
      continue;
    }

    if (!led.supportsPerPixelColor(outIdx) || out.ledCount <= 1) {
      const uint8_t colorIdx = static_cast<uint8_t>(fmodf(phase * 3.0f, 3.0f));
      led.setOutputColor(outIdx, scaleColorFloat(s.primaryColors[colorIdx], globalGain));
      continue;
    }

    const uint32_t background = scaleColorFloat(s.backgroundColor, globalGain * (0.06f + 0.20f * (1.0f - levelNorm)));

    for (uint16_t px = 0; px < out.ledCount; ++px) {
      const float x = normalizedX(px, out.ledCount);
      const float waveBase = x * repeats - phase;
      uint32_t composed = background;

      for (uint8_t lane = 0; lane < 3; ++lane) {
        const float laneOffset = static_cast<float>(lane) / 3.0f;
        const float wave = waveBase - laneOffset;
        const float localPos = wave - floorf(wave);

        if (localPos < trainWidth) {
          // Head mas intensa, cola mas suave.
          const float ramp = 1.0f - (localPos / trainWidth);
          const float intensity = smoothstep(0.0f, 1.0f, ramp);
          const uint32_t chaseColor = scaleColorFloat(s.primaryColors[lane], intensity * globalGain * (0.55f + 0.45f * levelNorm));
          const uint8_t alpha = static_cast<uint8_t>(min(255.0f, intensity * (145.0f + 110.0f * levelNorm)));
          composed = blendAlpha(composed, chaseColor, alpha);
        }
      }

      setPixel(outIdx, px, composed);
    }
  }

  led.show();
}
