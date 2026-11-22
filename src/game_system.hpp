#pragma once
#include <SFML/Graphics.hpp>
#include "player.hpp"

struct GameSystem{
  static Player player;
  static sf::View camera;
  static sf::VertexArray grid;

  static void init();
  static void reset();
  static void clean();
  static void update(const float &dt);
  static void render(sf::RenderWindow &window);
};
