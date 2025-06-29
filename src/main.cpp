#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>


#include <chrono>

#include <iostream>

#include "Game/Game.h"
#include "Resources/ResourceManager.h"

glm::ivec2 g_windowSize(640, 480);
Game g_game(g_windowSize);


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
    g_game.setKey(key, action);
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


	glClearColor(0, 0, 0, -1);

    {
        ResourceManager::setExecutablePath(argv[0]); //первый аргумент содержит путь к экзешнику.
        g_game.init();
        

        auto lastTime = std::chrono::high_resolution_clock::now();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow)) //главная Render Loop. Делаем отрисовку на экране
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
            lastTime = currentTime;

            g_game.update(duration);


            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT); //очищает буффер цвета 

            g_game.render();

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow); //видеокарта рисует задний буффер, монитор отображает передний. 
            //Буфферы меняются местами, кадр заднего буффера закончен

            /* Poll for and process events */
            glfwPollEvents(); //Позволяет glfw обработать все события, такие как нажатие клавиши, клик мыши.
        }
        ResourceManager::unloadAllResources();
    }

    glfwTerminate(); //освободить все ресурсы glfw
    return 0;
}