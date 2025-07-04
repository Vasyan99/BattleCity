#pragma once
#include <string>
#include <memory>
#include <map>
#include <vector>

namespace Renderer {
	class ShaderProgram;
	class Texture2D;
	class Sprite;
	class AnimatedSprite;
}


class ResourceManager {
public:
	static void setExecutablePath(const std::string& executablePath);
	static void unloadAllResources(); //������ �����������. ������� ��� �������, �������� ������� ������ ��������

	~ResourceManager() = delete;

	ResourceManager() = delete;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	static std::shared_ptr<Renderer::ShaderProgram> loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	static std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string& shaderName);
	static std::shared_ptr<Renderer::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);//��� ������ ������� ����� �������. ��������� �������� ��������, � ����� ��� ��� ����� ������.
	static std::shared_ptr<Renderer::Sprite> loadSprite(const std::string& spriteName,
												 const std::string& textureName,
												 const std::string& shaderName,
												 const unsigned int spriteWidth,
												 const unsigned int spriteHeight,
												 const std::string& subTextureName = "default");


	static std::shared_ptr<Renderer::AnimatedSprite> loadAnimatedSprite(const std::string& spriteName,
													   	 const std::string& textureName,
														 const std::string& shaderName,
														 const unsigned int spriteWidth,
														 const unsigned int spriteHeight,
														 const std::string& subTextureName = "default");
	static std::shared_ptr<Renderer::AnimatedSprite> getAnimatedSprite(const std::string& spriteName);


	static std::shared_ptr<Renderer::Texture2D> getTexture(const std::string& textureName);
	static std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName);

	static std::shared_ptr<Renderer::Texture2D> loadTextureAtlas(std::string textureName,
														  std::string texturePath,
														  std::vector<std::string> subTextures,
														  const unsigned int subTexturewidth,
													      const unsigned int subTextureheight);


private:
	static std::string getFileString(const std::string relativeFilePath);


	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	static ShaderProgramsMap m_shaderPrograms;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
	static TexturesMap m_textures;

	//����� ����� ����� ���������� �������.
	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpritesMap;
	static SpritesMap m_sprites;

	//��������� ��� ��� AnimatedSprite
	typedef std::map<const std::string, std::shared_ptr<Renderer::AnimatedSprite>> AnimatedSpritesMap;
	static AnimatedSpritesMap m_animatedSprites;

	static std::string m_path;
};