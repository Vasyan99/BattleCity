#pragma once

#include <glm/vec2.hpp>
#include <memory>
#include <array>
class Tank;

class Game {
public:

	Game(const glm::ivec2& windowSize);

	~Game();

	void render();
	void update(const uint64_t delta); //обновление для всех имеющихся спрайтов в игре
	void setKey(const int key, const int action); //обработка нажатия клавиш в игре
	bool init(); //загрузка всех ресурсов игры

private:
	std::array<bool, 349> m_keys;

	enum class EGameState {
		Active,
		Pause
	};

	glm::ivec2 m_windowSize;
	EGameState m_eCurrentGameState;
	std::unique_ptr<Tank> m_pTank;
};