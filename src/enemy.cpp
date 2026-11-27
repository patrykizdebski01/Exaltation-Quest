#include "enemy.hpp"
#include "game_system.hpp"

//This function gives the player score when an enemy dies.
void enemy_killed(int enemyScore)
{
    //Add one to the player's kill count.
    ++GameSystem::player.kills;

    //Add the enemy's score to the player's total score.
    GameSystem::player.score += enemyScore;
}
