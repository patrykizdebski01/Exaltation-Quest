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
    //Draws the relic's sprite in the world, on the relic's position
    void render(sf::RenderWindow &window) const;
    //Draws the relic's sprite inside a HUD slot. position = top left of the slot, size = width and height.
    void draw_icon(sf::RenderWindow &window, const sf::Vector2f &position, const float &size) const;
    //Is the player touching the relic?
    bool touching(const sf::Vector2f &position, const float &radius) const;
    //Applies the relic's effect to the player
    void apply (Player &player) const;

protected:
    RelicType _type;
};