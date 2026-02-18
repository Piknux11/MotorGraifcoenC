#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>

constexpr unsigned int WIDTH {800};
constexpr unsigned int HEIGHT {600};

/// Especificamos el tamano de la ventana de renderizado en OpenGL
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/// Funcion para manejar los inputs de los usuarios
void processInput(GLFWwindow* window);

int main(void) {

    /// Configuracion de la Window y del Context OpenGL
    if (! glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window =  glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);

    if (window == NULL) {
        std::cerr << "Failed to create a Window with GLFW\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    if (! gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to load the OpenGL functions";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /*
    float vertices[] = { -0.5f, -0.5f, 0.0f,
                          0.5f, -0.5f, 0.0f,
                          0.0f,  0.5f, 0.0f };
    */

    std::array<float, 9> vertices = { -0.5f, -0.5f, 0.0f,
                                       0.5f, -0.5f, 0.0f,
                                       0.0f,  0.5f, 0.0f };

    unsigned int VBO; 
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /// Funcion que copia los datos del vertice que definimos en la memoria del
    /// buffer
    glBufferData(GL_ARRAY_BUFFER, 
                 vertices.size() * sizeof(float), 
                 vertices.data(), 
                 GL_STATIC_DRAW);

    /// Blucle de renderizado 
    while (! glfwWindowShouldClose(window)) {

        /// Manejo del Input del usuario
        processInput(window);

        /// Renderizado
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
