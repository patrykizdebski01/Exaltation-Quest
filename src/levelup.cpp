#include <algorithm>
#include <random>
#include "levelup.hpp"
#include "hud.hpp"
#include "game_system.hpp"
#include "game_parameters.hpp"

using param = Parameters;
using gs = GameSystem;

bool LevelUp::shown = false;
std::vector<LevelUpChoice> LevelUp::choices;
bool LevelUp::key_was_down[3] = {false, false, false};

//How many different weapons there are in make_weapon. Add one here when you make a new weapon.
static const int weapon_count = 3;

std::shared_ptr<Weapon> LevelUp::make_weapon(const int &id) {
  switch (id) {
    case 0:
      return std::make_shared<Greatsword>();
    case 1:
      return std::make_shared<FireAura>();
    default:
      return std::make_shared<ThrowingAxes>();
  }
}

void LevelUp::show() {
  choices.clear();

  //Every weapon the player has that is not at the top level can be upgraded
  for (const std::shared_ptr<Weapon> &weapon : gs::weapons) {
    if (weapon->get_level() < param::weapon_max_level) {
      LevelUpChoice choice;
      choice.weapon = weapon;
      choice.is_new = false;
      choices.push_back(choice);
    }
  }

  //Every weapon the player does not have yet can be picked up, if there is a free slot
  if (static_cast<int>(gs::weapons.size()) < param::weapon_slots) {
    for (int id = 0; id < weapon_count; ++id) {
      std::shared_ptr<Weapon> candidate = make_weapon(id);
      bool owned = false;
      for (const std::shared_ptr<Weapon> &weapon : gs::weapons) {
        if (weapon->get_name() == candidate->get_name())
          owned = true;
      }
      if (!owned) {
        LevelUpChoice choice;
        choice.weapon = candidate;
        choice.is_new = true;
        choices.push_back(choice);
      }
    }
  }

  //Nothing to offer (everything is maxed out), so carry on playing
  if (choices.empty())
    return;

  //Shuffle and keep the first few
  static std::mt19937 engine((std::random_device())());
  std::shuffle(choices.begin(), choices.end(), engine);
  if (static_cast<int>(choices.size()) > param::levelup_choices)
    choices.erase(choices.begin() + param::levelup_choices, choices.end());

  //If the player is already holding a key, it must be let go and pressed again to count
  for (int i = 0; i < 3; ++i)
    key_was_down[i] = true;
  shown = true;
}

void LevelUp::update(const float &dt) {
  (void)dt;   // nothing moves while the screen is open
  for (int i = 0; i < 3; ++i) {
    bool down = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(sf::Keyboard::Num1 + i)) ||
                sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(sf::Keyboard::Numpad1 + i));
    if (down && !key_was_down[i] && i < static_cast<int>(choices.size())) {
      choose(i);
      return;
    }
    key_was_down[i] = down;
  }
}

void LevelUp::choose(const int &index) {
  LevelUpChoice choice = choices[index];
  if (choice.is_new)
    gs::weapons.push_back(choice.weapon);
  else
    choice.weapon->upgrade();
  choices.clear();
  shown = false;
}

void LevelUp::render(sf::RenderWindow &window) {
  const float screen_width = param::game_width;
  const float screen_height = param::game_height;

  //Darken the game behind the screen
  sf::RectangleShape dim(sf::Vector2f(screen_width, screen_height));
  dim.setFillColor(sf::Color(0, 0, 0, 170));
  window.draw(dim);

  Hud::draw_text(window, "LEVEL UP!", screen_width / 2.f, 120.f, 48, true);

  //One box per choice, in a row in the middle of the screen
  const float box_width = 340.f;
  const float box_height = 300.f;
  const float gap = 40.f;
  const int count = static_cast<int>(choices.size());
  const float row_width = count * box_width + (count - 1) * gap;
  const float start_x = (screen_width - row_width) / 2.f;
  const float box_y = 230.f;
  const float icon_size = 96.f;   // a local copy, because draw_icon takes a reference

    for (int i = 0; i < count; ++i) {
    const LevelUpChoice &choice = choices[i];
    const float x = start_x + i * (box_width + gap);
    const float middle = x + box_width / 2.f;

    Hud::draw_box(window, x, box_y, box_width, box_height);
    Hud::draw_text(window, std::to_string(i + 1), x + 14.f, box_y + 8.f, 24, false);
    choice.weapon->draw_icon(window, sf::Vector2f(middle - icon_size / 2.f, box_y + 36.f), icon_size);
    Hud::draw_text(window, choice.weapon->get_name(), middle, box_y + 140.f, 30, true);

    if (choice.is_new) {
      Hud::draw_text(window, "NEW WEAPON", middle, box_y + 190.f, 22, true);
    } else {
      Hud::draw_text(window, "Level " + std::to_string(choice.weapon->get_level()) + " -> " +
                             std::to_string(choice.weapon->get_level() + 1), middle, box_y + 190.f, 22, true);
      Hud::draw_text(window, choice.weapon->get_upgrade_text(), middle, box_y + 232.f, 16, true);
    }
  }

  Hud::draw_text(window, "Press 1, 2 or 3 to choose", screen_width / 2.f, box_y + box_height + 30.f, 20, true);
}

void LevelUp::clean() {
  shown = false;
  choices.clear();
}