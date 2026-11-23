#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class Weapon : public sf::ConvexShape {
public:
  Weapon();
  virtual ~Weapon() = 0;
  //Counts the cooldown down. Virtual so each weapon can add its own behaviour.
  virtual void update(const float &dt);
  //The name shown in the HUD
  virtual std::string get_name() const = 0;
  //Draws the weapon's icon inside a HUD slot. position = top left of the slot, size = width and height.
  virtual void draw_icon(sf::RenderWindow &window, const sf::Vector2f &position, const float &size) const = 0;

protected:
  //Seconds until the weapon can attack again
  float _cooldown = 0.f;
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
