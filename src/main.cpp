
#include <SFML/Graphics.hpp>
#include "game_parameters.hpp"
#include "game_system.hpp"
#include "menu.hpp"

using gs = GameSystem;
using par = Parameters;

//Graphics
void applyGraphics(sf::RenderWindow& window)
{
    const sf::VideoMode resolutions[] = {
        sf::VideoMode(1280, 720),
        sf::VideoMode(1600, 900),
        sf::VideoMode(1920, 1080)
    };

    if (Menu::fullscreen)
    {
        window.create(
            sf::VideoMode::getDesktopMode(),
            "Survivor",
            sf::Style::Fullscreen
        );
    }
    else
    {
        window.create(
            resolutions[Menu::resolution],
            "Survivor",
            sf::Style::Default
        );
    }

    window.setVerticalSyncEnabled(true);
}

int main() {

    //game window
    sf::RenderWindow window(
        sf::VideoMode(par::game_width, par::game_height),
        "Survivor"
    );

    window.setVerticalSyncEnabled(true);


    gs::init();

    sf::Clock clock;

    //main game loop
    while (window.isOpen()) {

        float dt = clock.restart().asSeconds();

        sf::Event event;

        
        while (window.pollEvent(event)) {

            
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {

                //MAIN MENU
                if (Menu::state == GameState::Menu) {

                    if (event.key.code == sf::Keyboard::Num1) {
                        gs::init();
                        Menu::state = GameState::Playing;
                    }

                    else if (event.key.code == sf::Keyboard::Num2) {
                        Menu::state = GameState::Controls;
                    }

                    else if (event.key.code == sf::Keyboard::Num3) {
                        Menu::state = GameState::Graphics;
                    }

                    else if (event.key.code == sf::Keyboard::Num4) {
                        window.close();
                    }
                }

                //PAUSE MENU
                else if (Menu::state == GameState::Paused) {

                    if (event.key.code == sf::Keyboard::Escape) {
                        Menu::state = GameState::Playing;
                    }

                    else if (event.key.code == sf::Keyboard::R) {
                        gs::init();
                        Menu::state = GameState::Playing;
                    }

                    else if (event.key.code == sf::Keyboard::M) {
                        Menu::state = GameState::Menu;
                    }
                }

                //CONTROLS SCREEN
                else if (Menu::state == GameState::Controls) {

                    if (event.key.code == sf::Keyboard::Escape) {
                        Menu::state = GameState::Menu;
                    }
                }

                //GRAPHICS SCREEN
                else if (Menu::state == GameState::Graphics)
                {
                    if (event.key.code == sf::Keyboard::Num1)
                    {
                        Menu::resolution++;

                        if (Menu::resolution > 2)
                        {
                            Menu::resolution = 0;
                        }

                        applyGraphics(window);
                    }

                    else if (event.key.code == sf::Keyboard::Num2)
                    {
                        Menu::fullscreen = !Menu::fullscreen;

                        applyGraphics(window);
                    }

                    else if (event.key.code == sf::Keyboard::Escape)
                    {
                        Menu::state = GameState::Menu;
                    }
                }


                //GAME OVER SCREEN
                else if (Menu::state == GameState::GameOver) {

                    if (event.key.code == sf::Keyboard::R) {
                        gs::init();
                        Menu::state = GameState::Playing;
                    }

                    else if (event.key.code == sf::Keyboard::M) {
                        Menu::state = GameState::Menu;
                    }
                }

                
                else if (Menu::state == GameState::Playing) {

                    if (event.key.code == sf::Keyboard::Escape) {
                        Menu::state = GameState::Paused;
                    }
                }
            }
        }

        //Update the game while playing
        if (Menu::state == GameState::Playing) {
            gs::update(dt);
        }

        //Draw everything
        window.clear();

        if (Menu::state == GameState::Playing) {
            gs::render(window);
        }
        else {
            Menu::draw(window);
        }

        window.display();
    }

    //Clean up
    gs::clean();

    return 0;
}
