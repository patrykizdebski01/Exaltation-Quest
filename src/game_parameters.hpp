#pragma once

struct Parameters{
  static constexpr int game_width = 1280;     // window size = what the camera can see
  static constexpr int game_height = 720;
  static constexpr int map_screens = 9;       // the map is 9 x 9 screens big
  static constexpr int map_width = game_width * map_screens;
  static constexpr int map_height = game_height * map_screens;
  static constexpr float player_radius = 10.f;
  static constexpr float player_speed = 300.f;
};
