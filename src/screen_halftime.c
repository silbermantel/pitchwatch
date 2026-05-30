#include <pebble.h>
#include "types.h"
#include "game_logic.h"
#include "screen_halftime.h"
#include "screen_game.h"

static Window    *s_window;
static TextLayer *s_label;

static void select_click(ClickRecognizerRef r, void *ctx) {
  g_game.current_half     = 2;
  g_game.current_player   = 0;
  g_game.turns_left[0]    = g_config.turns_per_half;
  g_game.turns_left[1]    = g_config.turns_per_half;
  g_game.turn_elapsed     = 0;
  g_game.paused           = false;
  g_game.alarm_active     = false;
  g_game.confirm_end_turn = false;
  g_game.state            = STATE_GAME;
  window_stack_pop(true);
  game_logic_start_timer();
  screen_game_update_display();
}

static void click_config(void *ctx) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click);
}

static void window_load(Window *win) {
  Layer *root   = window_get_root_layer(win);
  GRect  bounds = layer_get_bounds(root);

  s_label = text_layer_create(bounds);
  text_layer_set_text(s_label, "HALBZEIT!\n\nSELECT\nfuer 2. Halbzeit");
  text_layer_set_font(s_label, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_label, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_label));

  window_set_click_config_provider(win, click_config);
  vibes_double_pulse();
}

static void window_unload(Window *win) {
  text_layer_destroy(s_label);
}

void screen_halftime_init(void) {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers){
    .load   = window_load,
    .unload = window_unload,
  });
}

void screen_halftime_deinit(void) { window_destroy(s_window); }

Window *screen_halftime_get_window(void) { return s_window; }
