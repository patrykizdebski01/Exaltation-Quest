#include "game_system.hpp"
#include "game_parameters.hpp"
#include "hud.hpp"
#include "levelup.hpp"

using param = Parameters;

Player GameSystem::player;
std::vector<std::shared_ptr<Weapon>> GameSystem::weapons;
sf::View GameSystem::camera;
sf::VertexArray GameSystem::grid;
float GameSystem::elapsed = 0.f;

void GameSystem::init() {
  //Lines round the edge of the map and round every screen-sized square,
  //so you can see yourself moving and how big the map is.
  //No colours are set, so SFML draws them white on the default black background.
  grid.setPrimitiveType(sf::Lines);
  for (int i = 0; i <= param::map_screens; ++i) {
    grid.append(sf::Vertex(sf::Vector2f(static_cast<float>(i * param::game_width), 0.f)));
    grid.append(sf::Vertex(sf::Vector2f(static_cast<float>(i * param::game_width), param::map_height)));
    grid.append(sf::Vertex(sf::Vector2f(0.f, static_cast<float>(i * param::game_height))));
    grid.append(sf::Vertex(sf::Vector2f(param::map_width, static_cast<float>(i * param::game_height))));
  }
  Hud::init();
  reset();
}

void GameSystem::reset() {
  player = Player();
  player.setPosition(param::map_width / 2.f, param::map_height / 2.f);
  elapsed = 0.f;
  camera.setSize(sf::Vector2f(param::game_width, param::game_height));
  camera.setCenter(player.getPosition());

  //The warrior starts with the greatsword
  weapons.push_back(std::make_shared<Greatsword>());
  LevelUp::clean();

  
}

void GameSystem::clean() {
  for (std::shared_ptr<Weapon> &weapon : weapons)
    weapon.reset();
  weapons.clear();
  grid.clear();
}

void GameSystem::update(const float &dt) {
  //The game is paused while the level-up screen is open
  if (LevelUp::shown) {
    LevelUp::update(dt);
    return;
  }
  elapsed += dt;
  player.update(dt);
  for (std::shared_ptr<Weapon> &weapon : weapons)
    weapon->update(dt);
  //The camera follows the player
  camera.setCenter(player.getPosition());
  //FOR TESTING LEVEL UPS ONLY 
  static bool l_was_down = false;
  bool l_down = sf::Keyboard::isKeyPressed(sf::Keyboard::L);
  if (l_down && !l_was_down)
    player.gain_xp(player.xp_to_next);
  l_was_down = l_down;
  if (player.xp >= player.xp_to_next){
    LevelUp::show();
    player.level_up();
  }
  //FOR TESTING LEVEL UPS ONLY ^^^^


}

void GameSystem::render(sf::RenderWindow &window) {
  window.setView(camera);
  window.draw(grid);
  for (const std::shared_ptr<Weapon> &weapon : weapons)
      weapon->render(window);
  window.draw(player);
  Hud::render(window);
  if (LevelUp::shown)
    LevelUp::render(window);
}
