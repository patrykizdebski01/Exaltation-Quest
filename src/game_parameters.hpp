#pragma once

struct Parameters{
  static constexpr int game_width = 1280;     // window size = what the camera can see
  static constexpr int game_height = 720;
  static constexpr int map_screens = 9;       // the map is 9 x 9 screens big
  static constexpr int map_width = game_width * map_screens;
  static constexpr int map_height = game_height * map_screens;
  static constexpr float player_radius = 10.f;
  static constexpr float player_speed = 300.f;
  static constexpr float player_health = 100.f;
  static constexpr float xp_first_level = 10.f;   // XP needed for the first level up
  static constexpr float run_length = 1200.f;     // a run lasts 20 minutes

  // HUD
  static constexpr float hud_margin = 20.f;       // gap between the HUD and the edge of the screen
  static constexpr float slot_size = 48.f;        // one weapon / relic slot
  static constexpr float slot_gap = 8.f;
  static constexpr int weapon_slots = 6;
  static constexpr int relic_slots = 4;
  static constexpr float health_bar_width = 60.f; // the small bar under the player
  static constexpr float health_bar_height = 8.f;
 

  // Greatsword
  static constexpr float greatsword_range = 120.f;      // how far the swing reaches
  static constexpr float greatsword_cooldown = 1.1f;    // seconds between swings
  static constexpr float greatsword_swing_time = 0.18f; // how long one swing takes
  static constexpr float greatsword_damage = 18.f;
  static constexpr int swing_points = 18;               // how smooth the swing arc is
  //Fire Aura
  static constexpr float fire_aura_radius = 100.f;
  static constexpr float fire_aura_interval = 0.5f;
  static constexpr float fire_aura_damage = 6.f;
  static constexpr float fire_aura_flash = 0.1f;
  static constexpr int aura_points = 40;

  //Throwing Axes
  static constexpr float axe_cooldown = 1.5f;
  static constexpr float axe_speed = 350.f;
  static constexpr float axe_lifetime = 1.2f;
  static constexpr float axe_spin = 720.f;
  static constexpr float axe_radius = 14.f;
  static constexpr float axe_damage = 14.f;


};
