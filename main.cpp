#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <boost/array.hpp>
#include <boost/static_string.hpp>
#include <string>

constexpr unsigned int WIDTH {800};
constexpr unsigned int HEIGHT {600};

/// Especificamos el tamano de la ventana de renderizado en OpenGL
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/// Funcion para manejar los inputs de los usuarios
void processInput(GLFWwindow* window);

/// Creacion de un progrma en GLSL de un Vertex Shader
const std::string vertexShaderSource { 
    R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main() { 
        gl_Position = vec4(aPos, 1.0);
    }
    )glsl"
};

const std::string fragmentShaderSource {
    R"glsl(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    }
    )glsl"
};

const char* sourceV {nullptr};
const char* sourceF {nullptr};

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

    GLFWwindow* window {glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL)};

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

    boost::array<float, 9> vertices { -0.5f, -0.5f, 0.0f,
                                       0.5f, -0.5f, 0.0f,
                                       0.0f,  0.5f, 0.0f };

    unsigned int VBO; 
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /// Funcion que copia los datos del vertice que definimos en la memoria del
    /// buffer
    glBufferData(GL_ARRAY_BUFFER, 
                 vertices.size() * sizeof(float), 
                 vertices.data(), 
                 GL_STATIC_DRAW);

    //glVertexAttribPointer();
    /// Creacion del VAO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /// Compilacion de un vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    sourceV = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &sourceV, NULL);
    glCompileShader(vertexShader);

     /// Encontrar errores de compilacion del Shader
     int success;
     boost::static_string<512> infoLog;
     glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

     if (! success) {
         glGetShaderInfoLog(vertexShader, 512, NULL, infoLog.data());
         std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
     }
     else {
         std::cout << "Compilation Successful" << std::endl;
     }

    /// Creacion del Fragment Shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    sourceF = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &sourceF, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (! success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog.data());
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILARION_FAILED\n" << infoLog << std::endl;
    }
    else {
        std::cout << "Compilation Successful" << std::endl;
    }

    /// Creacion y link del Shader Program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (! success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog.data());
        std::cerr << "ERROR::LINK_PROGRAM\n" << infoLog << std::endl;
    }
    else {
        std::cout << "Link Program Successful" << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /// Blucle de renderizado 
    while (! glfwWindowShouldClose(window)) {

        /// Manejo del Input del usuario
        processInput(window);

        /// Renderizado
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

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
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        std::cout << "Hello, world!!" << std::endl;
    }
}
