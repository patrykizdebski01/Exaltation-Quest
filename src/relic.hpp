#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "player.hpp"

enum class RelicType{damage, cooldown, speed};

class Relic : public sf::CircleShape{
public:
    Relic(const RelicType &type, const sf::Vector2f &position);
    RelicType get_type() const;
    std::string get_name() const;
    //Letters as placeholders for hud sprite
    std::string get_short_name() const;
    //Is the player touching the relic?
    bool touching(const sf::Vector2f &position, const float &radius) const;
    //Applies the relic's effect to the player
    void apply (Player &player) const;

protected:
    RelicType _type;
};