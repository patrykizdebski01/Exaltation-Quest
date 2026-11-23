#pragma once
#include <SFML/Graphics.hpp>

class Weapon : public sf::ConvexShape {
public:
  Weapon();
  //Pure virtual deconstructor -- makes this an abstract class, like Ship
  virtual ~Weapon() = 0;
  //Counts the cooldown down. Virtual so each weapon can add its own behaviour.
  virtual void update(const float &dt);

protected:
  //Seconds until the weapon can attack again
  float _cooldown = 0.f;
};

//The warrior's weapon. It swings in a half circle round the player, alternating right, left, right...
class Greatsword : public Weapon {
public:
  Greatsword();
  void update(const float &dt) override;

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
