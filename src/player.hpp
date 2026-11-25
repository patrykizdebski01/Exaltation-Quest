#pragma once
#include <SFML/Graphics.hpp>

class Player : public sf::CircleShape {
public:
  Player();
  //Moves the player with W A S D and keeps it inside the map
  void update(const float &dt);
  //Adds XP. The level up itself is checked in GameSystem::update.
  void gain_xp(const float &amount);
  //Takes the XP for the current level and makes the next level harder
  void level_up();
  //Shown in the HUD
  float max_health;
  float health;
  int level;
  float xp;
  float xp_to_next;
  int gold;
  int kills;
  int score;
  //Changed by relics. 1.0 means no change.
  float damage_multiplier;
  float cooldown_multiplier;
  float speed_multiplier;
};
