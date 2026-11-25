#pragma once
#include <SFML/Graphics.hpp>

//The heads-up display, drawn on top of the game:
//  top left:  weapon slots (icons), relic slots, XP bar and level
//  top right: gold, kills and score
//  under the player: a small health bar
//  screen edges: arrows pointing at points of interest that are off screen (still to add)
struct Hud{
  static sf::Font font;
  static bool font_loaded;

  static void init();
  //Call this while the camera view is still set (the health bar is drawn in the world).
  static void render(sf::RenderWindow &window);

  static void draw_box(sf::RenderWindow &window, float x, float y, float width, float height);
  //A box with part of it filled in. fraction goes from 0 (empty) to 1 (full).
  static void draw_bar(sf::RenderWindow &window, float x, float y, float width, float height, float fraction);
  static void draw_text(sf::RenderWindow &window, const std::string &text, float x, float y, unsigned size, bool centred);
  //Arrows on the edge of the screen pointing at relics that are off screen
  static void draw_arrows(sf::RenderWindow &window);
};
