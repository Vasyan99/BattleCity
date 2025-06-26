#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"

GLfloat point[] = {
     0.0f,  50.f, 0.0f,
     50.f, -50.f, 0.0f,
    -50.f, -50.f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

GLfloat texCoord[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};


glm::ivec2 g_windowSize(640, 480);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0, 0, g_windowSize.x, g_windowSize.y); // показываем, где хотим рисовать, в какой области окна. В параметры передаём координаты левого нижнего угла окна (0, 0), далее ширина и высота
//Меняется область, где хотим рисовать в окне

}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE); //Если нажата клавиша ескейп, закрываем окно
    }
}



int main(int argc, char** argv) //в параметрах мэйн передаётся путь к экзешнику.
{


    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "glfwInit failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //хотим использовать версию 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //хотим использовать core profile для opengl
    //Core profile - определённое подмножество функций opengl куда не включены разные функции обратной совместимости

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "Battle City", nullptr, nullptr); //размеры, название окна и доп. параметры монитор + другое окно
    if (!pWindow)
    {
        std::cout << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback); //Регистрируем наш коллбэк. Теперь, когда меняем вручную размер окна - меняются и параметры
    glfwSetKeyCallback(pWindow, glfwKeyCallback);


    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow); //делаем контекст окна для текущего окна. окон может быть несколько для каждого контекст
	
	if (!gladLoadGL())
	{
		std::cout << "Can't load GLAD!" << std::endl;
		return -1;
	}
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;


	glClearColor(0, 1, 0, 1);

    {

        ResourceManager resourceManager(argv[0]); //первый аргумент содержит путь к экзешнику.

        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram) {
            std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
            return -1;
        }

        auto pSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pDefaultShaderProgram) {
            std::cerr << "Can't create shader program: " << "SpriteShader" << std::endl;
            return -1;
        }

        auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

        std::vector<std::string> subTexturesNames = { "block", "topBlock", "bottomBlock", "leftBlock", "rightBlock", "topLeftBlock", "topRightBlock","bottomLeftBlock", "bottomRightBlock", "beton"};
        auto pTextureAtlas = resourceManager.loadTextureAtlas("DefaultTextureAtlas", "res/textures/map_16x16.png", std::move(subTexturesNames), 16, 16);



        auto pSprite = resourceManager.loadSprite("NewSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "beton");
        pSprite->setPosition(glm::vec2(300, 100));

        //Теперь всю необходимую информацию о шейдерах передать в память видеокарты. Используем vertexbuffobject

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo); //драйвер генерирует один вертекс драйвер обжект. И записывает значение идентификатора по этому указателю, что передали сюда.
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo); //подключили, сделали текущим
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);//Теперь заполняем информацией - позициями вертекса на видеокарту. тип буффера, данные, подсказка для драйвера (куда хотим записать данные на видеокарте)
        //эту подсказку драйвер использует для того, чтобы оптимально разместить данные на видеокарте. динамик дроу - собираемся часто менять данные, драйвер разместил бы в другой области памяти где запись быстрее. Команда выполняется для текущего связанного

        //Теперь всё то же самое, только для массива цвета
        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo); //драйвер генерирует один вертекс драйвер обжект. И записывает значение идентификатора по этому указателю, что передали сюда.
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo); //подключили, сделали текущим
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);//Теперь заполняем информацией - позициями вертекса на видеокарту. тип буффера, данные, подсказка для драйвера (куда хотим записать данные на видеокарте)
        //Из оперативной памяти переместили цвета и вертексы в память видеокарты. Видеокарта пока не знает, что с этими данными делать и как связать с входными аттрибутами шейдера.
        //Нужен для этого vertexarrayobject.
        
         //загружаем для текстуры
        GLuint texCoord_vbo = 0;
        glGenBuffers(1, &texCoord_vbo); //драйвер генерирует один вертекс драйвер обжект. И записывает значение идентификатора по этому указателю, что передали сюда.
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo); //подключили, сделали текущим
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);//Теперь заполняем информацией - позициями вертекса на видеокарту. тип буффера, данные, подсказка для драйвера (куда хотим записать данные на видеокарте)

        GLuint vao = 0;
        glGenVertexArrays(1, &vao); //указываем сколько массивов и указатель на идентификатор.
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);//необходимо включить позицию вертексов. Две позиции - для координат и для цвета, по умолчанию выключены. Функция включает нулевую позицию в шейдере.
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo); //подключили буффер, сделали текущим опять
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        //для цвета
        glEnableVertexAttribArray(1);//необходимо включить позицию вертексов. Две позиции - для координат и для цвета, по умолчанию выключены. Функция включает нулевую позицию в шейдере.
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo); //подключили буффер, сделали текущим опять
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        //Всё сделали, остаётся выполнить отрисовку в основном цикле. 
        ///для текстуры
        glEnableVertexAttribArray(2);//необходимо включить позицию вертексов. Две позиции - для координат и для цвета, по умолчанию выключены. Функция включает нулевую позицию в шейдере.
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo); //подключили буффер, сделали текущим опять
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);


        pDefaultShaderProgram->use(); //подключаем шейдер, который хотим использовать для рисования
        pDefaultShaderProgram->setInt("tex", 0); //имя текстуры и значение - слот. подключая текстуру, мы подключаем её в конкретный слот.


        glm::mat4 modelMatrix_1 = glm::mat4(1.f);
        modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.f, 0.f));

        glm::mat4 modelMatrix_2 = glm::mat4(1.f);
        modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0.f));


        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f); //параметры экрана (фрустума), near and far planes. После перемножения с этой матрицей все точки будут иметь координаты от минус до плюс единицы. Все эти матрицы передаём в вертексный шейдер

        pDefaultShaderProgram->setMatrix("projectionMat", projectionMatrix);


        pSpriteShaderProgram->use(); //подключаем шейдер, который хотим использовать для рисования
        pSpriteShaderProgram->setInt("tex", 0); //имя текстуры и значение - слот. подключая текстуру, мы подключаем её в конкретный слот.

        pSpriteShaderProgram->setMatrix("projectionMat", projectionMatrix);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow)) //главная Render Loop. Делаем отрисовку на экране
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT); //очищает буффер цвета 

            pDefaultShaderProgram->use(); //подключаем шейдер, который хотим использовать для рисования
            glBindVertexArray(vao);
            tex->bind();

            pDefaultShaderProgram->setMatrix("modelMat", modelMatrix_1);
            glDrawArrays(GL_TRIANGLES, 0, 3);//Сама команда отрисовки. отрисовывает текущий забинденый обжект.


            pDefaultShaderProgram->setMatrix("modelMat", modelMatrix_2); //рисуется другой треугольник с другой позицией (другая матрица преобразования)
            glDrawArrays(GL_TRIANGLES, 0, 3);//Сама команда отрисовки. отрисовывает текущий забинденый обжект.

            pSprite->render();

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow); //видеокарта рисует задний буффер, монитор отображает передний. 
            //Буфферы меняются местами, кадр заднего буффера закончен

            /* Poll for and process events */
            glfwPollEvents(); //Позволяет glfw обработать все события, такие как нажатие клавиши, клик мыши.
        }
    }

    glfwTerminate(); //освободить все ресурсы glfw
    return 0;
}