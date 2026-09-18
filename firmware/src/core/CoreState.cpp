/*
 * duxman-led next - v0.3.7-beta
 * Licensed under the Apache License 2.0
 * File: firmware/src/core/CoreState.cpp
 * Last commit: 2c35a63 - 2026-04-28
 */

#include "core/CoreState.h"
#include "core/ColorUtils.h"
#include "core/PaletteRegistry.h"
#include "effects/EffectRegistry.h"

#include <ArduinoJson.h>

namespace {
SemaphoreHandle_t gCoreStateMutex = nullptr;

uint8_t parseEffectId(JsonVariantConst value, uint8_t fallback) {
  if (value.isNull()) {
    return fallback;
  }

  if (value.is<const char *>()) {
    return EffectRegistry::parseId(String(value.as<const char *>()), fallback);
  }

  if (value.is<String>()) {
    return EffectRegistry::parseId(value.as<String>(), fallback);
  }

  if (value.is<int>()) {
    const int effectId = constrain(value.as<int>(), 0, 255);
    const EffectDescriptor *effect = EffectRegistry::findById(static_cast<uint8_t>(effectId));
    return effect != nullptr ? effect->id : fallback;
  }

  return fallback;
}

int16_t parsePaletteId(JsonVariantConst value, int16_t fallback) {
  if (value.isNull()) {
    return fallback;
  }

  if (value.is<const char *>()) {
    return PaletteRegistry::parseId(String(value.as<const char *>()), fallback);
  }

  if (value.is<String>()) {
    return PaletteRegistry::parseId(value.as<String>(), fallback);
  }

  if (value.is<int>()) {
    const int raw = value.as<int>();
    if (raw == PaletteRegistry::kManualPalette) {
      return PaletteRegistry::kManualPalette;
    }
    const ColorPaletteDescriptor *palette =
        PaletteRegistry::findById(static_cast<int16_t>(raw));
    return palette != nullptr ? palette->id : fallback;
  }

  return fallback;
}

uint8_t parseTransitionStyle(JsonVariantConst value, uint8_t fallback) {
  if (value.isNull()) {
    return fallback;
  }

  if (value.is<const char *>()) {
    const String style = String(value.as<const char *>());
    if (style == "wipe") {
      return 1;
    }
    if (style == "fade") {
      return 0;
    }
    return fallback;
  }

  if (value.is<String>()) {
    const String style = value.as<String>();
    if (style == "wipe") {
      return 1;
    }
    if (style == "fade") {
      return 0;
    }
    return fallback;
  }

  if (value.is<int>()) {
    return static_cast<uint8_t>(constrain(value.as<int>(), 0, 1));
  }

  return fallback;
}

const char *transitionStyleName(uint8_t style) {
  return style == 1 ? "wipe" : "fade";
}

bool isIntegerInRange(JsonVariantConst value, int minimum, int maximum) {
  if (!value.is<int>()) {
    return false;
  }
  const int number = value.as<int>();
  return number >= minimum && number <= maximum;
}

bool isValidEffect(JsonVariantConst value) {
  if (value.is<const char *>()) {
    return EffectRegistry::findByKey(String(value.as<const char *>())) != nullptr;
  }
  if (value.is<String>()) {
    return EffectRegistry::findByKey(value.as<String>()) != nullptr;
  }
  return value.is<int>() && value.as<int>() >= 0 &&
         value.as<int>() <= 255 &&
         EffectRegistry::findById(static_cast<uint8_t>(value.as<int>())) != nullptr;
}

bool isValidPalette(JsonVariantConst value) {
  if (value.is<const char *>()) {
    return PaletteRegistry::findByKey(String(value.as<const char *>())) != nullptr;
  }
  if (value.is<String>()) {
    return PaletteRegistry::findByKey(value.as<String>()) != nullptr;
  }
  if (!value.is<int>()) {
    return false;
  }
  const int paletteId = value.as<int>();
  return paletteId == PaletteRegistry::kManualPalette ||
         (paletteId >= 0 && paletteId <= 32767 &&
          PaletteRegistry::findById(static_cast<int16_t>(paletteId)) != nullptr);
}

bool isValidHexColor(String color) {
  color.trim();
  if (color.startsWith("#")) {
    color.remove(0, 1);
  }
  if (color.length() != 6) {
    return false;
  }
  for (size_t i = 0; i < color.length(); ++i) {
    if (!isxdigit(color[i])) {
      return false;
    }
  }
  return true;
}

bool isValidColor(JsonVariantConst value) {
  if (value.is<const char *>()) {
    return isValidHexColor(value.as<const char *>());
  }
  if (value.is<String>()) {
    return isValidHexColor(value.as<String>());
  }
  if (value.is<uint32_t>()) {
    return value.as<uint32_t>() <= 0xFFFFFFUL;
  }
  return value.is<int>() && value.as<int>() >= 0 && value.as<int>() <= 0xFFFFFF;
}

bool hasValidStatePatch(const JsonObjectConst &root, String *error) {
  if (!root["power"].isNull() && !root["power"].is<bool>()) {
    *error = "invalid_parameter";
    return false;
  }
  if (!root["brightness"].isNull() && !isIntegerInRange(root["brightness"], 0, 255)) {
    *error = "invalid_parameter";
    return false;
  }
  if (!root["effectId"].isNull() && !isValidEffect(root["effectId"])) {
    *error = "invalid_parameter";
    return false;
  }
  if (!root["effect"].isNull() && !isValidEffect(root["effect"])) {
    *error = "invalid_parameter";
    return false;
  }
  if (!root["sectionCount"].isNull() && !isIntegerInRange(root["sectionCount"], 1, 10)) {
    *error = "invalid_parameter";
    return false;
  }
  if (!root["effectSpeed"].isNull() && !isIntegerInRange(root["effectSpeed"], 1, 100)) {
    *error = "invalid_parameter";
    return false;
  }
  if (!root["effectLevel"].isNull() && !isIntegerInRange(root["effectLevel"], 1, 10)) {
    *error = "invalid_parameter";
    return false;
  }
  if (!root["effectTransitionMs"].isNull() &&
      !isIntegerInRange(root["effectTransitionMs"], 0, 1500)) {
    *error = "invalid_parameter";
    return false;
  }
  if (!root["effectTransitionStyle"].isNull() &&
      (root["effectTransitionStyle"].is<const char *>() ||
       root["effectTransitionStyle"].is<String>())) {
    const String style = root["effectTransitionStyle"].is<const char *>()
                             ? String(root["effectTransitionStyle"].as<const char *>())
                             : root["effectTransitionStyle"].as<String>();
    if (style != "fade" && style != "wipe") {
      *error = "invalid_parameter";
      return false;
    }
  } else if (!root["effectTransitionStyle"].isNull() &&
             !isIntegerInRange(root["effectTransitionStyle"], 0, 1)) {
    *error = "invalid_parameter";
    return false;
  }
  if (!root["paletteId"].isNull() && !isValidPalette(root["paletteId"])) {
    *error = "invalid_parameter";
    return false;
  }
  if (!root["palette"].isNull() && !isValidPalette(root["palette"])) {
    *error = "invalid_parameter";
    return false;
  }
  if (!root["primaryColors"].isNull()) {
    if (!root["primaryColors"].is<JsonArrayConst>()) {
      *error = "invalid_parameter";
      return false;
    }
    JsonArrayConst colors = root["primaryColors"].as<JsonArrayConst>();
    if (colors.size() != 3) {
      *error = "invalid_parameter";
      return false;
    }
    for (JsonVariantConst color : colors) {
      if (!isValidColor(color)) {
        *error = "invalid_parameter";
        return false;
      }
    }
  }
  if (!root["backgroundColor"].isNull() && !isValidColor(root["backgroundColor"])) {
    *error = "invalid_parameter";
    return false;
  }
  return true;
}
} // namespace

CoreState CoreState::defaults() {
  return CoreState{};
}

void CoreState::setMutex(SemaphoreHandle_t mutex) {
  gCoreStateMutex = mutex;
}

const char *CoreState::effectName(uint8_t effectId) {
  return EffectRegistry::keyFor(effectId);
}

const char *CoreState::effectLabel(uint8_t effectId) {
  return EffectRegistry::labelFor(effectId);
}

bool CoreState::lock(TickType_t timeout) const {
  if (gCoreStateMutex == nullptr) {
    return true;
  }
  return xSemaphoreTake(gCoreStateMutex, timeout) == pdTRUE;
}

void CoreState::unlock() const {
  if (gCoreStateMutex != nullptr) {
    xSemaphoreGive(gCoreStateMutex);
  }
}

CoreState CoreState::snapshot() const {
  if (!lock()) {
    return *this;
  }
  const CoreState copy = *this;
  unlock();
  return copy;
}

String CoreState::toJson() const {
  if (!lock()) {
    return "{}";
  }

  JsonDocument doc;
  doc["power"] = power;
  doc["brightness"] = brightness;
  doc["effectId"] = effectId;
  doc["effect"] = effectName(effectId);
  doc["effectLabel"] = effectLabel(effectId);
  doc["effectUsesAudio"] = EffectRegistry::usesAudio(effectId);
  doc["sectionCount"] = sectionCount;
  doc["effectSpeed"] = effectSpeed;
  doc["effectLevel"] = effectLevel;
  doc["effectTransitionMs"] = effectTransitionMs;
  doc["effectTransitionStyle"] = transitionStyleName(effectTransitionStyle);
  doc["paletteId"] = paletteId;
  doc["palette"] = PaletteRegistry::keyFor(paletteId);
  doc["paletteLabel"] = PaletteRegistry::labelFor(paletteId);
  doc["paletteStyle"] = PaletteRegistry::styleFor(paletteId);
  doc["reactiveToAudio"] = reactiveToAudio;
  doc["audioLevel"] = audioLevel;
  doc["beatDetected"] = beatDetected;
  doc["audioPeakHold"] = audioPeakHold;
  doc["availableEffects"] = serialized(EffectRegistry::toJsonArray());
  doc["availablePalettes"] = serialized(PaletteRegistry::toJsonArray());

  JsonArray colors = doc["primaryColors"].to<JsonArray>();
  for (uint8_t i = 0; i < 3; ++i) {
    colors.add(ColorUtils::formatHexColor(primaryColors[i]));
  }

  doc["backgroundColor"] = ColorUtils::formatHexColor(backgroundColor);

  String json;
  serializeJson(doc, json);
  unlock();
  return json;
}

bool CoreState::applyPatchJson(const String &payload) {
  return applyPatchJson(payload, nullptr);
}

bool CoreState::applyPatchJson(const String &payload, String *error) {
  if (error != nullptr) {
    error->clear();
  }

  JsonDocument doc;
  if (deserializeJson(doc, payload)) {
    if (error != nullptr) {
      *error = "invalid_json";
    }
    return false;
  }

  const JsonObjectConst root = doc.as<JsonObjectConst>();
  if (root.isNull()) {
    if (error != nullptr) {
      *error = "invalid_parameter";
    }
    return false;
  }

  String validationError;
  if (!hasValidStatePatch(root, error != nullptr ? error : &validationError)) {
    return false;
  }

  if (!lock()) {
    return false;
  }

  CoreState next = *this;

  if (!root["power"].isNull()) {
    next.power = root["power"].as<bool>();
  }

  if (!root["brightness"].isNull()) {
    next.brightness = static_cast<uint8_t>(constrain(root["brightness"].as<int>(), 0, 255));
  }

  if (!root["effectId"].isNull()) {
    next.effectId = parseEffectId(root["effectId"], next.effectId);
  }

  if (!root["effect"].isNull()) {
    next.effectId = parseEffectId(root["effect"], next.effectId);
  }

  // La reactividad al audio ya no es un toggle global del motor base.
  // Se deriva automáticamente del tipo de efecto seleccionado.
  next.reactiveToAudio = EffectRegistry::usesAudio(next.effectId);

  if (!root["sectionCount"].isNull()) {
    next.sectionCount = static_cast<uint8_t>(constrain(root["sectionCount"].as<int>(), 1, 10));
  }

  if (!root["effectSpeed"].isNull()) {
    next.effectSpeed = static_cast<uint8_t>(constrain(root["effectSpeed"].as<int>(), 1, 100));
  }

  if (!root["effectLevel"].isNull()) {
    next.effectLevel = static_cast<uint8_t>(constrain(root["effectLevel"].as<int>(), 1, 10));
  }

  if (!root["effectTransitionMs"].isNull()) {
    next.effectTransitionMs = static_cast<uint16_t>(constrain(root["effectTransitionMs"].as<int>(), 0, 1500));
  }

  if (!root["effectTransitionStyle"].isNull()) {
    next.effectTransitionStyle = parseTransitionStyle(root["effectTransitionStyle"], next.effectTransitionStyle);
  }

  if (!root["paletteId"].isNull()) {
    next.paletteId = parsePaletteId(root["paletteId"], next.paletteId);
    if (next.paletteId >= 0) {
      PaletteRegistry::applyToColors(next.paletteId, next.primaryColors);
    }
  }

  if (!root["palette"].isNull()) {
    next.paletteId = parsePaletteId(root["palette"], next.paletteId);
    if (next.paletteId >= 0) {
      PaletteRegistry::applyToColors(next.paletteId, next.primaryColors);
    }
  }

  if (root["primaryColors"].is<JsonArrayConst>()) {
    JsonArrayConst colors = root["primaryColors"].as<JsonArrayConst>();
    for (uint8_t i = 0; i < 3 && i < colors.size(); ++i) {
      next.primaryColors[i] = ColorUtils::parseColorValue(colors[i], next.primaryColors[i]);
    }
    next.paletteId = PaletteRegistry::kManualPalette;
  }

  next.backgroundColor = ColorUtils::parseColorValue(root["backgroundColor"], next.backgroundColor);

  bool changed = next.power != power || next.brightness != brightness ||
                 next.effectId != effectId || next.sectionCount != sectionCount ||
                 next.effectSpeed != effectSpeed || next.effectLevel != effectLevel ||
                 next.effectTransitionMs != effectTransitionMs ||
                 next.effectTransitionStyle != effectTransitionStyle ||
                 next.paletteId != paletteId ||
                 next.reactiveToAudio != reactiveToAudio ||
                 next.backgroundColor != backgroundColor;
  for (uint8_t i = 0; i < 3 && !changed; ++i) {
    if (next.primaryColors[i] != primaryColors[i]) {
      changed = true;
    }
  }

  *this = next;
  unlock();

  return changed;
}
