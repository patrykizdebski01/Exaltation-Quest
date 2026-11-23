#include <cmath>
#include <iostream>
#include <cstdio>
#include "hud.hpp"
#include "game_system.hpp"
#include "game_parameters.hpp"

using param = Parameters;
using gs = GameSystem;

sf::Font Hud::font;
bool Hud::font_loaded = false;

//Turns 75 seconds into "01:15"
static std::string format_time(float seconds) {
  int total = static_cast<int>(seconds);
  char text[16];
  std::snprintf(text, sizeof text, "%02d:%02d", total / 60, total % 60);
  return text;
}

void Hud::init() {
  font_loaded = font.loadFromFile("resources/fonts/font.ttf") ||
                font.loadFromFile("C:/Windows/Fonts/arial.ttf");
  if (!font_loaded) {
    std::cerr << "Failed to load a font!" << std::endl;
  }
}

void Hud::draw_box(sf::RenderWindow &window, float x, float y, float width, float height) {
  //An outline with nothing inside
  sf::RectangleShape box(sf::Vector2f(width, height));
  box.setPosition(x, y);
  box.setFillColor(sf::Color::Transparent);
  box.setOutlineThickness(1.f);
  window.draw(box);
}

void Hud::draw_bar(sf::RenderWindow &window, float x, float y, float width, float height, float fraction) {
  if (fraction < 0.f)
    fraction = 0.f;
  if (fraction > 1.f)
    fraction = 1.f;

  draw_box(window, x, y, width, height);

  //The filled part
  sf::RectangleShape filled(sf::Vector2f(width * fraction, height));
  filled.setPosition(x, y);
  window.draw(filled);
}

void Hud::draw_text(sf::RenderWindow &window, const std::string &text, float x, float y, unsigned size, bool centred) {
  if (!font_loaded)
    return;
  sf::Text label(text, font, size);
  if (centred)
    label.setOrigin(label.getLocalBounds().width / 2.f, 0.f);
  label.setPosition(x, y);
  window.draw(label);
}



void Hud::render(sf::RenderWindow &window) {
  const Player &player = gs::player;

  //Health bar, just under the player. This is drawn in the world (the camera view is still set),
  //so it stays with the player.
  draw_bar(window,
           player.getPosition().x - param::health_bar_width / 2.f,
           player.getPosition().y + param::player_radius + 8.f,
           param::health_bar_width, param::health_bar_height,
           player.health / player.max_health);

  //Everything below is drawn in screen space, so it does not move with the camera
  window.setView(window.getDefaultView());



  //Weapon slots, top left. Each equipped weapon draws its own icon in its slot.
  const float left = param::hud_margin;
  const float slot = param::slot_size; //a local copy, because draw_icon takes a reference
  const float step = slot + param::slot_gap;
  float row_y = param::hud_margin;
  for (int i = 0; i < param::weapon_slots; ++i) {
    draw_box(window, left + i * step, row_y, slot, slot);
    if (i < static_cast<int>(gs::weapons.size()))
      gs::weapons[i]->draw_icon(window, sf::Vector2f(left + i * step, row_y), slot);
  }

  //Relic slots, underneath. Empty until relics are added.
  row_y += step;
  for (int i = 0; i < param::relic_slots; ++i)
    draw_box(window, left + i * step, row_y, slot, slot);

  //XP bar under the relics, with the level next to it
  row_y += step;
  float bar_width = param::weapon_slots * step - param::slot_gap;
  draw_bar(window, left, row_y, bar_width, 12.f, player.xp / player.xp_to_next);
  draw_text(window, "Lvl. " + std::to_string(player.level), left + bar_width + 10.f, row_y - 4.f, 18, false);

  //Gold, kills and score, top right
  const float box_width = 360.f;
  const float box_x = param::game_width - param::hud_margin - box_width;
  draw_box(window, box_x, param::hud_margin, box_width, 40.f);
  draw_text(window, "Gold " + std::to_string(player.gold) + "  |  Kills " + std::to_string(player.kills) +
                    "  |  Score " + std::to_string(player.score),
            box_x + box_width / 2.f, param::hud_margin + 8.f, 20, true);

  //Timer, top middle (not in the mockup: delete this block if you do not want it)
  float shown = gs::elapsed;
  if (shown > param::run_length)
    shown = param::run_length;
  draw_text(window, format_time(shown) + " / " + format_time(param::run_length),
            param::game_width / 2.f, 14.f, 24, true);
}
