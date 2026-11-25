#pragma once
#include <SFML/Graphics.hpp>

//Every picture on the spritesheet, numbered left to right, then top to bottom
enum class Icon { boots, book, sword, orb, watch, axe };

struct Sprites{
  static sf::Texture texture;
  static bool loaded;

  static void init();
  //Draws one picture of the spritesheet, centred on a point, as a square of size x size pixels,
  //turned by angle degrees. Does nothing if the spritesheet did not load.
  static void draw(sf::RenderWindow &window, const Icon &icon, sf::Vector2f centre, float size, float angle = 0.f);
};