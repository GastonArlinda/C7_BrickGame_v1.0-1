#include "gui/cli/cli.h"

int main() {
  GameInfo_t game_info;
  GameState_t current_state = State_Start;

  initialize_game(&game_info);
  game_loop(&game_info, &current_state);

  InfoFree(&game_info);
  endwin();
  return 0;
}