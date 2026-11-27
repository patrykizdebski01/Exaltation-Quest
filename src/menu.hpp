#pragma once
// EMPTY FOR NOW. Still to add here (brief: main menu, quit to menu, restart, graphics options):
//  - game states: main menu, playing, paused, game over
//  - main menu: Play, Graphics, Controls, Quit
//  - pause menu: Resume, Restart, Quit to menu
//  - graphics screen: resolution and window mode
//  - controls screen (uses input + settings)
//  - the HUD: health, XP, timer


#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// The different screens in the game.
enum class GameState
{
    Menu,       // Main menu
    Playing,    // Game is running
    Paused,     // Pause screen
    Controls,   // Controls screen
    Graphics,   // Graphics screen
    GameOver    // Game over screen
};

// Stores the score and kills from one completed run.
struct RunScore
{
    int score = 0;  // Score from this run
    int kills = 0;  // Enemies killed in this run
};

// The menu class.
struct Menu
{
    // Current screen.
    static GameState state;

    // Graphics settings.
    static int resolution;
    static bool fullscreen;

    // Highest score from all runs.
    static int high_score;

    // Stores the latest 3 runs.
    static std::vector<RunScore> recent_runs;

    // Loads the scores from the save file.
    static void load_scores();

    // Saves a completed run.
    static void record_run(int score, int kills);

    // Draws the menu.
    static void draw(sf::RenderWindow& window);
};