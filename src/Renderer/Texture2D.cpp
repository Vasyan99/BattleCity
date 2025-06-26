#include "Texture2D.h"

namespace Renderer {
	Texture2D::Texture2D(const GLuint width,
		const GLuint height,
		const unsigned char* data,
		const unsigned int channels,
		const GLenum filter,
		const GLenum wrapMode) : m_width(width), m_height(height) {
		//первым делом нужно сгенерировать айди текстуры

		switch (channels) 
		{
		case 4:
			m_mode = GL_RGBA;
			break;
		case 3:
			m_mode = GL_RGB;
			break;
		default:
			m_mode = GL_RGBA;
			break;
		}

		glGenTextures(1, &m_ID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ID); //далее нужно прибиндить эту текстуру (сделать текущей). Все дальнейшие команды только для этой текстуры
		glTexImage2D(GL_TEXTURE_2D, 0, m_mode, m_width, m_height, 0, m_mode, GL_UNSIGNED_BYTE, data);//далее загружаем все параметры. Для какого слота хотим данные использовать. Уровень текстуры (mipmap - каждый последующий уровень даёт текстуру уменьшенную в два раза). Сколько каналов.Ширина высота. Формат данных. Как представляются данные.Ссылка на сами пиксели (data).

		//теперь парочка параметров для фильтрации текстуры.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode); //параметры указывают, каким образом берутся координаты из текстуры, если мы выходим за границы треугольника либо слишком маленькая.кламп ту эйдж - всё что выходит за пределы 1, повторяется последний пиксель.S - по иксу, Т - по игрику. 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter); //фильтрация помогает, если пикселей меньше чем разрешение. На несколько пикселей треугольника приходится один пиксель текстуры. GL_LINEAR - пиксель треугольника усредняется между соседними пикселями.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);  //маг - удаляемся от объекта, на один пиксель треугольника несколько пикселей текстуры.
		glGenerateMipmap(GL_TEXTURE_2D); // генерируем все остальные мип мэпы.

		glBindTexture(GL_TEXTURE_2D, 0); //зануляем активную текстуру
	}

	Texture2D& Texture2D::operator=(Texture2D&& texture2d) {
		glDeleteTextures(1, &m_ID);
		m_ID = texture2d.m_ID;
		texture2d.m_ID = 0;
		m_mode = texture2d.m_mode;
		m_width = texture2d.m_width;
		m_height = texture2d.m_height;
		return *this;

	}


	Texture2D::Texture2D(Texture2D&& texture2d) {
		m_ID = texture2d.m_ID;
		texture2d.m_ID = 0;
		m_mode = texture2d.m_mode;
		m_width = texture2d.m_width;
		m_height = texture2d.m_height;
	}
	Texture2D::~Texture2D() {
		glDeleteTextures(1, &m_ID);  //чтобы уничтожить текстуру, нужно удалить с видеокарты.
	}


	void Texture2D::bind() const { //делаем нашу текстуру активной (текущей)
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void Texture2D::addSubTexture(std::string name, const glm::vec2 leftBottomUV, const glm::vec2 rightTopUV) {
		m_subTextures.emplace(std::move(name), SubTexture2D(leftBottomUV, rightTopUV));
	}
	const Texture2D::SubTexture2D& Texture2D::getSubTexture(std::string name) const {
		auto it = m_subTextures.find(name);
		if (it != m_subTextures.end()) {
			return it->second;
		}
		//в случае, если не удаётся найти сабтекстуру, возвращаем всю текстуру целиком
		const static SubTexture2D defaultSubTexture;
		return defaultSubTexture;
	}


}