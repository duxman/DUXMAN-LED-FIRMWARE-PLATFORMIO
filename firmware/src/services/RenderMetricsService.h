/*
 * duxman-led next
 * Licensed under the Apache License 2.0
 * File: firmware/src/services/RenderMetricsService.h
 *
 * Servicio de metricas del loop de render.
 *
 * Uso desde renderTask:
 *   gRenderMetrics.frameBegin();
 *   // ... renderFrame() ...
 *   gRenderMetrics.frameEnd();
 *
 * Consulta desde cualquier tarea (control, API):
 *   String json = gRenderMetrics.toJson();
 *   gRenderMetrics.reset();
 *
 * Metricas expuestas:
 *   fps              – media de frames por segundo (ventana deslizante de 1 s)
 *   frameTimeAvgUs   – tiempo medio de render por frame (microsegundos)
 *   frameTimeMinUs   – minimo de render desde el ultimo reset
 *   frameTimeMaxUs   – maximo de render desde el ultimo reset
 *   frameCount       – total de frames desde boot
 *   heapFreeBytes    – heap libre actual
 *   heapMinFreeBytes – minimo heap libre desde boot
 *   renderStackWords – headroom de la tarea render (palabras FreeRTOS)
 *   uptimeMs         – tiempo desde boot (millis)
 *
 * Seguridad de hilos:
 *   frameBegin/frameEnd solo los llama el renderTask (core 1).
 *   toJson/reset los llama el controlTask (core 0).
 *   Los campos son volatile uint32_t; lectura escalar atomica en ESP32.
 *   Suficiente para metricas de diagnostico (no se requiere exactitud critica).
 */

#pragma once

#include <Arduino.h>

class RenderMetricsService {
public:
  RenderMetricsService() = default;

  // Llamar justo antes de renderFrame() desde renderTask.
  void frameBegin();

  // Llamar justo despues de renderFrame() desde renderTask.
  void frameEnd();

  // Resetea min/max/acumuladores de frametime.
  // No resetea frameCount ni uptimeMs.
  void reset();

  // Devuelve un JSON con todas las metricas.
  String toJson() const;

private:
  // --- FPS (ventana de 1 s) -------------------------------------------------
  volatile uint32_t fpsCurrent_   = 0;   // FPS del ultimo segundo completo
  uint32_t          fpsCounter_   = 0;   // frames contados en la ventana actual
  uint32_t          fpsWindowMs_  = 0;   // inicio de la ventana actual

  // --- Frame timing ---------------------------------------------------------
  int64_t           frameBeginUs_ = 0;   // timestamp de frameBegin (us)

  volatile uint32_t frameTimeAvgUs_ = 0; // media acumulada (us)
  volatile uint32_t frameTimeMinUs_ = UINT32_MAX;
  volatile uint32_t frameTimeMaxUs_ = 0;
  uint64_t          frameTimeAccUs_ = 0; // acumulador para media

  // --- Contadores -----------------------------------------------------------
  volatile uint32_t frameCount_ = 0;     // total frames desde boot

  // --- Heap (muestreado cada kHeapSamplePeriod frames) ----------------------
  static constexpr uint8_t kHeapSamplePeriod = 60;
  uint8_t heapSampleCounter_ = 0;

  volatile uint32_t heapFreeBytes_    = 0;
  volatile uint32_t heapMinFreeBytes_ = UINT32_MAX;

  // --- Stack headroom -------------------------------------------------------
  volatile uint32_t renderStackWords_ = 0;
};

// Instancia global (definida en RenderMetricsService.cpp).
extern RenderMetricsService gRenderMetrics;
