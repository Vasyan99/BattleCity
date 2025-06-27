#pragma once

#include "Sprite.h"

#include <map>
#include <vector>

namespace Renderer {

	class AnimatedSprite : public Sprite {
	public:

		AnimatedSprite( std::shared_ptr<Texture2D> pTexture, 
			    std::string initialSubTexture,
			    std::shared_ptr<ShaderProgram> pShaderProgram, 
			   const glm::vec2& position = glm::vec2(0.f), 
			   const glm::vec2& size = glm::vec2(1.f),
			   const float rotation = 0.f);
	
		//для установки самого состояния. (добавление значения в мэп)
		void insertState(std::string state, std::vector<std::pair<std::string, uint64_t>> subTexturesDuration);

		void render() const override;

		void setState(const std::string& newState); //будет устанавливать конкретное состояние нашего спрайта.
		void update(const uint64_t delta); //передаём длительность времени, прошедшего с последнего кадра. Относительно этой длительности будем отсчитывать, когда наступит следующий кадр.

	private:
		//будет содержать различные состояния спрайта, и длительность кадра спрайта. Мэп - ключом название состояния, а значением - вектор кадров и к каждому кадру будет добавлена длительность. Вектор из пар, в паре название сабтекстуры которая обозначает конкретный кадр, и длительность.
		std::map<std::string, std::vector<std::pair<std::string, uint64_t>>> m_statesMap;
		//каждому состоянию последовательность кадров. К каждому кадру идёт длительность

		size_t m_currentFrame = 0; //счётчик кадров
		uint64_t m_currentAnimationTime = 0; //время, сколько прошло с установки состояния. В апдейте приращиваем. Когда достигает определённой длительности, меняем кадр и сбрасываем параметр в ноль. Каррент фрэйм добавляем 1.
		std::map<std::string, std::vector<std::pair<std::string, uint64_t>>>::const_iterator m_pCurrentAnimationDurations;
		mutable bool m_dirty = false; // Чтобы не передавать позиции и данные в видеокарту весь кадр. меняется при обновлении кадра.

	};


}