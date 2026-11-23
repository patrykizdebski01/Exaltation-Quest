#pragma once
#include <SFML/Graphics.hpp>

class Player : public sf::CircleShape {
public:
  Player();
  //Moves the player with W A S D and keeps it inside the map
  void update(const float &dt);

  //Shown in the HUD
  float max_health;
  float health;
  int level;
  float xp;
  float xp_to_next;
  int gold;
  int kills;
  int score;
};
