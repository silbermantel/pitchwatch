#pragma once
#include <pebble.h>

typedef struct {
  int turns_per_half;
  int total_seconds;
  int max_turn_seconds;
} GameConfig;

typedef enum {
  STATE_SETTINGS,
  STATE_GAME,
  STATE_HALFTIME,
  STATE_GAMEOVER
} GameState;

typedef struct {
  GameState state;
  int  current_player;
  int  current_half;
  int  turns_left[2];
  int  total_elapsed;
  int  turn_elapsed;
  bool paused;
  bool alarm_active;
  bool confirm_end_turn;
} GameData;

// Globale Instanzen — definiert in main.c, extern überall verfügbar
extern GameConfig g_config;
extern GameData   g_game;