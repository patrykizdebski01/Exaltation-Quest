#include <iostream>
#include "sprites.hpp"
#include "game_parameters.hpp"

using param = Parameters;

sf::Texture Sprites::texture;
sf::Texture Sprites::entities_texture;
bool Sprites::loaded = false;
bool Sprites::entities_loaded = false;

void Sprites::init() {
  loaded = texture.loadFromFile("resources/images/items.png");
  if (!loaded) {
    std::cerr << "Failed to load resources/images/items.png!" << std::endl;
  }
  entities_loaded = entities_texture.loadFromFile("resources/images/entities.png");
  if (!entities_loaded) {
    std::cerr << "Failed to load resources/images/entities.png!" << std::endl;
  }
}

void Sprites::draw(sf::RenderWindow &window, const Icon &icon, sf::Vector2f centre, float size, float angle) {
  if (!loaded)
    return;
  draw_cell(window, texture, static_cast<int>(icon), centre, size, angle, false);
}

void Sprites::draw_entity(sf::RenderWindow &window, const Entity &entity, sf::Vector2f centre, float size, bool facing_left) {
  if (!entities_loaded)
    return;
  draw_cell(window, entities_texture, static_cast<int>(entity), centre, size, 0.f, !facing_left);
}

void Sprites::draw_cell(sf::RenderWindow &window, const sf::Texture &sheet, int index,
                        sf::Vector2f centre, float size, float angle, bool flipped) {
  //Work out which 32 x 32 square of the spritesheet this picture is
  const int cell = static_cast<int>(param::sprite_size);
  const int column = index % param::sprites_per_row;
  const int row = index / param::sprites_per_row;

  sf::Sprite sprite(sheet, sf::IntRect(column * cell, row * cell, cell, cell));
  //Origin in the middle, so it turns round its middle and sits centred on the point
  sprite.setOrigin(param::sprite_size / 2.f, param::sprite_size / 2.f);
  float scale = size / param::sprite_size;
  //A negative width flips the picture left to right
  sprite.setScale(flipped ? -scale : scale, scale);
  sprite.setPosition(centre);
  sprite.setRotation(angle);
  window.draw(sprite);
}