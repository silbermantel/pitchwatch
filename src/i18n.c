#include <pebble.h>
#include "i18n.h"

// ---- String-Tabellen ----
static const char *s_strings_de[I18N_KEY_COUNT] = {
  [KEY_PLAYER]          = "Spieler %d",
  [KEY_PAUSE]           = "PAUSE  [SELECT]",
  [KEY_HINT]            = "[UP]=Pause [SEL]=Zug?",
  [KEY_CONFIRM_END]     = "Zug beenden? [SEL]",
  [KEY_TURN_OVER]       = "ZUG ABGELAUFEN!",
  [KEY_TIME_UP]         = "ZEIT! SPIELENDE",
  [KEY_GAME_OVER]       = "SPIELENDE!",
  [KEY_HZ]              = "HZ",
  [KEY_HALFTIME_TITLE]  = "HALBZEIT!",
  [KEY_HALFTIME_BODY]   = "SELECT\nfuer 2. Halbzeit",
  [KEY_MENU_TITLE]      = "Blood Bowl Uhr",
  [KEY_MENU_TURNS]      = "Zuege/Halbzeit",
  [KEY_MENU_DURATION]   = "Spielzeit",
  [KEY_MENU_START]      = "SPIEL STARTEN",
  [KEY_MENU_START_SUB]  = "SELECT druecken",
};

static const char *s_strings_en[I18N_KEY_COUNT] = {
  [KEY_PLAYER]          = "Player %d",
  [KEY_PAUSE]           = "PAUSE  [SELECT]",
  [KEY_HINT]            = "[UP]=Pause [SEL]=End?",
  [KEY_CONFIRM_END]     = "End turn? [SEL]",
  [KEY_TURN_OVER]       = "TURN OVER!",
  [KEY_TIME_UP]         = "TIME! GAME OVER",
  [KEY_GAME_OVER]       = "GAME OVER!",
  [KEY_HZ]              = "HT",
  [KEY_HALFTIME_TITLE]  = "HALF TIME!",
  [KEY_HALFTIME_BODY]   = "SELECT\nfor 2nd Half",
  [KEY_MENU_TITLE]      = "Blood Bowl Clock",
  [KEY_MENU_TURNS]      = "Turns/Half",
  [KEY_MENU_DURATION]   = "Game duration",
  [KEY_MENU_START]      = "START GAME",
  [KEY_MENU_START_SUB]  = "Press SELECT",
};

// Neue Sprache hinzufügen: Array anlegen, in i18n_init() eintragen.

static const char **s_active = NULL;

void i18n_init(void) {
  const char *locale = i18n_get_system_locale();
  if (locale && strncmp(locale, "de", 2) == 0) {
    s_active = s_strings_de;
  } else {
    // Fallback: Englisch
    s_active = s_strings_en;
  }
}

const char *i18n_get(I18nKey key) {
  if (!s_active || key >= I18N_KEY_COUNT) return "???";
  return s_active[key];
}