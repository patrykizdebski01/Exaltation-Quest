#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "weapon.hpp"
#include "relic.hpp"

struct GameSystem{
  static Player player;
  static std::vector<std::shared_ptr<Weapon>> weapons;
  static sf::View camera;
  static sf::VertexArray grid;
  //Relics still lying on the map
  static std::vector<Relic> relics;
  //Relics the player has picked up, shown in the HUD
  static std::vector<Relic> collected_relics;
  //Seconds since the run started
  static float elapsed;

  static void init();
  static void reset();
  static void clean();
  static void update(const float &dt);
  static void render(sf::RenderWindow &window);
  
protected:
  //Puts one of each relic in a random place on the map
  static void place_relics();
};
