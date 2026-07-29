/*
 * duxman-led next
 * Licensed under the Apache License 2.0
 * File: firmware/src/services/RenderMetricsService.cpp
 */

#include "services/RenderMetricsService.h"

#include <ArduinoJson.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Instancia global.
RenderMetricsService gRenderMetrics;

// ---------------------------------------------------------------------------

void RenderMetricsService::frameBegin() {
  frameBeginUs_ = esp_timer_get_time();
}

void RenderMetricsService::frameEnd() {
  const int64_t nowUs  = esp_timer_get_time();
  const uint32_t nowMs = static_cast<uint32_t>(millis());

  // ── Frame timing ──────────────────────────────────────────────────────────
  const uint32_t durationUs = (nowUs > frameBeginUs_)
                              ? static_cast<uint32_t>(nowUs - frameBeginUs_)
                              : 0;

  frameCount_++;
  frameTimeAccUs_ += durationUs;

  if (durationUs < frameTimeMinUs_) { frameTimeMinUs_ = durationUs; }
  if (durationUs > frameTimeMaxUs_) { frameTimeMaxUs_ = durationUs; }
  if (frameCount_ > 0) {
    frameTimeAvgUs_ = static_cast<uint32_t>(frameTimeAccUs_ / frameCount_);
  }

  // ── FPS (ventana deslizante de 1 segundo) ─────────────────────────────────
  fpsCounter_++;
  if (fpsWindowMs_ == 0) {
    fpsWindowMs_ = nowMs;
  }
  const uint32_t elapsed = nowMs - fpsWindowMs_;
  if (elapsed >= 1000) {
    fpsCurrent_  = static_cast<uint32_t>((fpsCounter_ * 1000UL) / elapsed);
    fpsCounter_  = 0;
    fpsWindowMs_ = nowMs;
  }

  // ── Heap (muestreo periodico) ─────────────────────────────────────────────
  if (++heapSampleCounter_ >= kHeapSamplePeriod) {
    heapSampleCounter_ = 0;
    const uint32_t freeNow = static_cast<uint32_t>(ESP.getFreeHeap());
    heapFreeBytes_ = freeNow;
    if (freeNow < heapMinFreeBytes_) {
      heapMinFreeBytes_ = freeNow;
    }
  }

  // ── Stack headroom de la tarea render ─────────────────────────────────────
  // Solo en frames de heap para no llamar a FreeRTOS en cada frame.
  if (heapSampleCounter_ == 1) {
    const UBaseType_t watermark = uxTaskGetStackHighWaterMark(nullptr);
    renderStackWords_ = static_cast<uint32_t>(watermark);
  }
}

void RenderMetricsService::reset() {
  frameTimeMinUs_ = UINT32_MAX;
  frameTimeMaxUs_ = 0;
  frameTimeAccUs_ = 0;
  frameTimeAvgUs_ = 0;
  frameCount_     = 0;
  fpsCounter_     = 0;
  fpsWindowMs_    = 0;
  fpsCurrent_     = 0;
  heapMinFreeBytes_ = heapFreeBytes_;
}

String RenderMetricsService::toJson() const {
  JsonDocument doc;

  doc["fps"]              = fpsCurrent_;
  doc["frameTimeAvgUs"]   = frameTimeAvgUs_;
  doc["frameTimeMinUs"]   = (frameTimeMinUs_ == UINT32_MAX) ? 0 : frameTimeMinUs_;
  doc["frameTimeMaxUs"]   = frameTimeMaxUs_;
  doc["frameCount"]       = frameCount_;
  doc["heapFreeBytes"]    = heapFreeBytes_;
  doc["heapMinFreeBytes"] = (heapMinFreeBytes_ == UINT32_MAX) ? 0 : heapMinFreeBytes_;
  doc["renderStackWords"] = renderStackWords_;
  doc["uptimeMs"]         = static_cast<uint32_t>(millis());

  String out;
  serializeJson(doc, out);
  return out;
}
