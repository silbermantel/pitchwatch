#include <pebble.h>
#include "types.h"
#include "i18n.h"
#include "game_logic.h"
#include "screen_settings.h"

static Window          *s_window;
static SimpleMenuLayer *s_menu_layer;
static SimpleMenuSection s_menu_section;
static SimpleMenuItem    s_menu_items[3];

static void refresh_menu(void) {
  s_menu_items[0].subtitle = game_logic_turns_label(game_logic_get_sel_turns());
  s_menu_items[1].subtitle = game_logic_duration_label(game_logic_get_sel_duration());
  layer_mark_dirty(simple_menu_layer_get_layer(s_menu_layer));
}

static void cb_turns(int index, void *ctx) {
  game_logic_set_sel_turns(
    (game_logic_get_sel_turns() + 1) % game_logic_turns_count());
  game_logic_recalc_max_turn();
  refresh_menu();
}

static void cb_duration(int index, void *ctx) {
  game_logic_set_sel_duration(
    (game_logic_get_sel_duration() + 1) % game_logic_duration_count());
  game_logic_recalc_max_turn();
  refresh_menu();
}

static void cb_start(int index, void *ctx) {
  game_logic_start_game();
}

static void window_load(Window *win) {
  Layer *root   = window_get_root_layer(win);
  GRect  bounds = layer_get_bounds(root);

  game_logic_recalc_max_turn();

  s_menu_items[0] = (SimpleMenuItem){
    .title    = i18n_get(KEY_MENU_TURNS),
    .subtitle = game_logic_turns_label(game_logic_get_sel_turns()),
    .callback = cb_turns,
  };
  s_menu_items[1] = (SimpleMenuItem){
    .title    = i18n_get(KEY_MENU_DURATION),
    .subtitle = game_logic_duration_label(game_logic_get_sel_duration()),
    .callback = cb_duration,
  };
  s_menu_items[2] = (SimpleMenuItem){
    .title    = i18n_get(KEY_MENU_START),
    .subtitle = i18n_get(KEY_MENU_START_SUB),
    .callback = cb_start,
  };

  s_menu_section = (SimpleMenuSection){
    .title     = i18n_get(KEY_MENU_TITLE),
    .items     = s_menu_items,
    .num_items = 3,
  };

  s_menu_layer = simple_menu_layer_create(bounds, win, &s_menu_section, 1, NULL);
  layer_add_child(root, simple_menu_layer_get_layer(s_menu_layer));
}

static void window_unload(Window *win) {
  simple_menu_layer_destroy(s_menu_layer);
}

void screen_settings_init(void) {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers){
    .load   = window_load,
    .unload = window_unload,
  });
}

void screen_settings_deinit(void) { window_destroy(s_window); }

Window *screen_settings_get_window(void) { return s_window; }
