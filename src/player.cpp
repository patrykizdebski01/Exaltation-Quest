#include <cmath>
#include "player.hpp"
#include "game_parameters.hpp"

using param = Parameters;

Player::Player() : sf::CircleShape(param::player_radius) {
  //origin in the middle, so the position is the centre of the dot
  setOrigin(param::player_radius, param::player_radius);

  max_health = param::player_health;
  health = max_health;
  level = 1;
  xp = 0.f;
  xp_to_next = param::xp_first_level;
  gold = 0;
  kills = 0;
  score = 0;
}
void Player::gain_xp(const float &amount) {
  xp += amount;
}

void Player::level_up() {
  xp -= xp_to_next;
  level++;
  xp_to_next *= param::xp_growth;
}

void Player::update(const float &dt) {
  sf::Vector2f direction(0.f, 0.f);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    direction.y -= 1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    direction.y += 1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    direction.x -= 1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    direction.x += 1.f;

  //Normalise, so moving diagonally is not faster
  float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
  if (length > 0.f)
    direction = sf::Vector2f(direction.x / length, direction.y / length);

  move(direction * param::player_speed * dt);

  //Keep the player inside the map
  sf::Vector2f position = getPosition();
  if (position.x < param::player_radius)
    position.x = param::player_radius;
  if (position.x > param::map_width - param::player_radius)
    position.x = param::map_width - param::player_radius;
  if (position.y < param::player_radius)
    position.y = param::player_radius;
  if (position.y > param::map_height - param::player_radius)
    position.y = param::map_height - param::player_radius;
  setPosition(position);
}
