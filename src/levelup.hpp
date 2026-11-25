#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "weapon.hpp"

//One option on the level-up screen
struct LevelUpChoice {
  //A weapon the player already has (upgrade it) or a brand new one (add it)
  std::shared_ptr<Weapon> weapon;
  bool is_new;
};

//The screen that appears when the player levels up. The game is paused while it is shown.
//The player picks one of up to 3 options with the keys 1, 2 and 3.
struct LevelUp{
  static bool shown;
  static std::vector<LevelUpChoice> choices;
  //Was each of the keys 1, 2, 3 down last frame? So holding a key only picks once.
  static bool key_was_down[3];

  //Works out the options and opens the screen. Does nothing if there is nothing left to offer.
  static void show();
  static void update(const float &dt);
  static void render(sf::RenderWindow &window);
  static void clean();

protected:
  static void choose(const int &index);
  //Makes a new weapon from the list of every weapon in the game
  static std::shared_ptr<Weapon> make_weapon(const int &id);
};