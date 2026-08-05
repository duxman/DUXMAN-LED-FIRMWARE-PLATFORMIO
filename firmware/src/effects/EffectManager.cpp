/*
 * duxman-led next - v0.3.7-beta
 * Licensed under the Apache License 2.0
 * File: firmware/src/effects/EffectManager.cpp
 * Last commit: 2c35a63 - 2026-04-28
 */

#include "effects/EffectManager.h"

#include "effects/audio-reactive/EffectAudioPulse.h"
#include "effects/audio-reactive/EffectAudioNeonEq.h"
#include "effects/audio-reactive/EffectAudioSpectrum.h"
#include "effects/audio-reactive/EffectAudioRainbowWave.h"
#include "effects/audio-reactive/EffectAudioSpectrumChase.h"
#include "effects/audio-reactive/EffectAudioSectionStrobe.h"
#include "effects/audio-reactive/EffectBouncingPhysics.h"
#include "effects/audio-reactive/EffectStellarTwinkle.h"
#include "effects/audio-reactive/EffectTrigRibbon.h"
#include "effects/visual-only/EffectBlinkFixed.h"
#include "effects/visual-only/EffectBlinkGradient.h"
#include "effects/visual-only/EffectBreathFixed.h"
#include "effects/visual-only/EffectBreathGradient.h"
#include "effects/visual-only/EffectDiagnostic.h"
#include "effects/visual-only/EffectFixed.h"
#include "effects/visual-only/EffectGradient.h"
#include "effects/visual-only/EffectGradientMeteor.h"
#include "effects/visual-only/EffectLavaFlow.h"
#include "effects/visual-only/EffectPolarIce.h"
#include "effects/visual-only/EffectRandomColorPop.h"
#include "effects/EffectRegistry.h"
#include "effects/visual-only/EffectScanningPulse.h"
#include "effects/visual-only/EffectTripleChase.h"

namespace {
uint32_t blendColorLerp(uint32_t from, uint32_t to, uint16_t t256) {
  const uint16_t inv = static_cast<uint16_t>(256 - t256);
  const uint8_t fromR = static_cast<uint8_t>((from >> 16) & 0xFF);
  const uint8_t fromG = static_cast<uint8_t>((from >> 8) & 0xFF);
  const uint8_t fromB = static_cast<uint8_t>(from & 0xFF);
  const uint8_t toR = static_cast<uint8_t>((to >> 16) & 0xFF);
  const uint8_t toG = static_cast<uint8_t>((to >> 8) & 0xFF);
  const uint8_t toB = static_cast<uint8_t>(to & 0xFF);

  const uint8_t outR = static_cast<uint8_t>((fromR * inv + toR * t256) >> 8);
  const uint8_t outG = static_cast<uint8_t>((fromG * inv + toG * t256) >> 8);
  const uint8_t outB = static_cast<uint8_t>((fromB * inv + toB * t256) >> 8);
  return (static_cast<uint32_t>(outR) << 16) | (static_cast<uint32_t>(outG) << 8) | outB;
}
} // namespace

struct EffectManager::Impl {
  static constexpr size_t kEffectCount = 22;
  static constexpr uint16_t kMaxTransitionMs = 1500;

  CoreState &state;
  LedDriver &driver;
  EffectEngine *effects[kEffectCount] = {};
  uint8_t lastEffectId = 255; // 255 = ninguno activo aun
  bool transitionActive = false;
  uint32_t transitionStartedAtMs = 0;
  uint16_t transitionDurationMs = 0;
  uint8_t transitionStyle = 0;
  uint32_t *transitionPrevFrame = nullptr;
  size_t transitionPrevFrameSize = 0;

  EffectFixed fixedEffect;
  EffectGradient gradientEffect;
  EffectBlinkFixed blinkFixedEffect;
  EffectBlinkGradient blinkGradientEffect;
  EffectDiagnostic diagnosticEffect;
  EffectBreathFixed breathFixedEffect;
  EffectBreathGradient breathGradientEffect;
  EffectTripleChase tripleChaseEffect;
  EffectGradientMeteor gradientMeteorEffect;
  EffectScanningPulse scanningPulseEffect;
  EffectTrigRibbon trigRibbonEffect;
  EffectLavaFlow lavaFlowEffect;
  EffectPolarIce polarIceEffect;
  EffectStellarTwinkle stellarTwinkleEffect;
  EffectRandomColorPop randomColorPopEffect;
  EffectBouncingPhysics bouncingPhysicsEffect;
  EffectAudioPulse audioPulseEffect;
  EffectAudioSpectrum audioSpectrumEffect;
  EffectAudioNeonEq audioNeonEqEffect;
  EffectAudioRainbowWave audioRainbowWaveEffect;
  EffectAudioSpectrumChase audioSpectrumChaseEffect;
  EffectAudioSectionStrobe audioSectionStrobeEffect;

  Impl(CoreState &stateRef, LedDriver &driverRef)
      : state(stateRef),
        driver(driverRef),
        fixedEffect(stateRef, driverRef),
        gradientEffect(stateRef, driverRef),
        blinkFixedEffect(stateRef, driverRef),
        blinkGradientEffect(stateRef, driverRef),
        diagnosticEffect(stateRef, driverRef),
        breathFixedEffect(stateRef, driverRef),
        breathGradientEffect(stateRef, driverRef),
        tripleChaseEffect(stateRef, driverRef),
        gradientMeteorEffect(stateRef, driverRef),
        scanningPulseEffect(stateRef, driverRef),
        trigRibbonEffect(stateRef, driverRef),
        lavaFlowEffect(stateRef, driverRef),
        polarIceEffect(stateRef, driverRef),
        stellarTwinkleEffect(stateRef, driverRef),
        randomColorPopEffect(stateRef, driverRef),
        bouncingPhysicsEffect(stateRef, driverRef),
        audioPulseEffect(stateRef, driverRef),
        audioSpectrumEffect(stateRef, driverRef),
        audioNeonEqEffect(stateRef, driverRef),
        audioRainbowWaveEffect(stateRef, driverRef),
        audioSpectrumChaseEffect(stateRef, driverRef),
        audioSectionStrobeEffect(stateRef, driverRef) {
    effects[0] = &fixedEffect;
    effects[1] = &gradientEffect;
    effects[2] = &blinkFixedEffect;
    effects[3] = &blinkGradientEffect;
    effects[4] = &diagnosticEffect;
    effects[5] = &breathFixedEffect;
    effects[6] = &breathGradientEffect;
    effects[7] = &tripleChaseEffect;
    effects[8] = &gradientMeteorEffect;
    effects[9] = &scanningPulseEffect;
    effects[10] = &trigRibbonEffect;
    effects[11] = &lavaFlowEffect;
    effects[12] = &polarIceEffect;
    effects[13] = &stellarTwinkleEffect;
    effects[14] = &randomColorPopEffect;
    effects[15] = &bouncingPhysicsEffect;
    effects[16] = &audioPulseEffect;
    effects[17] = &audioSpectrumEffect;
    effects[18] = &audioNeonEqEffect;
    effects[19] = &audioRainbowWaveEffect;
    effects[20] = &audioSpectrumChaseEffect;
    effects[21] = &audioSectionStrobeEffect;
  }

  ~Impl() {
    delete[] transitionPrevFrame;
    transitionPrevFrame = nullptr;
    transitionPrevFrameSize = 0;
  }

  size_t logicalPixelCount() const {
    size_t total = 0;
    for (uint8_t outputIndex = 0; outputIndex < driver.outputCount(); ++outputIndex) {
      total += driver.outputLogicalPixelCount(outputIndex);
    }
    return total;
  }

  bool ensureTransitionBuffer() {
    const size_t required = logicalPixelCount();
    if (required == 0) {
      transitionPrevFrameSize = 0;
      return false;
    }

    if (transitionPrevFrame != nullptr && transitionPrevFrameSize == required) {
      return true;
    }

    delete[] transitionPrevFrame;
    transitionPrevFrame = new uint32_t[required];
    if (transitionPrevFrame == nullptr) {
      transitionPrevFrameSize = 0;
      return false;
    }

    transitionPrevFrameSize = required;
    return true;
  }

  bool captureCurrentFrame() {
    if (!ensureTransitionBuffer()) {
      return false;
    }

    size_t index = 0;
    for (uint8_t outputIndex = 0; outputIndex < driver.outputCount(); ++outputIndex) {
      const uint16_t logicalCount = driver.outputLogicalPixelCount(outputIndex);
      for (uint16_t pixelIndex = 0; pixelIndex < logicalCount; ++pixelIndex) {
        transitionPrevFrame[index++] = driver.outputPixelColor(outputIndex, pixelIndex);
      }
    }
    return index == transitionPrevFrameSize;
  }

  void startTransitionIfNeeded() {
    transitionActive = false;
    transitionDurationMs = static_cast<uint16_t>(constrain(state.effectTransitionMs, 0, kMaxTransitionMs));
    transitionStyle = static_cast<uint8_t>(constrain(state.effectTransitionStyle, static_cast<uint8_t>(0), static_cast<uint8_t>(1)));

    if (transitionDurationMs == 0 || lastEffectId == 255) {
      return;
    }

    if (!captureCurrentFrame()) {
      transitionDurationMs = 0;
      return;
    }

    transitionActive = true;
    transitionStartedAtMs = millis();
  }

  void applyTransitionFrame() {
    if (!transitionActive || transitionDurationMs == 0 || transitionPrevFrame == nullptr) {
      transitionActive = false;
      return;
    }

    const uint32_t nowMs = millis();
    const uint32_t elapsedMs = nowMs - transitionStartedAtMs;
    if (elapsedMs >= transitionDurationMs) {
      transitionActive = false;
      return;
    }

    const uint16_t t256 = static_cast<uint16_t>((elapsedMs * 256UL) / transitionDurationMs);
    const size_t revealPixels = (logicalPixelCount() * elapsedMs) / transitionDurationMs;

    size_t index = 0;
    for (uint8_t outputIndex = 0; outputIndex < driver.outputCount(); ++outputIndex) {
      const uint16_t logicalCount = driver.outputLogicalPixelCount(outputIndex);
      if (logicalCount == 0) {
        continue;
      }

      if (driver.supportsPerPixelColor(outputIndex)) {
        for (uint16_t pixelIndex = 0; pixelIndex < logicalCount; ++pixelIndex) {
          const uint32_t from = transitionPrevFrame[index++];
          const uint32_t to = driver.outputPixelColor(outputIndex, pixelIndex);
          uint32_t out = to;
          if (transitionStyle == 1) {
            out = (index <= revealPixels) ? to : from;
          } else {
            out = blendColorLerp(from, to, t256);
          }
          driver.setPixelColor(outputIndex, pixelIndex, out);
        }
      } else {
        const uint32_t from = transitionPrevFrame[index++];
        const uint32_t to = driver.outputPixelColor(outputIndex, 0);
        uint32_t out = to;
        if (transitionStyle == 1) {
          out = (index <= revealPixels) ? to : from;
        } else {
          out = blendColorLerp(from, to, t256);
        }
        driver.setOutputColor(outputIndex, out);
      }
    }

    driver.show();
  }
};

EffectManager::EffectManager(CoreState &state, LedDriver &driver)
    : impl_(new Impl(state, driver)) {}

EffectManager::~EffectManager() {
  delete impl_;
}

void EffectManager::begin() {
  impl_->fixedEffect.begin();
}

void EffectManager::renderFrame() {
  if (!impl_->state.lock()) {
    return;
  }

  if (!impl_->driver.isInitialized()) {
    impl_->driver.begin();
  }

  if (!impl_->state.power) {
    impl_->transitionActive = false;
    impl_->driver.clear();
    impl_->driver.show();
    impl_->state.unlock();
    return;
  }

  EffectEngine &activeEffect = resolveActiveEffect();

  // Ciclo de vida: detectar cambio de efecto y notificar onActivate/onDeactivate.
  if (impl_->state.effectId != impl_->lastEffectId) {
    impl_->startTransitionIfNeeded();

    if (impl_->lastEffectId != 255) {
      for (size_t i = 0; i < Impl::kEffectCount; ++i) {
        if (impl_->effects[i] && impl_->effects[i]->supports(impl_->lastEffectId)) {
          impl_->effects[i]->onDeactivate();
          break;
        }
      }
    }
    // Limpiar el buffer antes de activar el nuevo efecto para que no queden
    // pixeles residuales del efecto anterior.  NO llamar show() aqui: hacer
    // un Show() DMA y luego escribir en el mismo buffer dentro de
    // activeEffect.renderFrame() (mas abajo, en el mismo ciclo) produce una
    // carrera de datos que corrompe la trama y enciende pixeles aleatorios.
    // El buffer limpio se enviara al strip en el show() del primer frame del
    // nuevo efecto.
    impl_->driver.clear();

    // Activar log de diagnostico para el primer frame del nuevo efecto.
    impl_->driver.scheduleShowLog();

    activeEffect.onActivate();
    impl_->lastEffectId = impl_->state.effectId;
  }

  activeEffect.renderFrame();
  impl_->applyTransitionFrame();
  impl_->state.unlock();
}

EffectEngine &EffectManager::resolveActiveEffect() {
  for (size_t i = 0; i < Impl::kEffectCount; ++i) {
    if (impl_->effects[i] != nullptr && impl_->effects[i]->supports(impl_->state.effectId)) {
      return *impl_->effects[i];
    }
  }

  return impl_->fixedEffect;
}