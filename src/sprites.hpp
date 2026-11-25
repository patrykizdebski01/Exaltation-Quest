#pragma once
#include <SFML/Graphics.hpp>

//Every picture on the items spritesheet, numbered left to right, then top to bottom
enum class Icon { boots, book, sword, orb, watch, axe };

//Every picture on the entities spritesheet (the characters)
enum class Entity { skeleton, knight, red_skeleton };

struct Sprites{
  static sf::Texture texture;
  static sf::Texture entities_texture;
  static bool loaded;
  static bool entities_loaded;

  static void init();
  //Draws one picture of the items spritesheet, centred on a point, as a square of size x size pixels,
  //turned by angle degrees. Does nothing if the spritesheet did not load.
  static void draw(sf::RenderWindow &window, const Icon &icon, sf::Vector2f centre, float size, float angle = 0.f);
  //Draws one character, centred on a point. They face right, so facing_left flips the picture.
  static void draw_entity(sf::RenderWindow &window, const Entity &entity, sf::Vector2f centre, float size, bool facing_left = false);

protected:
  //Draws square number "index" (counting left to right, then top to bottom) of any spritesheet
  static void draw_cell(sf::RenderWindow &window, const sf::Texture &sheet, int index,
                        sf::Vector2f centre, float size, float angle, bool flipped);
};