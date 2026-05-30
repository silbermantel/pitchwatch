#include <pebble.h>
#include "types.h"
#include "game_logic.h"
#include "screen_game.h"
#include "screen_halftime.h"

static AppTimer *s_tick_timer = NULL;

static int s_turns_options[]    = {6, 7, 8};
static int s_duration_real[]    = {3600, 4500, 5400};
static int s_sel_turns          = 0;
static int s_sel_duration       = 0;

// Diese werden von screen_settings.c gesetzt
void game_logic_set_sel_turns(int idx)    { s_sel_turns    = idx; }
void game_logic_set_sel_duration(int idx) { s_sel_duration = idx; }
int  game_logic_get_sel_turns(void)       { return s_sel_turns; }
int  game_logic_get_sel_duration(void)    { return s_sel_duration; }

// Labels — auch von screen_settings gebraucht
const char *game_logic_turns_label(int idx)    {
  static const char *l[] = {"6","7","8"}; return l[idx];
}
const char *game_logic_duration_label(int idx) {
  static const char *l[] = {"60 min","75 min","90 min"}; return l[idx];
}
int game_logic_turns_count(void)    { return 3; }
int game_logic_duration_count(void) { return 3; }

void game_logic_recalc_max_turn(void) {
  g_config.turns_per_half   = s_turns_options[s_sel_turns];
  g_config.total_seconds    = s_duration_real[s_sel_duration];
  g_config.max_turn_seconds = g_config.total_seconds / (g_config.turns_per_half * 2);
}

void game_logic_stop_timer(void) {
  if (s_tick_timer) {
    app_timer_cancel(s_tick_timer);
    s_tick_timer = NULL;
  }
}

static void tick_callback(void *context);

void game_logic_start_timer(void) {
  game_logic_stop_timer();
  s_tick_timer = app_timer_register(1000, tick_callback, NULL);
}

void game_logic_trigger_alarm(const char *reason) {
  g_game.alarm_active = true;
  g_game.paused       = true;
  game_logic_stop_timer();
  vibes_long_pulse();
  screen_game_set_status(reason);
}

void game_logic_next_turn(void) {
  g_game.alarm_active     = false;
  g_game.confirm_end_turn = false;
  g_game.turns_left[g_game.current_player]--;
  g_game.turn_elapsed = 0;

  if (g_game.turns_left[0] == 0 && g_game.turns_left[1] == 0) {
    if (g_game.current_half == 1) {
      g_game.state = STATE_HALFTIME;
      game_logic_stop_timer();
      window_stack_push(screen_halftime_get_window(), true);
      return;
    } else {
      game_logic_trigger_alarm("SPIELENDE!");
      g_game.state = STATE_GAMEOVER;
      screen_game_update_display();
      return;
    }
  }

  g_game.current_player = 1 - g_game.current_player;
  g_game.paused         = false;
  game_logic_start_timer();
  screen_game_update_display();
}

static void tick_callback(void *context) {
  if (g_game.paused) return;

  g_game.total_elapsed++;
  g_game.turn_elapsed++;

  if (g_game.total_elapsed >= g_config.total_seconds) {
    game_logic_trigger_alarm("ZEIT! SPIELENDE");
    g_game.state = STATE_GAMEOVER;
    screen_game_update_display();
    return;
  }

  if (g_config.max_turn_seconds > 0 &&
      g_game.turn_elapsed >= g_config.max_turn_seconds) {
    game_logic_trigger_alarm("ZUG ABGELAUFEN!");
    screen_game_update_display();
    return;
  }

  screen_game_update_display();
  game_logic_start_timer();
}

void game_logic_start_game(void) {
  game_logic_recalc_max_turn();
  g_game.state            = STATE_GAME;
  g_game.current_player   = 0;
  g_game.current_half     = 1;
  g_game.turns_left[0]    = g_config.turns_per_half;
  g_game.turns_left[1]    = g_config.turns_per_half;
  g_game.total_elapsed    = 0;
  g_game.turn_elapsed     = 0;
  g_game.paused           = false;
  g_game.alarm_active     = false;
  g_game.confirm_end_turn = false;
  window_stack_push(screen_game_get_window(), true);
}
