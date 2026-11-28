#pragma once
// EMPTY FOR NOW. Still to add here (brief: some form of AI or physics):
//  - an Enemy class that chases the player
//  - spawning enemies just off screen on a timer, more of them as time goes on
//  - enemies not stacking on top of each other
//  - damage to the player on contact, and the player's health
//  - XP drops, levelling up and choosing upgrades
//  - bosses on a timer, the 20 minute run, win and lose
//  - performance: lots of enemies at 1080p / 60 fps (brief: evidence of performance analysis)

#pragma once
#include <SFML/Graphics.hpp>

struct Enemy
{
	sf::CircleShape shape;
	float speed = 115.f;
	float radius = 30.f;
	float health = 10.f;

	void update(float dt, const sf::Vector2f& playerPos);
	void render(sf::RenderWindow& window) const;
	bool isDead() const;
};