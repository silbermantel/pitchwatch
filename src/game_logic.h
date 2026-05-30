#pragma once

void        game_logic_recalc_max_turn(void);
void        game_logic_start_game(void);
void        game_logic_next_turn(void);
void        game_logic_trigger_alarm(const char *reason);
void        game_logic_start_timer(void);
void        game_logic_stop_timer(void);

void        game_logic_set_sel_turns(int idx);
void        game_logic_set_sel_duration(int idx);
int         game_logic_get_sel_turns(void);
int         game_logic_get_sel_duration(void);
const char *game_logic_turns_label(int idx);
const char *game_logic_duration_label(int idx);
int         game_logic_turns_count(void);
int         game_logic_duration_count(void);
