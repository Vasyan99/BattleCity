#pragma once

#include <glm/vec2.hpp>
#include <memory>

namespace Renderer {
	class AnimatedSprite;
}


class Tank {
public:


	enum class EOrientation {
		Top,
		Bottom,
		Left,
		Right
	};

	Tank(std::shared_ptr<Renderer::AnimatedSprite> pSprite, const float velocity, const glm::vec2& position); //спрайт, скорость и позиция танка

	void render() const; //будет отрисовывать танк
	void setOrientation(const EOrientation eOrientation);
	void move(const bool move); //когда танк будет двигаться, должна будет включаться анимация его спрайта
	void update(const uint64_t delta); //будет вызывать апдейт спрайта. чтобы спрайт анимировался
private:
	EOrientation m_eOrientation; //танк будет кататься по полю. надо вычислять, куда будет лететь снаряд
	std::shared_ptr<Renderer::AnimatedSprite> m_pSprite;
	bool m_move;
	float m_velocity;
	glm::vec2 m_position;
	glm::vec2 m_moveOffset; //в какую сторону, на какой оффсет будет перемещаться танк. вектор, по которому будет двигаться танк.
};