#include <cmath>
#include "relic.hpp"
#include "game_parameters.hpp"
#include "sprites.hpp"
using param = Parameters;

Relic::Relic(const RelicType &type, const sf::Vector2f &position) : sf::CircleShape(param::player_radius, static_cast<int>(type) + 3), _type(type){
    setPosition(position);
    setOrigin(param::player_radius, param::player_radius);
}
RelicType Relic::get_type() const {
    return _type;
}

std::string Relic::get_name() const {
    
    switch (_type) {
        case RelicType::damage:
            return "Damage Relic";
        case RelicType::cooldown:
            return "Cooldown Relic";
        default:
            return "Speed Relic";
        
    }
}
//Which picture on the spritesheet each relic uses
static Icon icon_for(const RelicType &type) {
  switch (type) {
    case RelicType::damage:
      return Icon::orb;
    case RelicType::cooldown:
      return Icon::watch;
    default:
      return Icon::boots;
  }
}

void Relic::render(sf::RenderWindow &window) const {
  Sprites::draw(window, icon_for(_type), getPosition(), param::relic_draw_size);
}

void Relic::draw_icon(sf::RenderWindow &window, const sf::Vector2f &position, const float &size) const {
  Sprites::draw(window, icon_for(_type), position + sf::Vector2f(size / 2.f, size / 2.f), size);
}
bool Relic::touching(const sf::Vector2f &position, const float &radius) const {
  sf::Vector2f offset = position - getPosition();
  float distance = std::sqrt(offset.x * offset.x + offset.y * offset.y);
  return distance <= param::relic_radius + radius;
}

void Relic::apply(Player &player) const {
  switch (_type) {
    case RelicType::damage:
      player.damage_multiplier += param::relic_damage_bonus;
      break;
    case RelicType::cooldown:
      player.cooldown_multiplier -= param::relic_cooldown_bonus;
      break;
    default:
      player.speed_multiplier += param::relic_speed_bonus;
      break;
  }
}