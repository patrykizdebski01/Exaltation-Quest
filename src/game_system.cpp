#include "game_system.hpp"
#include "game_parameters.hpp"
#include "hud.hpp"
#include "sprites.hpp"
#include "levelup.hpp"
#include <cmath>
#include <random>
#include <iostream>
#include "menu.hpp"

using param = Parameters;

Player GameSystem::player;
std::vector<std::shared_ptr<Weapon>> GameSystem::weapons;
std::vector<Relic> GameSystem::relics;
std::vector<Relic> GameSystem::collected_relics;

sf::View GameSystem::camera;
sf::VertexArray GameSystem::grid;
sf::Texture GameSystem::grass;
sf::RectangleShape GameSystem::ground;
bool GameSystem::grass_loaded = false;
float GameSystem::elapsed = 0.f;

static float distance_between(const sf::Vector2f &a, const sf::Vector2f &b) {
  sf::Vector2f offset = a - b;
  return std::sqrt(offset.x * offset.x + offset.y * offset.y);
}

//One of each relic, somewhere random on the map. Not too near the edge, not near where the player
//starts, and not near each other, so the player has to travel to find them.
void GameSystem::place_relics() {
  relics.clear();
  collected_relics.clear();

  static std::mt19937 engine((std::random_device())());
  std::uniform_real_distribution<float> random_x(param::relic_edge_margin, param::map_width - param::relic_edge_margin);
  std::uniform_real_distribution<float> random_y(param::relic_edge_margin, param::map_height - param::relic_edge_margin);
  const sf::Vector2f start(param::map_width / 2.f, param::map_height / 2.f);

  for (int type = 0; type < 3; ++type) {
    sf::Vector2f position = start;
    //Keep picking random places until one is far enough from everything (gives up after 1000 tries)
    for (int attempt = 0; attempt < 1000; ++attempt) {
      position = sf::Vector2f(random_x(engine), random_y(engine));
      bool good = distance_between(position, start) >= param::relic_min_distance_from_start;
      for (const Relic &other : relics) {
        if (distance_between(position, other.getPosition()) < param::relic_min_distance_between)
          good = false;
      }
      if (good)
        break;
    }
    relics.push_back(Relic(static_cast<RelicType>(type), position));
  }
}
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
  Sprites::init();
  
  //One rectangle the size of the whole map, covered in grass. The texture rectangle is bigger than
  //the picture, so SFML repeats the 32 x 32 grass tile over and over to fill it.
  grass_loaded = grass.loadFromFile("resources/images/grass.png");
  if (!grass_loaded) {
    std::cerr << "Failed to load resources/images/grass.png!" << std::endl;
  } else {
    grass.setRepeated(true);
    ground.setSize(sf::Vector2f(param::map_width, param::map_height));
    ground.setTexture(&grass);
    ground.setTextureRect(sf::IntRect(0, 0, static_cast<int>(param::map_width / param::grass_scale),
                                            static_cast<int>(param::map_height / param::grass_scale)));
  }
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
  place_relics();

  
}

void GameSystem::clean() {
  for (std::shared_ptr<Weapon> &weapon : weapons)
    weapon.reset();
  weapons.clear();
  grid.clear();
  relics.clear();
  collected_relics.clear();
}

void GameSystem::update(const float &dt) {

    // Check if the player has died.
    if (player.health <= 0.f)
    {
        // Save the score and kills from this run.
        Menu::record_run(player.score, player.kills);

        // Return to the main menu.
        Menu::state = GameState::Menu;

        // Stop updating the game.
        return;
    }
  //The game is paused while the level-up screen is open
  if (LevelUp::shown) {
    LevelUp::update(dt);
    return;
  }
  elapsed += dt;
  player.update(dt);
  for (std::shared_ptr<Weapon> &weapon : weapons)
    weapon->update(dt);
    //Pick up any relic the player walks into
  const float player_radius = param::player_radius;   // a local copy, because touching takes a reference
  for (std::size_t i = 0; i < relics.size();) {
    if (relics[i].touching(player.getPosition(), player_radius)) {
      relics[i].apply(player);
      collected_relics.push_back(relics[i]);
      relics.erase(relics.begin() + i);
    } else {
      ++i;
    }
  }
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
  if (grass_loaded)
    window.draw(ground);
  for (const Relic &relic : relics)
    relic.render(window);
  for (const std::shared_ptr<Weapon> &weapon : weapons)
      weapon->render(window);
  //The player's circle is only used for its position and collisions. The knight sprite is drawn instead.
  Sprites::draw_entity(window, Entity::knight, player.getPosition(), param::player_draw_size, player.facing_left);  
  Hud::render(window);
  if (LevelUp::shown)
    LevelUp::render(window);
}
