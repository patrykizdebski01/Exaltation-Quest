#include <iostream>
#include "sprites.hpp"
#include "game_parameters.hpp"

using param = Parameters;

sf::Texture Sprites::texture;
bool Sprites::loaded = false;

void Sprites::init() {
  loaded = texture.loadFromFile("resources/images/items.png");
  if (!loaded) {
    std::cerr << "Failed to load resources/images/items.png!" << std::endl;
  }
}

void Sprites::draw(sf::RenderWindow &window, const Icon &icon, sf::Vector2f centre, float size, float angle) {
  if (!loaded)
    return;

  //Work out which 32 x 32 square of the spritesheet this picture is
  const int index = static_cast<int>(icon);
  const int cell = static_cast<int>(param::sprite_size);
  const int column = index % param::sprites_per_row;
  const int row = index / param::sprites_per_row;

  sf::Sprite sprite(texture, sf::IntRect(column * cell, row * cell, cell, cell));
  //Origin in the middle, so it turns round its middle and sits centred on the point
  sprite.setOrigin(param::sprite_size / 2.f, param::sprite_size / 2.f);
  sprite.setScale(size / param::sprite_size, size / param::sprite_size);
  sprite.setPosition(centre);
  sprite.setRotation(angle);
  window.draw(sprite);
}