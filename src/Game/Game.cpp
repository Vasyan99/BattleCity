#include "Game.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include "Tank.h"

#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Game::Game(const glm::ivec2& windowSize) : m_eCurrentGameState(EGameState::Active), m_windowSize(windowSize) {
	m_keys.fill(false); //в начале игры, когда стартуем ни одна клавиша не нажата 
}

Game::~Game() {

}

void Game::render() {
    //ResourceManager::getAnimatedSprite("NewAnimatedSprite")->render();
    if (m_pTank) {
        m_pTank->render();
    }
}
void Game::update(const uint64_t delta) { //обновление для всех имеющихся спрайтов в игре
   // ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);
    if (m_pTank) {

        if (m_keys[GLFW_KEY_W]) {
            m_pTank->setOrientation(Tank::EOrientation::Top);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_A]) {
            m_pTank->setOrientation(Tank::EOrientation::Left);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_D]) {
            m_pTank->setOrientation(Tank::EOrientation::Right);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_S]) {
            m_pTank->setOrientation(Tank::EOrientation::Bottom);
            m_pTank->move(true);
        }
        else {
            m_pTank->move(false);
        }
        m_pTank->update(delta);
    }

}
void Game::setKey(const int key, const int action) { //обработка нажатия клавиш в игре
	m_keys[key] = action; //если нажимается клавиша, ставится true. Если отжимается - false
}
bool Game::init() { //загрузка всех ресурсов игры
    auto pDefaultShaderProgram = ResourceManager::loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
    if (!pDefaultShaderProgram) {
        std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
        return false;
    }

    auto pSpriteShaderProgram = ResourceManager::loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
    if (!pDefaultShaderProgram) {
        std::cerr << "Can't create shader program: " << "SpriteShader" << std::endl;
        return false;
    }

    auto tex = ResourceManager::loadTexture("DefaultTexture", "res/textures/map_16x16.png");

    std::vector<std::string> subTexturesNames = {
        "block",
        "topBlock",
        "bottomBlock",
        "leftBlock",
        "rightBlock",
        "topLeftBlock",
        "topRightBlock",
        "bottomLeftBlock",

        "bottomRightBlock",
        "beton",
        "topBeton",
        "bottomBeton",
        "leftBeton",
        "rightBeton",
        "topLeftBeton",
        "topRightBeton",

        "bottomLeftBeton",
        "bottomRightBeton",
        "water1",
        "water2",
        "water3",
        "trees",
        "ice",
        "wall",

        "eagle",
        "deadEagle",
        "nothing",
        "respawn1",
        "respawn2",
        "respawn3",
        "respawn4"

    };
    auto pTextureAtlas = ResourceManager::loadTextureAtlas("DefaultTextureAtlas", "res/textures/map_16x16.png", std::move(subTexturesNames), 16, 16);

   
    auto pAnimatedSprite = ResourceManager::loadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "beton");
    pAnimatedSprite->setPosition(glm::vec2(300, 300));
    std::vector<std::pair<std::string, uint64_t>> waterState;
    //добавляем кадры
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water1", 1000000000));
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water2", 1000000000));
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water3", 1000000000));

    std::vector<std::pair<std::string, uint64_t>> eagleState;
    //добавляем кадры
    eagleState.emplace_back(std::make_pair<std::string, uint64_t>("eagle", 1000000000));
    eagleState.emplace_back(std::make_pair<std::string, uint64_t>("deadEagle", 1000000000));

    pAnimatedSprite->insertState("waterState", std::move(waterState));
    pAnimatedSprite->insertState("eagleState", std::move(eagleState));

    pAnimatedSprite->setState("waterState");

    //Теперь всю необходимую информацию о шейдерах передать в память видеокарты. Используем vertexbuffobject

    pDefaultShaderProgram->use(); //подключаем шейдер, который хотим использовать для рисования
    pDefaultShaderProgram->setInt("tex", 0); //имя текстуры и значение - слот. подключая текстуру, мы подключаем её в конкретный слот.


    glm::mat4 modelMatrix_1 = glm::mat4(1.f);
    modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.f, 0.f));

    glm::mat4 modelMatrix_2 = glm::mat4(1.f);
    modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0.f));


    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f); //параметры экрана (фрустума), near and far planes. После перемножения с этой матрицей все точки будут иметь координаты от минус до плюс единицы. Все эти матрицы передаём в вертексный шейдер

    pDefaultShaderProgram->setMatrix("projectionMat", projectionMatrix);


    pSpriteShaderProgram->use(); //подключаем шейдер, который хотим использовать для рисования
    pSpriteShaderProgram->setInt("tex", 0); //имя текстуры и значение - слот. подключая текстуру, мы подключаем её в конкретный слот.

    pSpriteShaderProgram->setMatrix("projectionMat", projectionMatrix);




    std::vector<std::string> tanksSubTexturesNames = {
        "TankTop1",
        "TankTop2",
        "TankLeft1",
        "TankLeft2",
        "TankBottom1",
        "TankBottom2",
        "TankRight1",
        "TankRight2"
    };
    auto pTanksTextureAtlas = ResourceManager::loadTextureAtlas("TanksTextureAtlas", "res/textures/tanks.png", std::move(tanksSubTexturesNames), 16, 16);
    auto pTanksAnimatedSprite = ResourceManager::loadAnimatedSprite("TanksAnimatedSprite", "TanksTextureAtlas", "SpriteShader", 100, 100, "TankTop1");

    std::vector<std::pair<std::string, uint64_t>> tankTopState;
    //добавляем кадры
    tankTopState.emplace_back(std::make_pair<std::string, uint64_t>("TankTop1", 500000000));
    tankTopState.emplace_back(std::make_pair<std::string, uint64_t>("TankTop2", 500000000));

    std::vector<std::pair<std::string, uint64_t>> tankBottomState;
    //добавляем кадры
    tankBottomState.emplace_back(std::make_pair<std::string, uint64_t>("TankBottom1", 500000000));
    tankBottomState.emplace_back(std::make_pair<std::string, uint64_t>("TankBottom2", 500000000));
    std::vector<std::pair<std::string, uint64_t>> tankLeftState;
    //добавляем кадры
    tankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("TankLeft1", 500000000));
    tankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("TankLeft2", 500000000));
    std::vector<std::pair<std::string, uint64_t>> tankRightState;
    //добавляем кадры
    tankRightState.emplace_back(std::make_pair<std::string, uint64_t>("TankRight1", 500000000));
    tankRightState.emplace_back(std::make_pair<std::string, uint64_t>("TankRight2", 500000000));

    pTanksAnimatedSprite->insertState("tankTopState", std::move(tankTopState));
    pTanksAnimatedSprite->insertState("tankBottomState", std::move(tankBottomState));
    pTanksAnimatedSprite->insertState("tankLeftState", std::move(tankLeftState));
    pTanksAnimatedSprite->insertState("tankRightState", std::move(tankRightState));

    pTanksAnimatedSprite->setState("tankTopState");//задаём также начальное состояние

    m_pTank = std::make_unique<Tank>(pTanksAnimatedSprite, 0.0000001f, glm::vec2(100.f, 100.f));

    return true;
}