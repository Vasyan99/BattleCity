#pragma once

#include <array>

class Game {
public:

	Game();

	~Game();

	void render();
	void update(const uint64_t delta); //обновление для всех имеющихся спрайтов в игре
	void setKey(const int key, const int action); //обработка нажатия клавиш в игре
	bool init(); //загрузка всех ресурсов игры

private:
	std::array<bool, 349> m_keys;

	enum EGameState {
		Active,
		Pause
	};

	EGameState m_eCurrentGameState;
;};