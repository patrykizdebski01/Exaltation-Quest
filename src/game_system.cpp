#include "game_system.hpp"
#include "game_parameters.hpp"

using param = Parameters;

Player GameSystem::player;
sf::View GameSystem::camera;
sf::VertexArray GameSystem::grid;

void GameSystem::init() {
  //Lines round the edge of the map and round every screen-sized square,
  //so you can see yourself moving and how big the map is.
  //No colours are set, so SFML draws them white on the default black background.
  grid.setPrimitiveType(sf::Lines);
  for (int i = 0; i <= param::map_screens; ++i) {
    grid.append(sf::Vertex(sf::Vector2f(i * param::game_width, 0.f)));
    grid.append(sf::Vertex(sf::Vector2f(i * param::game_width, param::map_height)));
    grid.append(sf::Vertex(sf::Vector2f(0.f, i * param::game_height)));
    grid.append(sf::Vertex(sf::Vector2f(param::map_width, i * param::game_height)));
  }
  reset();
}

void GameSystem::reset() {
  //Start in the middle of the map
  player.setPosition(param::map_width / 2.f, param::map_height / 2.f);
  camera.setSize(sf::Vector2f(param::game_width, param::game_height));
  camera.setCenter(player.getPosition());
}

void GameSystem::clean() {
  grid.clear();
}

void GameSystem::update(const float &dt) {
  player.update(dt);
  //The camera follows the player
  camera.setCenter(player.getPosition());
}

void GameSystem::render(sf::RenderWindow &window) {
  window.setView(camera);
  window.draw(grid);
  window.draw(player);
}
