#pragma once

// Alle String-Keys der App
typedef enum {
  // Screen: Game
  KEY_PLAYER,           // "Spieler %d" / "Player %d"
  KEY_PAUSE,            // "PAUSE  [SELECT]"
  KEY_HINT,             // "[UP]=Pause [SEL]=Zug?"
  KEY_CONFIRM_END,      // "Zug beenden? [SEL]"
  KEY_TURN_OVER,        // "ZUG ABGELAUFEN!"
  KEY_TIME_UP,          // "ZEIT! SPIELENDE"
  KEY_GAME_OVER,        // "SPIELENDE!"
  KEY_HZ,               // "HZ"

  // Screen: Halftime
  KEY_HALFTIME_TITLE,   // "HALBZEIT!"
  KEY_HALFTIME_BODY,    // "SELECT\nfuer 2. Halbzeit"

  // Screen: Settings
  KEY_MENU_TITLE,       // "Blood Bowl Uhr"
  KEY_MENU_TURNS,       // "Zuege/Halbzeit"
  KEY_MENU_DURATION,    // "Spielzeit"
  KEY_MENU_START,       // "SPIEL STARTEN"
  KEY_MENU_START_SUB,   // "SELECT druecken"

  I18N_KEY_COUNT        // Sentinel — immer am Ende lassen
} I18nKey;

// Initialisierung — einmal in init() aufrufen
void i18n_init(void);

// String für aktuelle Locale abrufen
const char *i18n_get(I18nKey key);