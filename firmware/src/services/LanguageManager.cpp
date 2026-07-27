#include "LanguageManager.h"

#include <ArduinoJson.h>
#include <LittleFS.h>

namespace {
constexpr const char *kLittleFsI18nPrefix = "/ui/i18n/";
constexpr const char *kJsonPathSeparator = ".";
}

// Embedded fallback packs. LittleFS remains the primary source of truth.

static const char* packEN PROGMEM = R"({
  "lang": "en",
  "name": "English",
  "nav": {
    "home": "Home",
    "config": "Config",
    "api": "API",
    "settings": "Settings",
    "advanced": "Advanced",
    "help": "Help"
  },
  "common": {
    "save": "Save",
    "cancel": "Cancel",
    "apply": "Apply",
    "delete": "Delete",
    "loading": "Loading...",
    "error": "Error",
    "success": "Success"
  }
})";

static const char* packES PROGMEM = R"({
  "lang": "es",
  "name": "Español",
  "nav": {
    "home": "Inicio",
    "config": "Config",
    "api": "API",
    "settings": "Configuración",
    "advanced": "Avanzado",
    "help": "Ayuda"
  },
  "common": {
    "save": "Guardar",
    "cancel": "Cancelar",
    "apply": "Aplicar",
    "delete": "Eliminar",
    "loading": "Cargando...",
    "error": "Error",
    "success": "Éxito"
  }
})";

// Global instance
LanguageManager gLanguageManager;

LanguageManager::LanguageManager() 
    : currentLanguage_(DEFAULT_LANGUAGE), 
  currentLanguageEnum_(Language::ENGLISH),
  currentPackJson_(String(packEN)),
  fallbackPackJson_(String(packEN)) {}

void LanguageManager::begin(const String& language) {
  setLanguage(language);
}

void LanguageManager::setLanguage(const String& languageCode) {
  String fallbackPack;
  if (!loadFromLittleFS_(FALLBACK_LANGUAGE, &fallbackPack) &&
      !loadEmbeddedPack_(FALLBACK_LANGUAGE, &fallbackPack)) {
    fallbackPack = String(packEN);
  }
  fallbackPackJson_ = fallbackPack;

  String currentPack;
  if (loadFromLittleFS_(languageCode, &currentPack) || loadEmbeddedPack_(languageCode, &currentPack)) {
    currentLanguage_ = languageCode;
    currentLanguageEnum_ = getLanguageEnum(languageCode);
    currentPackJson_ = currentPack;
    Serial.printf("[i18n] Language active: %s\n", currentLanguage_.c_str());
    return;
  }

  Serial.printf("[i18n] Language '%s' not found, falling back to '%s'\n",
                languageCode.c_str(), FALLBACK_LANGUAGE);
  currentLanguage_ = FALLBACK_LANGUAGE;
  currentLanguageEnum_ = getLanguageEnum(FALLBACK_LANGUAGE);
  currentPackJson_ = fallbackPackJson_;
}

LanguageManager::Language LanguageManager::getLanguageEnum(const String& code) const {
  if (code == "en") return Language::ENGLISH;
  if (code == "es") return Language::SPANISH;
  if (code == "fr") return Language::FRENCH;
  if (code == "de") return Language::GERMAN;
  if (code == "it") return Language::ITALIAN;
  return Language::UNKNOWN;
}

String LanguageManager::t(const char* key) const {
  if (!key || strlen(key) == 0) {
    return String(key);
  }

  const String translated = getFromJson_(currentPackJson_, key);
  if (!translated.isEmpty()) {
    return translated;
  }

  const String fallback = getFromJson_(fallbackPackJson_, key);
  if (!fallback.isEmpty()) {
    return fallback;
  }

  return String(key);
}

String LanguageManager::getEffectName(uint8_t effectId) const {
  // Placeholder - would look up effect names in current language
  return "Effect_" + String(effectId);
}

String LanguageManager::getPaletteName(uint8_t paletteId) const {
  // Placeholder - would look up palette names in current language
  return "Palette_" + String(paletteId);
}

String LanguageManager::formatNumber(int32_t value, bool includeSign) const {
  const bool negative = value < 0;
  uint32_t absValue = static_cast<uint32_t>(negative ? -value : value);
  String digits = String(absValue);
  String result;
  const char separator = currentLanguage_ == "en" ? ',' : '.';

  for (int i = static_cast<int>(digits.length()) - 1, group = 0; i >= 0; --i, ++group) {
    if (group > 0 && group % 3 == 0) {
      result = String(separator) + result;
    }
    result = String(digits.charAt(i)) + result;
  }

  if (negative) {
    result = "-" + result;
  } else if (includeSign) {
    result = "+" + result;
  }
  return result;
}

String LanguageManager::formatPercent(uint8_t percent) const {
  return String(percent) + "%";
}

String LanguageManager::formatTime(uint32_t seconds) const {
  uint32_t hours = seconds / 3600;
  uint32_t minutes = (seconds % 3600) / 60;
  uint32_t secs = seconds % 60;
  
  String result = "";
  if (hours < 10) result += "0";
  result += String(hours) + ":";
  if (minutes < 10) result += "0";
  result += String(minutes) + ":";
  if (secs < 10) result += "0";
  result += String(secs);
  
  return result;
}

String LanguageManager::formatFileSize(uint32_t bytes) const {
  const char* units[] = {"B", "KB", "MB", "GB"};
  int unitIndex = 0;
  float size = bytes;
  
  while (size >= 1024 && unitIndex < 3) {
    size /= 1024;
    unitIndex++;
  }
  
  char buffer[16];
  if (size < 10) {
    snprintf(buffer, sizeof(buffer), "%.2f %s", size, units[unitIndex]);
  } else {
    snprintf(buffer, sizeof(buffer), "%.1f %s", size, units[unitIndex]);
  }
  
  return String(buffer);
}

bool LanguageManager::loadEmbeddedPack_(const String& languageCode, String* target) {
  if (target == nullptr) {
    return false;
  }
  if (languageCode == "en") {
    *target = String(packEN);
    Serial.println("[i18n] Loaded embedded EN language pack");
    return true;
  }
  if (languageCode == "es") {
    *target = String(packES);
    Serial.println("[i18n] Loaded embedded ES language pack");
    return true;
  }
  return false;
}

bool LanguageManager::loadFromLittleFS_(const String& languageCode, String* target) {
  if (target == nullptr || languageCode.isEmpty()) {
    return false;
  }

  const String path = String(kLittleFsI18nPrefix) + languageCode + ".json";
  if (!LittleFS.exists(path)) {
    return false;
  }

  File file = LittleFS.open(path, "r");
  if (!file) {
    return false;
  }

  const String content = file.readString();
  file.close();
  if (content.isEmpty()) {
    return false;
  }

  *target = content;
  Serial.printf("[i18n] Loaded LittleFS language pack: %s\n", path.c_str());
  return true;
}

String LanguageManager::getFromJson_(const String& jsonData, const String& key) const {
  if (jsonData.isEmpty() || key.isEmpty()) {
    return String();
  }

  JsonDocument doc;
  if (deserializeJson(doc, jsonData)) {
    return String();
  }

  JsonVariantConst current = doc.as<JsonVariantConst>();
  int start = 0;
  while (start < static_cast<int>(key.length())) {
    const int separatorPos = key.indexOf(kJsonPathSeparator, start);
    const String part = separatorPos < 0 ? key.substring(start) : key.substring(start, separatorPos);
    JsonObjectConst object = current.as<JsonObjectConst>();
    if (object.isNull()) {
      return String();
    }

    current = object[part.c_str()];
    if (current.isNull()) {
      return String();
    }

    if (separatorPos < 0) {
      break;
    }
    start = separatorPos + 1;
  }

  if (current.is<const char*>()) {
    return String(current.as<const char*>());
  }
  if (current.is<String>()) {
    return current.as<String>();
  }

  String out;
  serializeJson(current, out);
  return out;
}

String LanguageManager::getAllStringsJson() const {
  return currentPackJson_.isEmpty() ? fallbackPackJson_ : currentPackJson_;
}

String LanguageManager::getLanguageName(const String& code) const {
  if (code == "en") return "English";
  if (code == "es") return "Español";
  if (code == "fr") return "Français";
  if (code == "de") return "Deutsch";
  if (code == "it") return "Italiano";
  return code;
}

String LanguageManager::getLanguageCode(Language lang) const {
  switch (lang) {
    case Language::ENGLISH: return "en";
    case Language::SPANISH: return "es";
    case Language::FRENCH: return "fr";
    case Language::GERMAN: return "de";
    case Language::ITALIAN: return "it";
    default: return "en";
  }
}
