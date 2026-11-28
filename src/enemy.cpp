// EMPTY FOR NOW. See enemy.hpp for what is still to add.

#include "enemy.hpp"
#include <cmath>

void Enemy::update(float dt, const sf::Vector2f& playerPos)
{
	sf::Vector2f toPlayer = playerPos - shape.getPosition();
	float length = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
	if (length > 0.001f)
	{
		sf::Vector2f direction(toPlayer.x / length, toPlayer.y / length);
		shape.move(direction * speed * dt);
	}
}

void Enemy::render(sf::RenderWindow& window) const
{
	window.draw(shape);
}

bool Enemy::isDead() const
{
	return health <= 0.f;
}