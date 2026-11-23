#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "weapon.hpp"

struct GameSystem{
  static Player player;
  static std::vector<std::shared_ptr<Weapon>> weapons;
  static sf::View camera;
  static sf::VertexArray grid;
  //Seconds since the run started
  static float elapsed;

  static void init();
  static void reset();
  static void clean();
  static void update(const float &dt);
  static void render(sf::RenderWindow &window);
};
