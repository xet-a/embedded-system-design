#include "common.h"
#include "game.h"

//void Game_update();
void Game_start();

GameStatus gameStatus;

void Game_start(){
    gameStatus.start = 0;
    gameStatus.score = 0;
}