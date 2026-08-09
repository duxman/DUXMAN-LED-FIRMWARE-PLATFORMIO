/*
 * duxman-led next
 * Licensed under the Apache License 2.0
 * File: firmware/src/effects/EffectCanvas1D.cpp
 */

#include "effects/EffectCanvas1D.h"

#include <Arduino.h>

namespace {
uint8_t blendChannel(uint8_t a, uint8_t b, uint16_t weightA, uint16_t weightB) {
  if (weightA + weightB == 0) {
    return a;
  }
  return static_cast<uint8_t>((static_cast<uint16_t>(a) * weightA + static_cast<uint16_t>(b) * weightB) / (weightA + weightB));
}

uint32_t blendColor(uint32_t a, uint32_t b, uint16_t weightA, uint16_t weightB) {
  const uint8_t ar = static_cast<uint8_t>((a >> 16) & 0xFF);
  const uint8_t ag = static_cast<uint8_t>((a >> 8) & 0xFF);
  const uint8_t ab = static_cast<uint8_t>(a & 0xFF);
  const uint8_t br = static_cast<uint8_t>((b >> 16) & 0xFF);
  const uint8_t bg = static_cast<uint8_t>((b >> 8) & 0xFF);
  const uint8_t bb = static_cast<uint8_t>(b & 0xFF);
  const uint8_t r = blendChannel(ar, br, weightA, weightB);
  const uint8_t g = blendChannel(ag, bg, weightA, weightB);
  const uint8_t bl = blendChannel(ab, bb, weightA, weightB);
  return (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | bl;
}
} // namespace

EffectCanvas1D::~EffectCanvas1D() {
  delete[] pixels_;
  pixels_ = nullptr;
  pixelCount_ = 0;
}

void EffectCanvas1D::allocate(uint16_t pixelCount) {
  if (pixelCount == pixelCount_ && pixels_ != nullptr) {
    return;
  }

  delete[] pixels_;
  pixels_ = nullptr;
  pixelCount_ = 0;

  if (pixelCount == 0) {
    return;
  }

  pixels_ = new uint32_t[pixelCount];
  if (pixels_ == nullptr) {
    return;
  }
  pixelCount_ = pixelCount;
  clear(0);
}

void EffectCanvas1D::clear(uint32_t color) {
  if (pixels_ == nullptr || pixelCount_ == 0) {
    return;
  }
  for (uint16_t i = 0; i < pixelCount_; ++i) {
    pixels_[i] = color;
  }
}

void EffectCanvas1D::setPixel(uint16_t pixelIndex, uint32_t color) {
  if (pixels_ == nullptr || pixelIndex >= pixelCount_) {
    return;
  }
  pixels_[pixelIndex] = color;
}

uint32_t EffectCanvas1D::getPixel(uint16_t pixelIndex) const {
  if (pixels_ == nullptr || pixelIndex >= pixelCount_) {
    return 0;
  }
  return pixels_[pixelIndex];
}

void EffectCanvas1D::addPixel(uint16_t pixelIndex, uint32_t color) {
  if (pixels_ == nullptr || pixelIndex >= pixelCount_) {
    return;
  }

  const uint32_t base = pixels_[pixelIndex];
  const uint8_t br = min<uint8_t>(255, static_cast<uint8_t>(((base >> 16) & 0xFF) + ((color >> 16) & 0xFF)));
  const uint8_t bg = min<uint8_t>(255, static_cast<uint8_t>(((base >> 8) & 0xFF) + ((color >> 8) & 0xFF)));
  const uint8_t bb = min<uint8_t>(255, static_cast<uint8_t>((base & 0xFF) + (color & 0xFF)));
  pixels_[pixelIndex] = (static_cast<uint32_t>(br) << 16) | (static_cast<uint32_t>(bg) << 8) | bb;
}

void EffectCanvas1D::fadeToBlackBy(uint8_t fadeAmount) {
  if (pixels_ == nullptr || pixelCount_ == 0) {
    return;
  }

  const uint16_t keep = static_cast<uint16_t>(255 - fadeAmount);
  for (uint16_t i = 0; i < pixelCount_; ++i) {
    const uint32_t color = pixels_[i];
    const uint8_t r = static_cast<uint8_t>(((color >> 16) & 0xFF) * keep / 255u);
    const uint8_t g = static_cast<uint8_t>(((color >> 8) & 0xFF) * keep / 255u);
    const uint8_t bl = static_cast<uint8_t>((color & 0xFF) * keep / 255u);
    pixels_[i] = (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | bl;
  }
}

void EffectCanvas1D::blur(uint8_t blurAmount) {
  if (pixels_ == nullptr || pixelCount_ < 3 || blurAmount == 0) {
    return;
  }

  uint32_t *snapshot = new uint32_t[pixelCount_];
  if (snapshot == nullptr) {
    return;
  }

  for (uint16_t i = 0; i < pixelCount_; ++i) {
    snapshot[i] = pixels_[i];
  }

  const uint16_t centerWeight = static_cast<uint16_t>(255 - blurAmount);
  const uint16_t sideWeight = static_cast<uint16_t>(blurAmount / 2u);
  for (uint16_t i = 0; i < pixelCount_; ++i) {
    const uint16_t prev = i == 0 ? 0 : i - 1;
    const uint16_t next = i + 1 < pixelCount_ ? i + 1 : pixelCount_ - 1;
    const uint32_t a = snapshot[prev];
    const uint32_t c = snapshot[i];
    const uint32_t b = snapshot[next];

    pixels_[i] = blendColor(blendColor(a, c, sideWeight, centerWeight), b, centerWeight, sideWeight);
  }

  delete[] snapshot;
}

void EffectCanvas1D::flushToDriver(LedDriver &driver, uint8_t outputIndex) const {
  if (pixels_ == nullptr || pixelCount_ == 0) {
    return;
  }

  if (!driver.supportsPerPixelColor(outputIndex)) {
    const uint32_t sample = pixels_[pixelCount_ / 2];
    driver.setOutputColor(outputIndex, sample);
    return;
  }

  for (uint16_t i = 0; i < pixelCount_; ++i) {
    driver.setPixelColor(outputIndex, i, pixels_[i]);
  }
}
