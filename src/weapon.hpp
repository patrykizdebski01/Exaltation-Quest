#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>

class Weapon : public sf::ConvexShape {
public:
  Weapon();
  virtual ~Weapon() = 0;
  //Counts the cooldown down. Virtual so each weapon can add its own behaviour.
  virtual void update(const float &dt);
  //The name shown in the HUD
  virtual std::string get_name() const = 0;
  int get_level() const;
  //Makes the weapon stronger: more damage, and it attacks a bit faster
  virtual void upgrade();
  //Text for the level-up screen, e.g. "Damage +25%, attacks 10% faster"
  std::string get_upgrade_text() const;
  //Draws the weapon in the world. Most weapons are just their own shape, so that is the default.
  virtual void render(sf::RenderWindow &window) const;
  //Draws the weapon's icon inside a HUD slot. position = top left of the slot, size = width and height.
  virtual void draw_icon(sf::RenderWindow &window, const sf::Vector2f &position, const float &size) const = 0;

protected:
  //Seconds until the weapon can attack again
  float _cooldown = 0.f;
  int _level = 1;
  //1.0 at level 1, bigger every level
  float damage_scale() const;
  float speed_scale() const;
};

//The warrior's weapon. It swings in a half circle round the player, alternating right, left, right
class Greatsword : public Weapon {
public:
  Greatsword();
  void update(const float &dt) override;
  std::string get_name() const override;
  void draw_icon(sf::RenderWindow &window, const sf::Vector2f &position, const float &size) const override;

  bool is_swinging() const;
  float get_damage() const;
  //Is a circle (an enemy, later) touching the swing right now?
  bool hits(const sf::Vector2f &position, const float &radius) const;

protected:
  void swing();
  void build_arc(const float &sweep);
  //Seconds left in the current swing (0 = not swinging)
  float _swing_time = 0.f;
  //Side of the current swing: 1 = right, -1 = left
  float _side = -1.f;
};
//The player gets an aura that damages every enemy inside it, once every interval
class FireAura : public Weapon {
public:
  FireAura();
  void update(const float &dt) override;
  std::string get_name() const override;
  void draw_icon(sf::RenderWindow &window, const sf::Vector2f &position, const float &size) const override;

  //True for the one frame each time the aura damages. Enemies are only hurt when this is true.
  bool is_ticking() const;
  float get_damage() const;
  //Is a circle (an enemy, later) inside the aura?
  bool hits(const sf::Vector2f &position, const float &radius) const;

protected:
  bool _ticking = false;
  //Seconds left of the thicker outline after a tick
  float _flash = 0.f;
};

//One thrown axe
struct Axe {
  sf::Vector2f position;
  sf::Vector2f direction;
  float life;     // seconds left before it disappears
  float angle;    // how far it has spun
};

//Throws four axes in an X pattern (up-left, up-right, down-left, down-right) every few seconds
class ThrowingAxes : public Weapon {
public:
  ThrowingAxes();
  void update(const float &dt) override;
  //This weapon is lots of little axes, not one shape, so it draws them itself
  void render(sf::RenderWindow &window) const override;
  std::string get_name() const override;
  void draw_icon(sf::RenderWindow &window, const sf::Vector2f &position, const float &size) const override;

  float get_damage() const;
  //Is a circle (an enemy, later) touching an axe? If it is, that axe is used up and disappears.
  bool hits(const sf::Vector2f &position, const float &radius);

protected:
  void throw_axes();
  std::vector<Axe> _axes;
};
