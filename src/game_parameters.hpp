#pragma once

struct Parameters{
  static constexpr int game_width = 1280;     // window size = what the camera can see
  static constexpr int game_height = 720;
  static constexpr int map_screens = 9;       // the map is 9 x 9 screens big
  static constexpr int map_width = game_width * map_screens;
  static constexpr int map_height = game_height * map_screens;
  static constexpr float player_radius = 16.f;        // the size of the player's collision circle
  static constexpr float player_draw_size = 64.f;     // how big the player's sprite is drawn (2 x 32)
  static constexpr float player_speed = 300.f;
  static constexpr float player_health = 100.f;
  static constexpr float xp_first_level = 10.f;   // XP needed for the first level up
  static constexpr float run_length = 1200.f;     // a run lasts 20 minutes
  // Grass
  static constexpr bool show_grid = false;        // the white lines round every screen (for debugging)
  static constexpr float grass_scale = 2.f;       // the 32 x 32 grass tile is drawn twice as big

  static constexpr float xp_growth = 1.3f;        // each level needs this many times more XP than the last
  // Relics
  static constexpr float relic_radius = 30.f;
  static constexpr float relic_draw_size = 64.f;                 // how big a relic's sprite is drawn on the map (2 x 32)                    // size of a relic on the map
  static constexpr float relic_edge_margin = 400.f;              // relics never spawn this close to the edge of the map
  static constexpr float relic_min_distance_from_start = 1500.f; // or this close to where the player starts
  static constexpr float relic_min_distance_between = 2500.f;    // or this close to each other
  static constexpr float relic_damage_bonus = 0.3f;              // damage relic: +30% damage
  static constexpr float relic_cooldown_bonus = 0.2f;            // cooldown relic: weapons wait 20% less between attacks
  static constexpr float relic_speed_bonus = 0.25f;              // speed relic: +25% movement speed
  // Level up / weapon upgrades
  static constexpr int levelup_choices = 3;       // how many options the level-up screen offers
  static constexpr int weapon_max_level = 5;
  static constexpr float upgrade_damage = 0.25f;  // each weapon level adds this much damage (25%)
  static constexpr float upgrade_speed = 0.1f;    // and makes the weapon attack this much faster (10%)
  // Sprites
  static constexpr float sprite_size = 32.f;      // every picture on the spritesheet is 32 x 32 pixels
  static constexpr int sprites_per_row = 4;       // the spritesheet is 4 pictures wide
  // HUD
  static constexpr float hud_margin = 20.f;       // gap between the HUD and the edge of the screen
  static constexpr float slot_size = 64.f;        // one weapon / relic slot
  static constexpr float slot_gap = 8.f;
  static constexpr int weapon_slots = 6;
  static constexpr int relic_slots = 3;
  static constexpr float arrow_margin = 40.f;     // how far from the screen edge the arrows sit
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
