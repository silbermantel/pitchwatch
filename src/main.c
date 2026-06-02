#include <pebble.h>
#include "types.h"
#include "screen_settings.h"
#include "screen_game.h"
#include "screen_halftime.h"
#include "i18n.h"

GameConfig g_config;
GameData   g_game;

static void init(void) {
  i18n_init();
  screen_settings_init();
  screen_game_init();
  screen_halftime_init();
  window_stack_push(screen_settings_get_window(), true);
}

static void deinit(void) {
  screen_settings_deinit();
  screen_game_deinit();
  screen_halftime_deinit();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
