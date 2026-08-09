/*
 * duxman-led next
 * Licensed under the Apache License 2.0
 * File: firmware/src/effects/EffectCanvas1D.h
 */

#pragma once

#include <Arduino.h>

#include "drivers/LedDriver.h"

class EffectCanvas1D {
public:
  EffectCanvas1D() = default;
  ~EffectCanvas1D();

  void allocate(uint16_t pixelCount);
  void clear(uint32_t color = 0);
  void setPixel(uint16_t pixelIndex, uint32_t color);
  uint32_t getPixel(uint16_t pixelIndex) const;
  void addPixel(uint16_t pixelIndex, uint32_t color);
  void fadeToBlackBy(uint8_t fadeAmount);
  void blur(uint8_t blurAmount);
  void flushToDriver(LedDriver &driver, uint8_t outputIndex) const;
  uint16_t pixelCount() const { return pixelCount_; }

private:
  uint16_t pixelCount_ = 0;
  uint32_t *pixels_ = nullptr;
};
