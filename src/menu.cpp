

#include "menu.hpp"

#include <fstream>
#include <algorithm>

// Start the game on the main menu.
GameState Menu::state = GameState::Menu;

// Default graphics settings.
int Menu::resolution = 0;
bool Menu::fullscreen = false;

// Start with no saved high score in memory.
int Menu::high_score = 0;

// Store the latest runs.
std::vector<RunScore> Menu::recent_runs;


// Load saved scores from the file.
void Menu::load_scores()
{
    // Open the score file.
    std::ifstream file("scores.txt");

    // If the file does not exist, start with empty scores.
    if (!file)
        return;

    // Read the high score first.
    file >> high_score;

    // Clear the current run history.
    recent_runs.clear();

    // Temporary score record.
    RunScore run;

    // Read each saved score and kill count.
    while (file >> run.score >> run.kills)
    {
        // Add the run to the history.
        recent_runs.push_back(run);
    }

    // Only keep the latest 3 runs.
    if (recent_runs.size() > 3)
    {
        recent_runs.resize(3);
    }
}


// Save a completed run.
void Menu::record_run(int score, int kills)
{
    // Create a record for this run.
    RunScore run;

    // Store the score.
    run.score = score;

    // Store the kills.
    run.kills = kills;

    // Add the newest run to the beginning.
    recent_runs.insert(recent_runs.begin(), run);

    // Keep only 3 runs.
    if (recent_runs.size() > 3)
    {
        recent_runs.resize(3);
    }

    // Check if this run is the new high score.
    if (score > high_score)
    {
        high_score = score;
    }

    // Open the file for saving.
    std::ofstream file("scores.txt");

    // Stop if the file cannot be opened.
    if (!file)
        return;

    // Save the high score on the first line.
    file << high_score << "\n";

    // Save each run's score and kills.
    for (const RunScore& saved : recent_runs)
    {
        file << saved.score << " "
            << saved.kills << "\n";
    }
}


// Draw the menu screen.
void Menu::draw(sf::RenderWindow& window)
{
    // Text to display on the left side.
    std::string text;

    // Main menu.
    if (state == GameState::Menu)
    {
        text = "MAIN MENU\n\n"
            "1  Play\n"
            "2  Controls\n"
            "3  Graphics\n"
            "4  Quit";
    }

    // Pause menu.
    else if (state == GameState::Paused)
    {
        text = "PAUSED\n\n"
            "ESC  Resume\n"
            "R  Restart\n"
            "M  Main Menu";
    }

    // Controls screen.
    else if (state == GameState::Controls)
    {
        text = "CONTROLS\n\n"
            "WASD - Move\n"
            "ESC - Back";
    }

    // Graphics screen.
    else if (state == GameState::Graphics)
    {
        text = "GRAPHICS\n\n"
            "1  Change Resolution\n"
            "2  Toggle Fullscreen\n"
            "ESC  Back\n\n"
            "Resolution option: " +
            std::to_string(resolution + 1) +
            "\nFullscreen: " +
            (fullscreen ? "ON" : "OFF");
    }

    // Game over screen.
    else if (state == GameState::GameOver)
    {
        text = "GAME OVER\n\n"
            "R  Restart\n"
            "M  Main Menu";
    }

    // Load the font once.
    static sf::Font font;

    // Remember whether the font loaded successfully.
    static bool font_loaded =
        font.loadFromFile("resources/fonts/arial.ttf");

    // Draw the menu if the font loaded.
    if (font_loaded)
    {
        // Create the menu text.
        sf::Text menu_text(text, font, 30);

        // Set the text colour.
        menu_text.setFillColor(sf::Color::White);

        // Put the menu on the left.
        menu_text.setPosition(100.f, 100.f);

        // Draw the menu.
        window.draw(menu_text);


        // Only show scores on the main menu.
        if (state == GameState::Menu)
        {
            // Start the score text with the high score.
            std::string stats =
                "HIGH SCORE: " +
                std::to_string(high_score) +
                "\n\nLATEST RUNS";

            // Add the latest runs.
            for (std::size_t i = 0;
                i < recent_runs.size();
                ++i)
            {
                // Add the run number, score and kills.
                stats +=
                    "\nRun " +
                    std::to_string(i + 1) +
                    ": " +
                    std::to_string(recent_runs[i].score) +
                    " score  /  " +
                    std::to_string(recent_runs[i].kills) +
                    " kills";
            }

            // Create the score text.
            sf::Text stats_text(stats, font, 22);

            // Set the text colour.
            stats_text.setFillColor(sf::Color::White);

            // Put the score history on the right.
            stats_text.setPosition(690.f, 100.f);

            // Draw the scores.
            window.draw(stats_text);
        }
    }
}