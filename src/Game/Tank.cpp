#include "Tank.h"

#include "../Renderer/AnimatedSprite.h"


Tank::Tank(std::shared_ptr<Renderer::AnimatedSprite> pSprite, const float velocity, const glm::vec2& position)
	: m_eOrientation(EOrientation::Top),
	m_pSprite(std::move(pSprite)),
	m_move(false),
	m_velocity(velocity),
	m_position(position),
	m_moveOffset(glm::vec2(0.f, 1.f)) { //спрайт, скорость и позици€ танка
	m_pSprite->setPosition(m_position);
}
void Tank::render() const { //будет отрисовывать танк
	m_pSprite->render();
}
void Tank::setOrientation(const EOrientation eOrientation) {
	if (m_eOrientation == eOrientation) {
		return;
	}
	m_eOrientation = eOrientation;

	switch (m_eOrientation)
	{
	case Tank::EOrientation::Top:
		m_pSprite->setState("tankTopState"); // четыре разные анимации дл€ движени€ танка
		m_moveOffset.x = 0.f;
		m_moveOffset.y = 1.f;
		break;
	case Tank::EOrientation::Bottom:
		m_pSprite->setState("tankBottomState");
		m_moveOffset.x = 0.f;
		m_moveOffset.y = -1.f;
		break;
	case Tank::EOrientation::Left:
		m_pSprite->setState("tankLeftState");
		m_moveOffset.x = -1.f;
		m_moveOffset.y = 0.f;
		break;
	case Tank::EOrientation::Right:
		m_pSprite->setState("tankRightState");
		m_moveOffset.x = 1.f;
		m_moveOffset.y = 0.f;
		break;
	default:
		break;

	}

}
void Tank::move(const bool move) { //когда танк будет двигатьс€, должна будет включатьс€ анимаци€ его спрайта
	m_move = move;
}
void Tank::update(const uint64_t delta) { //будет вызывать апдейт спрайта. чтобы спрайт анимировалс€
	if (m_move) {
		m_position += delta * m_velocity * m_moveOffset; //нужно ещЄ двигать спрайт
		m_pSprite->setPosition(m_position);//когда вычислили нужную позицию, надо задать еЄ спрайту. –аботаем с двумерными векторами
		m_pSprite->update(delta); //апдейт спрайта
	}
}