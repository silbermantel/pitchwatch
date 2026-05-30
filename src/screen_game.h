#pragma once
#include <pebble.h>

void    screen_game_init(void);
void    screen_game_deinit(void);
Window *screen_game_get_window(void);
void    screen_game_update_display(void);
void    screen_game_set_status(const char *text);
