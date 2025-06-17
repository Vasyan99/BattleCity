#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer/ShaderProgram.h"

GLfloat point[] = {
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};
//Программка для вертексного шейдера
const char* vertex_shader =
"#version 460\n" //версия шейдера, которую будем использовать
"layout(location = 0) in vec3 vertex_position;" //входные данные, которые получает наш шейдер. позиция вертекса и его цвет в нашем случае
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;" //выходные данные. цвет. пойдёт на вход фрагментному шейдеру
"void main() {"
"   color = vertex_color;" //фрагментный шейдер получит интерполированное значение из этой переменной
"   gl_Position = vec4(vertex_position, 1.0);" // обозначает позицию вертекса, который потом пойдёт на выход. Должна быть в нормированных координатах. Четвёртая компонента - перспектива
"}";
//Программка для фрагментного шейдера
const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main() {"
"   frag_color = vec4(color, 1.0);" //четвёртое значение - прозрачность. Полностью непрозрачный - 1, прозрачный - 0 
"}";


int g_windowSizeX = 640;
int g_windowSizeY = 480;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY); // показываем, где хотим рисовать, в какой области окна. В параметры передаём координаты левого нижнего угла окна (0, 0), далее ширина и высота
//Меняется область, где хотим рисовать в окне

}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE); //Если нажата клавиша ескейп, закрываем окно
    }
}



int main(void)
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
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Battle City", nullptr, nullptr); //размеры, название окна и доп. параметры монитор + другое окно
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

    std::string vertexShader(vertex_shader);
    std::string fragmentShader(fragment_shader);

    Renderer::ShaderProgram shaderProgram(vertex_shader, fragment_shader);
	
    if (!shaderProgram.isCompiled()) {
        std::cerr << "Can't create shader program!" << std::endl;
        return -1;
    }

    //Теперь всю необходимую информацию о шейдерах передать в память видеокарты. Используем vertexbuffobject

    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo); //драйвер генерирует один вертекс драйвер обжект. И записывает значение идентификатора по этому указателю, что передали сюда.
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo); //подключили, сделали текущим
    glBufferData(GL_ARRAY_BUFFER,sizeof(point), point, GL_STATIC_DRAW);//Теперь заполняем информацией - позициями вертекса на видеокарту. тип буффера, данные, подсказка для драйвера (куда хотим записать данные на видеокарте)
    //эту подсказку драйвер использует для того, чтобы оптимально разместить данные на видеокарте. динамик дроу - собираемся часто менять данные, драйвер разместил бы в другой области памяти где запись быстрее. Команда выполняется для текущего связанного
    
    //Теперь всё то же самое, только для массива цвета
    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo); //драйвер генерирует один вертекс драйвер обжект. И записывает значение идентификатора по этому указателю, что передали сюда.
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo); //подключили, сделали текущим
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);//Теперь заполняем информацией - позициями вертекса на видеокарту. тип буффера, данные, подсказка для драйвера (куда хотим записать данные на видеокарте)
    //Из оперативной памяти переместили цвета и вертексы в память видеокарты. Видеокарта пока не знает, что с этими данными делать и как связать с входными аттрибутами шейдера.
    //Нужен для этого vertexarrayobject.

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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow)) //главная Render Loop. Делаем отрисовку на экране
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT); //очищает буффер цвета 

        shaderProgram.use(); //подключаем шейдер, который хотим использовать для рисования
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);//Сама команда отрисовки. отрисовывает текущий забинденый обжект.

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow); //видеокарта рисует задний буффер, монитор отображает передний. 
        //Буфферы меняются местами, кадр заднего буффера закончен

        /* Poll for and process events */
        glfwPollEvents(); //Позволяет glfw обработать все события, такие как нажатие клавиши, клик мыши.
    }

    glfwTerminate(); //освободить все ресурсы glfw
    return 0;
}