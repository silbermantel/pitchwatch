#include <pebble.h>
#include "types.h"
#include "game_logic.h"
#include "screen_game.h"

static Window    *s_window;
static TextLayer *s_player_layer;
static TextLayer *s_turn_time_layer;
static TextLayer *s_total_time_layer;
static TextLayer *s_turns_layer;
static TextLayer *s_status_layer;

static void format_time(char *buf, int seconds) {
  snprintf(buf, 8, "%02d:%02d", seconds / 60, seconds % 60);
}

void screen_game_set_status(const char *text) {
  text_layer_set_text(s_status_layer, text);
}

void screen_game_update_display(void) {
  static char turn_buf[8], total_buf[8], player_buf[16], turns_buf[32];

  int remaining_turn  = g_config.max_turn_seconds > 0
                        ? g_config.max_turn_seconds - g_game.turn_elapsed
                        : g_game.turn_elapsed;
  int remaining_total = g_config.total_seconds - g_game.total_elapsed;

  format_time(turn_buf,  remaining_turn  < 0 ? 0 : remaining_turn);
  format_time(total_buf, remaining_total < 0 ? 0 : remaining_total);

  snprintf(player_buf, sizeof(player_buf), "Spieler %d", g_game.current_player + 1);
  snprintf(turns_buf,  sizeof(turns_buf),
           "HZ %d  |  Zug %d/%d",
           g_game.current_half,
           g_config.turns_per_half - g_game.turns_left[g_game.current_player] + 1,
           g_config.turns_per_half);

  text_layer_set_text(s_player_layer,     player_buf);
  text_layer_set_text(s_turn_time_layer,  turn_buf);
  text_layer_set_text(s_total_time_layer, total_buf);
  text_layer_set_text(s_turns_layer,      turns_buf);

  if (!g_game.alarm_active) {
    if (g_game.confirm_end_turn) {
      screen_game_set_status("Zug beenden? [SEL]");
    } else if (g_game.paused) {
      screen_game_set_status("PAUSE  [SELECT]");
    } else {
      screen_game_set_status("[UP]=Pause [SEL]=Zug?");
    }
  }
}

// ---- Button Handler ----
static void select_click(ClickRecognizerRef r, void *ctx) {
  if (g_game.state == STATE_GAMEOVER) return;

  if (g_game.alarm_active) {
    g_game.paused           = false;
    g_game.confirm_end_turn = false;
    game_logic_next_turn();
    return;
  }
  if (g_game.paused) {
    g_game.paused           = false;
    g_game.confirm_end_turn = false;
    game_logic_start_timer();
    screen_game_update_display();
    return;
  }
  if (!g_game.confirm_end_turn) {
    g_game.confirm_end_turn = true;
    screen_game_update_display();
    return;
  }
  g_game.confirm_end_turn = false;
  game_logic_next_turn();
}

static void up_click(ClickRecognizerRef r, void *ctx) {
  if (g_game.state == STATE_GAMEOVER || g_game.alarm_active) return;

  if (g_game.confirm_end_turn) {
    g_game.confirm_end_turn = false;
    screen_game_update_display();
    return;
  }
  g_game.paused = !g_game.paused;
  if (!g_game.paused) {
    game_logic_start_timer();
  } else {
    game_logic_stop_timer();
  }
  screen_game_update_display();
}

static void back_click(ClickRecognizerRef r, void *ctx) {
  if (!g_game.paused && g_game.state != STATE_GAMEOVER) return;
  game_logic_stop_timer();
  window_stack_pop(true);
}

static void click_config(void *ctx) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click);
  window_single_click_subscribe(BUTTON_ID_UP,     up_click);
  window_single_click_subscribe(BUTTON_ID_BACK,   back_click);
}

// ---- Window Lifecycle ----
static void window_load(Window *win) {
  Layer *root   = window_get_root_layer(win);
  GRect  bounds = layer_get_bounds(root);

  s_player_layer = text_layer_create(GRect(0, 4, bounds.size.w, 28));
  text_layer_set_font(s_player_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_player_layer, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_player_layer));

  s_turn_time_layer = text_layer_create(GRect(0, 34, bounds.size.w, 42));
  text_layer_set_font(s_turn_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_turn_time_layer, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_turn_time_layer));

  s_total_time_layer = text_layer_create(GRect(0, 80, bounds.size.w, 22));
  text_layer_set_font(s_total_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_total_time_layer, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_total_time_layer));

  s_turns_layer = text_layer_create(GRect(0, 104, bounds.size.w, 20));
  text_layer_set_font(s_turns_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_turns_layer, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_turns_layer));

  s_status_layer = text_layer_create(GRect(0, 126, bounds.size.w, 20));
  text_layer_set_font(s_status_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_status_layer, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_status_layer));

  window_set_click_config_provider(win, click_config);
  screen_game_update_display();
  game_logic_start_timer();
}

static void window_unload(Window *win) {
  game_logic_stop_timer();
  text_layer_destroy(s_player_layer);
  text_layer_destroy(s_turn_time_layer);
  text_layer_destroy(s_total_time_layer);
  text_layer_destroy(s_turns_layer);
  text_layer_destroy(s_status_layer);
}

void screen_game_init(void) {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers){
    .load   = window_load,
    .unload = window_unload,
  });
}

void screen_game_deinit(void) { window_destroy(s_window); }

Window *screen_game_get_window(void) { return s_window; }
