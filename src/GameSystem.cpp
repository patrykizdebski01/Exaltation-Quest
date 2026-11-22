#include "GameSystem.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>

static const float kWidth = 1280.f, kHeight = 720.f;    // the game is always laid out at 1280x720

struct Resolution { unsigned width, height; };
static const Resolution kResolutions[] = {{1280, 720}, {1600, 900}, {1920, 1080}, {2560, 1440}};
static const int kResolutionCount = 4;

static int currentResolution(const Settings& s) {
    for (int i = 0; i < kResolutionCount; ++i)
        if (kResolutions[i].width == s.width && kResolutions[i].height == s.height) return i;
    return 2;
}

// ============================================================ setup and main loop

GameSystem::GameSystem()
    : settingsFile_(settingsPath()),
      input_(settings_),
      screenView_(sf::FloatRect(0.f, 0.f, kWidth, kHeight)) {
    loadSettings(settings_, settingsFile_);          // no file yet = keep the defaults

    // Put a font at assets/font.ttf (or it falls back to Arial on Windows).
    const char* fonts[] = {"assets/font.ttf", "C:/Windows/Fonts/arial.ttf", "C:/Windows/Fonts/segoeui.ttf"};
    for (const char* path : fonts)
        if (!fontLoaded_) fontLoaded_ = font_.loadFromFile(path);
    if (!fontLoaded_) std::puts("No font found - put one at assets/font.ttf");
}

void GameSystem::createWindow() {
    sf::VideoMode mode(settings_.width, settings_.height);
    unsigned style = sf::Style::Titlebar | sf::Style::Close;
    if (settings_.fullscreen) {
        style = sf::Style::Fullscreen;
        if (!mode.isValid()) mode = sf::VideoMode::getDesktopMode();
    }
    window_.create(mode, "Exaltation Quest (working title)", style);
    window_.setVerticalSyncEnabled(true);            // 60 fps on a 60 Hz monitor
}

int GameSystem::run() {
    createWindow();
    sf::Clock clock;

    while (window_.isOpen()) {
        sf::Event event;
        while (window_.pollEvent(event)) input_.handleEvent(event);
        input_.update();

        float dt = std::min(clock.restart().asSeconds(), 0.05f);   // never take a huge step after a lag spike

        handleInput();
        if (state_ == State::Playing) {
            sf::Vector2f move = window_.hasFocus() ? input_.movement() : sf::Vector2f(0.f, 0.f);
            world_.update(dt, move);
        }

        draw();
        input_.endFrame();
    }

    saveSettings(settings_, settingsFile_);
    return 0;
}

// ============================================================ screens and menus

void GameSystem::handleInput() {
    if (input_.closeRequested()) {
        window_.close();
        return;
    }
    if (input_.rebinding()) return;                  // waiting for the player to press a key

    if (state_ == State::Playing) {
        if (input_.back()) {
            state_ = State::Paused;
            selected_ = 0;
        }
        return;
    }

    int count = static_cast<int>(menuLabels().size());
    if (input_.up()) selected_ = (selected_ + count - 1) % count;
    if (input_.down()) selected_ = (selected_ + 1) % count;
    if (state_ == State::SettingsMenu) {
        if (input_.left()) changeSetting(selected_, -1);
        if (input_.right()) changeSetting(selected_, +1);
    }
    if (input_.confirm()) activate(selected_);
    else if (input_.back()) goBack();
}

void GameSystem::startGame() {
    world_ = World();                                // fresh game: player back at the start
    state_ = State::Playing;
    selected_ = 0;
}

void GameSystem::goBack() {
    if (state_ == State::SettingsMenu || state_ == State::ControlsMenu) {
        saveSettings(settings_, settingsFile_);
        state_ = previousState_;
        selected_ = 0;
    } else if (state_ == State::Paused) {
        state_ = State::Playing;
    }
}

std::vector<std::string> GameSystem::menuLabels() const {
    std::vector<std::string> labels;
    if (state_ == State::MainMenu) {
        labels = {"Play", "Graphics", "Controls", "Quit"};
    } else if (state_ == State::Paused) {
        labels = {"Resume", "Restart", "Graphics", "Controls", "Quit to menu"};
    } else if (state_ == State::SettingsMenu) {
        labels.push_back("Resolution:  " + std::to_string(settings_.width) + " x " + std::to_string(settings_.height));
        labels.push_back(std::string("Window mode:  ") + (settings_.fullscreen ? "Fullscreen" : "Windowed"));
        labels.push_back("Back");
    } else if (state_ == State::ControlsMenu) {
        for (int i = 0; i < ActionCount; ++i) {
            std::string row = std::string(actionName(static_cast<Action>(i))) + ":   ";
            if (input_.rebinding() && selected_ == i) row += "press a key or button...  (Esc cancels)";
            else row += Input::keyName(settings_.key[i]) + "   /   pad " + Input::padName(settings_.pad[i]);
            labels.push_back(row);
        }
        labels.push_back("Back");
    }
    return labels;
}

void GameSystem::activate(int item) {
    if (state_ == State::MainMenu) {
        if (item == 0) startGame();
        else if (item == 1) { previousState_ = state_; state_ = State::SettingsMenu; selected_ = 0; }
        else if (item == 2) { previousState_ = state_; state_ = State::ControlsMenu; selected_ = 0; }
        else window_.close();
    } else if (state_ == State::Paused) {
        if (item == 0) goBack();
        else if (item == 1) startGame();
        else if (item == 2) { previousState_ = state_; state_ = State::SettingsMenu; selected_ = 0; }
        else if (item == 3) { previousState_ = state_; state_ = State::ControlsMenu; selected_ = 0; }
        else { state_ = State::MainMenu; selected_ = 0; }
    } else if (state_ == State::SettingsMenu) {
        if (item <= 1) changeSetting(item, +1);
        else goBack();
    } else if (state_ == State::ControlsMenu) {
        if (item < ActionCount) input_.beginRebind(static_cast<Action>(item));
        else goBack();
    }
}

void GameSystem::changeSetting(int item, int step) {
    if (item == 0) {
        int next = (currentResolution(settings_) + step + kResolutionCount) % kResolutionCount;
        settings_.width = kResolutions[next].width;
        settings_.height = kResolutions[next].height;
        createWindow();
    } else if (item == 1) {
        settings_.fullscreen = !settings_.fullscreen;
        createWindow();
    }
}

// ============================================================ drawing

void GameSystem::drawText(const std::string& text, float x, float y, unsigned size, sf::Color color, bool centered) {
    if (!fontLoaded_) return;
    sf::Text label(text, font_, size);
    label.setFillColor(color);
    if (centered) label.setOrigin(label.getLocalBounds().width / 2.f, 0.f);
    label.setPosition(x, y);
    window_.draw(label);
}

void GameSystem::draw() {
    window_.clear(sf::Color(24, 28, 36));
    window_.setView(screenView_);

    if (state_ == State::Playing || state_ == State::Paused) {
        drawWorld();
        window_.setView(screenView_);
    }
    drawMenu();
    window_.display();
}

void GameSystem::drawWorld() {
    const Player& player = world_.player;
    window_.setView(sf::View(player.pos, sf::Vector2f(kWidth, kHeight)));   // camera follows the player

    // floor grid so you can see yourself moving
    sf::VertexArray grid(sf::Lines);
    sf::Color gridColor(255, 255, 255, 25);
    float left = player.pos.x - kWidth / 2.f, right = player.pos.x + kWidth / 2.f;
    float top = player.pos.y - kHeight / 2.f, bottom = player.pos.y + kHeight / 2.f;
    for (float x = std::floor(left / 64.f) * 64.f; x <= right; x += 64.f) {
        grid.append(sf::Vertex(sf::Vector2f(x, top), gridColor));
        grid.append(sf::Vertex(sf::Vector2f(x, bottom), gridColor));
    }
    for (float y = std::floor(top / 64.f) * 64.f; y <= bottom; y += 64.f) {
        grid.append(sf::Vertex(sf::Vector2f(left, y), gridColor));
        grid.append(sf::Vertex(sf::Vector2f(right, y), gridColor));
    }
    window_.draw(grid);

    sf::CircleShape me(player.radius);
    me.setOrigin(player.radius, player.radius);
    me.setPosition(player.pos);
    me.setFillColor(sf::Color(60, 120, 255));
    me.setOutlineThickness(2.f);
    me.setOutlineColor(sf::Color::White);
    window_.draw(me);
}

void GameSystem::drawMenu() {
    if (state_ == State::Playing) return;

    if (state_ == State::Paused) {                   // darken the game behind the pause menu
        sf::RectangleShape dim(sf::Vector2f(kWidth, kHeight));
        dim.setFillColor(sf::Color(0, 0, 0, 150));
        window_.draw(dim);
    }

    std::string title;
    if (state_ == State::MainMenu) title = "EXALTATION QUEST";
    else if (state_ == State::SettingsMenu) title = "Graphics";
    else if (state_ == State::ControlsMenu) title = "Controls";
    else title = "Paused";
    drawText(title, kWidth / 2.f, 90.f, 56, sf::Color::White, true);

    std::vector<std::string> labels = menuLabels();
    for (int i = 0; i < static_cast<int>(labels.size()); ++i) {
        bool selected = (i == selected_);
        drawText(selected ? "> " + labels[i] + " <" : labels[i], kWidth / 2.f, 220.f + 52.f * i, 30,
                 selected ? sf::Color(255, 220, 90) : sf::Color(210, 210, 210), true);
    }

    std::string hint = "W / S to move   -   Enter, Space or pad A to select   -   Esc or pad B to go back";
    if (state_ == State::SettingsMenu) hint = "A / D changes a value   -   Esc to go back (saved automatically)";
    if (state_ == State::ControlsMenu) hint = "Select a row to rebind it   -   Esc to cancel or go back";
    drawText(hint, kWidth / 2.f, kHeight - 40.f, 16, sf::Color(150, 150, 150), true);
}
