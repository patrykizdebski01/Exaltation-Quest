#include <cmath>
#include "weapon.hpp"
#include "game_system.hpp"
#include "game_parameters.hpp"

using param = Parameters;
using gs = GameSystem;

Weapon::Weapon() : sf::ConvexShape() {}

//Define the weapon deconstructor.
Weapon::~Weapon() = default;

void Weapon::update(const float &dt) {
  _cooldown -= dt;
}

Greatsword::Greatsword() : Weapon() {
  _cooldown = 0.3f;   // first swing shortly after the game starts
}

void Greatsword::update(const float &dt) {
  Weapon::update(dt);

  //The sword is always centred on the player
  setPosition(gs::player.getPosition());

  if (_swing_time > 0.f) {
    _swing_time -= dt;
    if (_swing_time > 0.f) {
      //0 at the start of the swing, 1 at the end
      float done = 1.f - _swing_time / param::greatsword_swing_time;
      build_arc(done);
    } else {
      //Swing finished: no points means nothing is drawn
      setPointCount(0);
    }
  }

  if (_cooldown <= 0.f) {
    swing();
  }
}

std::string Greatsword::get_name() const {
  return "Greatsword";
}

//The HUD icon: a sword made from three rectangles, tilted 45 degrees, in the middle of the slot.
void Greatsword::draw_icon(sf::RenderWindow &, const sf::Vector2f &, const float &) const {
}

void Greatsword::swing() {
  _side = -_side;                                  // swing the other way than last time
  _swing_time = param::greatsword_swing_time;
  _cooldown = param::greatsword_cooldown;
}

//Draws the swing as a half circle that opens from straight up to straight down.
//sweep goes from 0 (nothing) to 1 (the whole half circle).
void Greatsword::build_arc(const float &sweep) {
  const float pi = 3.14159265f;
  setPointCount(param::swing_points + 2);
  setPoint(0, sf::Vector2f(0.f, 0.f));
  for (int i = 0; i <= param::swing_points; ++i) {
    float angle = -pi / 2.f + pi * sweep * i / param::swing_points;
    setPoint(i + 1, sf::Vector2f(std::cos(angle) * param::greatsword_range * _side,
                                 std::sin(angle) * param::greatsword_range));
  }
}

bool Greatsword::is_swinging() const {
  return _swing_time > 0.f;
}

//TODO: the design document says the damage grows with the player's health.
//Once the player has health, scale this by (1 + health / max health).
float Greatsword::get_damage() const {
  return param::greatsword_damage;
}

//Used by enemies later: for each enemy call  sword->hits(enemy.getPosition(), enemy.radius)
bool Greatsword::hits(const sf::Vector2f &position, const float &radius) const {
  if (!is_swinging())
    return false;
  sf::Vector2f offset = position - getPosition();
  float distance = std::sqrt(offset.x * offset.x + offset.y * offset.y);
  if (distance > param::greatsword_range + radius)
    return false;
  //It has to be on the side that is being swung
  return offset.x * _side >= -radius;
}
