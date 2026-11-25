#include <cmath>
#include "weapon.hpp"
#include "game_system.hpp"
#include "game_parameters.hpp"
#include "levelup.hpp"
#include "sprites.hpp"

using param = Parameters;
using gs = GameSystem;

Weapon::Weapon() : sf::ConvexShape() {}

//Define the weapon deconstructor.
Weapon::~Weapon() = default;

void Weapon::update(const float &dt) {
  _cooldown -= dt;
}
int Weapon::get_level() const {
  return _level;
}

void Weapon::upgrade() {
  if (_level < param::weapon_max_level)
    _level++;
}

std::string Weapon::get_upgrade_text() const {
  return "Damage +" + std::to_string(static_cast<int>(param::upgrade_damage * 100.f)) +
         "%, attacks " + std::to_string(static_cast<int>(param::upgrade_speed * 100.f)) + "% faster";
}

float Weapon::damage_scale() const {
    return (1.f + param::upgrade_damage * (_level - 1)) * gs::player.damage_multiplier;
}

float Weapon::speed_scale() const {
    //Dividing by the cooldown multiplier: a relic that makes it 0.8 makes weapons 1.25 times faster
  return (1.f + param::upgrade_speed * (_level - 1)) / gs::player.cooldown_multiplier;
}
void Weapon::render(sf::RenderWindow &window) const {
  window.draw(*this);
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

void Greatsword::draw_icon(sf::RenderWindow &window, const sf::Vector2f &position, const float &size) const {
  Sprites::draw(window, Icon::sword, position + sf::Vector2f(size / 2.f, size / 2.f), size);
}

void Greatsword::swing() {
  _side = -_side;                                  // swing the other way than last time
  _swing_time = param::greatsword_swing_time;
  _cooldown = _cooldown = param::greatsword_cooldown / speed_scale();
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
  return param::greatsword_damage * damage_scale();
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
// ---------- Fire Aura ----------

FireAura::FireAura() : Weapon() {
  //A circle of points round (0, 0). The weapon is moved onto the player in update.
  const float pi = 3.14159265f;
  setPointCount(param::aura_points);
  for (int i = 0; i < param::aura_points; ++i) {
    float angle = 2.f * pi * i / param::aura_points;
    setPoint(i, sf::Vector2f(std::cos(angle) * param::fire_aura_radius,
                             std::sin(angle) * param::fire_aura_radius));
  }
  //Only the outline is drawn, so you can still see inside the aura
  setFillColor(sf::Color::Transparent);
  setOutlineThickness(2.f);
  _cooldown = param::fire_aura_interval;
}

void FireAura::update(const float &dt) {
  Weapon::update(dt);   // counts the cooldown down: here it is the time until the next tick

  //The aura is always centred on the player
  setPosition(gs::player.getPosition());

  _ticking = false;
  if (_flash > 0.f) {
    _flash -= dt;
    if (_flash <= 0.f)
      setOutlineThickness(2.f);
  }

  if (_cooldown <= 0.f) {
    _ticking = true;
    _cooldown = param::fire_aura_interval / speed_scale();
    _flash = param::fire_aura_flash;
    setOutlineThickness(6.f);   // flash thicker so you can see it hurt
  }
}

std::string FireAura::get_name() const {
  return "Fire Aura";
}

void FireAura::draw_icon(sf::RenderWindow &window, const sf::Vector2f &position, const float &size) const {
  Sprites::draw(window, Icon::book, position + sf::Vector2f(size / 2.f, size / 2.f), size);
}

bool FireAura::is_ticking() const {
  return _ticking;
}

float FireAura::get_damage() const {
  return param::fire_aura_damage* damage_scale();
}

//Used by enemies later:  if (aura->is_ticking() && aura->hits(enemy.getPosition(), enemy.radius)) ...
bool FireAura::hits(const sf::Vector2f &position, const float &radius) const {
  sf::Vector2f offset = position - getPosition();
  float distance = std::sqrt(offset.x * offset.x + offset.y * offset.y);
  return distance <= param::fire_aura_radius + radius;
}

// ---------- Throwing Axes ----------

ThrowingAxes::ThrowingAxes() : Weapon() {
  _cooldown = 0.5f;   // first throw shortly after the game starts
}

void ThrowingAxes::update(const float &dt) {
  Weapon::update(dt);

  if (_cooldown <= 0.f) {
    throw_axes();
    _cooldown = param::axe_cooldown / speed_scale();
  }

  //Move, spin and age every axe. Axes that have run out of time are removed.
  for (std::size_t i = 0; i < _axes.size();) {
    _axes[i].position += _axes[i].direction * param::axe_speed * dt;
    _axes[i].angle += param::axe_spin * dt;
    _axes[i].life -= dt;
    if (_axes[i].life <= 0.f)
      _axes.erase(_axes.begin() + i);
    else
      ++i;
  }
}

//Four axes, one in each diagonal direction, starting on the player
void ThrowingAxes::throw_axes() {
  const float diagonal = 0.70710678f;   // 1 / square root of 2, so diagonals are not faster
  const float x[4] = {-diagonal, diagonal, -diagonal, diagonal};
  const float y[4] = {-diagonal, -diagonal, diagonal, diagonal};
  for (int i = 0; i < 4; ++i) {
    Axe axe;
    axe.position = gs::player.getPosition();
    axe.direction = sf::Vector2f(x[i], y[i]);
    axe.life = param::axe_lifetime;
    axe.angle = 0.f;
    _axes.push_back(axe);
  }
}
void ThrowingAxes::draw_icon(sf::RenderWindow &window, const sf::Vector2f &position, const float &size) const {
  Sprites::draw(window, Icon::axe, position + sf::Vector2f(size / 2.f, size / 2.f), size);
}
//Each thrown axe is the axe sprite, spinning round its middle
void ThrowingAxes::render(sf::RenderWindow &window) const {
  for (const Axe &axe : _axes)
    Sprites::draw(window, Icon::axe, axe.position, param::sprite_size, axe.angle);
}

std::string ThrowingAxes::get_name() const {
  return "Throwing Axes";
}


float ThrowingAxes::get_damage() const {
  return param::axe_damage * damage_scale();
}

//Used by enemies later:  if (axes->hits(enemy.getPosition(), enemy.radius)) enemy takes axes->get_damage()
bool ThrowingAxes::hits(const sf::Vector2f &position, const float &radius) {
  for (std::size_t i = 0; i < _axes.size(); ++i) {
    sf::Vector2f offset = position - _axes[i].position;
    float distance = std::sqrt(offset.x * offset.x + offset.y * offset.y);
    if (distance <= param::axe_radius + radius) {
      _axes.erase(_axes.begin() + i);   // the axe is used up
      return true;
    }
  }
  return false;
}