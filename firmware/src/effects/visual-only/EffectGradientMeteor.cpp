/*
 * duxman-led next - v0.3.7-beta
 * Licensed under the Apache License 2.0
 * File: firmware/src/effects/EffectGradientMeteor.cpp
 * Last commit: 2c35a63 - 2026-04-28
 */

#include "effects/visual-only/EffectGradientMeteor.h"

#include "effects/EffectCanvas1D.h"
#include "effects/EffectRegistry.h"

#include <math.h>

bool EffectGradientMeteor::supports(uint8_t effectId) const {
  return effectId == EffectRegistry::kEffectGradientMeteor;
}

void EffectGradientMeteor::renderFrame() {
  CoreState &s = state();
  LedDriver &led = driver();

  const float t = normalizedTimeSec();
  const float speedNorm = speed01(s.effectSpeed);
  const float levelNorm = level01(s.effectLevel);
  const float levelCurve = powf(levelNorm, 1.2f);
  const float speedHz = 0.06f + speedNorm * 3.0f;
  const float globalGain = s.brightness / 255.0f;

  for (uint8_t outIdx = 0; outIdx < led.outputCount(); ++outIdx) {
    const LedDriverOutputConfig &out = led.outputConfig(outIdx);
    if (!out.enabled) {
      continue;
    }

    if (!led.supportsPerPixelColor(outIdx) || out.ledCount <= 1) {
      const uint32_t color = gradientColor(s.primaryColors[0], s.primaryColors[1], s.primaryColors[2],
                                           static_cast<uint16_t>(fmodf(t * speedHz * 100.0f, 100.0f)), 100);
      led.setOutputColor(outIdx, scaleColorFloat(color, globalGain));
      continue;
    }

    EffectCanvas1D canvas;
    canvas.allocate(out.ledCount);
    canvas.clear(scaleColorFloat(s.backgroundColor, globalGain * (0.08f + 0.22f * (1.0f - levelNorm))));

    const float ledCountF = static_cast<float>(out.ledCount);
    const float headPos = fmodf(t * speedHz * ledCountF, ledCountF);

    // level bajo: cola corta; level alto: cola larga y mas presencia.
    const float tailLen = max(1.0f, ledCountF * (0.08f + levelCurve * 0.57f));

    for (uint16_t px = 0; px < out.ledCount; ++px) {
      float dist = headPos - static_cast<float>(px);
      if (dist < 0.0f) {
        dist += ledCountF;
      }

      float intensity = 0.0f;
      if (dist <= tailLen) {
        const float norm = 1.0f - (dist / tailLen);
        intensity = smoothstep(0.0f, 1.0f, norm);
      }

      if (intensity > 0.0f) {
        const uint32_t meteorColor = gradientColor(s.primaryColors[0], s.primaryColors[1], s.primaryColors[2], px, out.ledCount);
        canvas.addPixel(px, scaleColorFloat(meteorColor, intensity * globalGain * (0.60f + 0.40f * levelNorm)));
      }
    }

    // Trail cleanup and slight diffusion for smoother meteor tail.
    const float profileBlend = 1.0f - levelNorm;
    const uint8_t fadeAmount = static_cast<uint8_t>(24.0f - 6.0f * levelNorm);
    const uint8_t blurAmount = static_cast<uint8_t>(12.0f + 20.0f * levelNorm);
    canvas.fadeToBlackBy(fadeAmount);
    if (blurAmount > 0) {
      canvas.blur(blurAmount);
      if (profileBlend < 0.45f) {
        canvas.blur(static_cast<uint8_t>(6.0f + 8.0f * levelNorm));
      }
    }
    canvas.flushToDriver(led, outIdx);
  }

  led.show();
}
