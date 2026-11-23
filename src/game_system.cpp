#include "game_system.hpp"
#include "game_parameters.hpp"
#include "hud.hpp"

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
  weapons.clear();
  weapons.push_back(std::make_shared<Greatsword>());

  
}

void GameSystem::clean() {
  for (std::shared_ptr<Weapon> &weapon : weapons)
    weapon.reset();
  weapons.clear();
  grid.clear();
}

void GameSystem::update(const float &dt) {
  elapsed += dt;
  player.update(dt);
  for (std::shared_ptr<Weapon> &weapon : weapons)
    weapon->update(dt);
  //The camera follows the player
  camera.setCenter(player.getPosition());
}

void GameSystem::render(sf::RenderWindow &window) {
  window.setView(camera);
  window.draw(grid);
  for (const std::shared_ptr<Weapon> &weapon : weapons)
    window.draw(*(weapon.get()));
  window.draw(player);
  Hud::render(window);
}
