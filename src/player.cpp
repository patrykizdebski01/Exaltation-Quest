#include <cmath>
#include "player.hpp"
#include "game_parameters.hpp"

using param = Parameters;

Player::Player() : sf::CircleShape(param::player_radius) {
  //origin in the middle, so the position is the centre of the dot
  setOrigin(param::player_radius, param::player_radius);
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
