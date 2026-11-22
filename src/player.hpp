#pragma once
#include <SFML/Graphics.hpp>

class Player : public sf::CircleShape {
public:
  Player();
  //Moves the player with W A S D and keeps it inside the map
  void update(const float &dt);
};
