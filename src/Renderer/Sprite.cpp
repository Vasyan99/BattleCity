#include "Sprite.h"

#include "ShaderProgram.h"
#include "Texture2D.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace Renderer {

	Sprite::Sprite(const std::shared_ptr<Texture2D> pTexture,
		const std::string initialSubTexture,
		const std::shared_ptr<ShaderProgram> pShaderProgram,
		const glm::vec2& position,
		const glm::vec2& size,
		const float rotation) : m_pTexture(std::move(pTexture)), 
		m_pShaderProgram(std::move(pShaderProgram)),
		m_position(position),
		m_size(size), m_rotation(rotation)
	{

		const GLfloat vertexCoords[] = {
//		2   3	1
//		|		|
//		1	3	2
//		X	  Y
	    0.f,  0.f,
		0.f,  1.f,
		1.f,  1.f,

		1.f,  1.f,
		1.f,  0.f,
		0.f,  0.f
		};

		auto subTexture = pTexture->getSubTexture(std::move(initialSubTexture));

		const GLfloat textureCoords[] = {
//		U	  V
		subTexture.leftBottomUV.x,  subTexture.leftBottomUV.y,
		subTexture.leftBottomUV.x,  subTexture.rightTopUV.y,
		subTexture.rightTopUV.x,    subTexture.rightTopUV.y,

		subTexture.rightTopUV.x,    subTexture.rightTopUV.y,
		subTexture.rightTopUV.x,    subTexture.leftBottomUV.y,
		subTexture.leftBottomUV.x,  subTexture.leftBottomUV.y
		};

		//загружаем информацию в видеокарту

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		//для вертексов
		glGenBuffers(1, &m_vertexCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), &vertexCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);//необходимо включить позицию вертексов. Две позиции - для координат и для цвета, по умолчанию выключены. Функция включает нулевую позицию в шейдере.
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		//то же самое, только для текстуры

		glGenBuffers(1, &m_textureCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), &textureCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);//необходимо включить позицию вертексов. Две позиции - для координат и для цвета, по умолчанию выключены. Функция включает нулевую позицию в шейдере.
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		//оставляем контекст в чистом состоянии
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}



	Sprite::~Sprite() {
		//нужно удалить все данные из памяти видеокарты
		glDeleteBuffers(1, &m_vertexCoordsVBO);
		glDeleteBuffers(1, &m_textureCoordsVBO);
		glDeleteVertexArrays(1, &m_VAO);
	}


	void Sprite::render() const {//метод отрисовки спрайта на экране
		m_pShaderProgram->use();

		glm::mat4 model(1.f);

		//Первым делом нужно скалирование спрайта. Увеличим его в величину, которую передали в спрайt size.

		model = glm::translate(model, glm::vec3(m_position, 0.f));
		model = glm::translate(model, glm::vec3(0.5f * m_size.x, 0.5f * m_size.y, 0.f));
		model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.f, 0.f, 1.f));
		model = glm::translate(model, glm::vec3(-0.5f * m_size.x, -0.5f * m_size.y, 0.f));
		model = glm::scale(model, glm::vec3(m_size, 1.f));
		//Далее делаем rotation. Для правильного поворота нужно переместить центр объекта в ориджин, потом относительно него поворачивать. Далее возвращаем на старую позицию
		//Последним шагом будет постановка спрайта в нужное место.

		//далее нужно забиндить вертекс эррэй обжект. И передаём шейдеру модель матрицу.
		glBindVertexArray(m_VAO);
		m_pShaderProgram->setMatrix("modelMat", model);

		//далее активируем текстуру.
		glActiveTexture(GL_TEXTURE0);
		m_pTexture->bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
	void Sprite::setPosition(const glm::vec2& position) { //переместить наш спрайт в новую позицию или изменить размер
		m_position = position;
	}
	void Sprite::setSize(const glm::vec2& size) {
		m_size = size;
	}
	void Sprite::setRotation(const float rotation) {
		m_rotation = rotation;
	}

}