#include <SFML/Graphics.hpp>
#include "game_parameters.hpp"
#include "game_system.hpp"

using gs = GameSystem;
using param = Parameters;

int main(){
  sf::RenderWindow window(sf::VideoMode({param::game_width, param::game_height}), "Survivor");
  window.setVerticalSyncEnabled(true);

  gs::init();
  sf::Clock clock;
  while(window.isOpen()){
    float dt = clock.restart().asSeconds();
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
        return 0;
      }
    }
    window.clear();
    gs::update(dt);
    gs::render(window);
    window.display();
  }
  gs::clean();
  return 0;
}
