#include "glad.h"
#include "glfw3.h"
#include "stdread.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int height;
    int width;
    const char *title;
} window_config;

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(void) {
    // =========================
    // Inicialização GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window_config w_config = {600, 800, "Learn OpenGL"};

    GLFWwindow* window = glfwCreateWindow(w_config.width, w_config.height,
                                          w_config.title, NULL, NULL);
    if (!window) {
        printf("Fail to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Fail to initialize GLAD\n");
        return -1;
    }

    // =========================
    // Dados do triângulo

    /*  (1)             (0)           (4)        
        
        +---------------------------------+
        |                |                |
        |                |                |
        |                |                |
        |                |                |
        +----------------+----------------+
        
        (2)             (3)            (5) 
    */
    float vertices[] = {
        0.5f, 0.5f, 0.0f, // 0 - cima/direita
        0.5f, -0.5f, 0.0f, // 1 - baixo/direita
        -0.5f, -0.5f, 0.0f, // 2 - baixo/esquerda
        -0.5f, 0.5f, 0.0f, // 3 - cima/esquerda

        1.5f,  0.5f, 0.0f,  // 4 - topo/direita (novo)
        1.5f, -0.5f, 0.0f,  // 5 - baixo/direita (novo)
    };
    
    unsigned int indices[] = { // começa do 0
        0, 1, 3, // primeiro triangulo
        1, 2, 3, // segundo triangulo

        // quadrado da direita (novo)
        4, 5, 0,
        5, 1, 0
    };

    unsigned int VBO, VAO, EBO;

    // Cria e vincula VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Cria e vincula VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // cria e vincula EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    

    // Configura atributo de vértice
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Desvincula VAO
    glBindVertexArray(0);

    // =========================
    // Carregar shaders de arquivo
    char *vertex_shader_source = read_file("shader/shader.vert");
    if (!vertex_shader_source) {
        printf("Error ao ler vertex shader\n");
        return -1;
    }

    char *frag_shader_source = read_file("shader/shader.frag");
    if (!frag_shader_source) {
        printf("Error ao ler fragment shader\n");
        return -1;
    }

    const char *vs_ptr = vertex_shader_source;
    const char *fs_ptr = frag_shader_source;

    // =========================
    // Compilar vertex shader
    unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vs_ptr, NULL);
    glCompileShader(vert_shader);

    int success;
    char infoLog[512];
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vert_shader, 512, NULL, infoLog);
        printf("ERROR::VERTEX_SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    }

    // Compilar fragment shader
    unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &fs_ptr, NULL);
    glCompileShader(frag_shader);

    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frag_shader, 512, NULL, infoLog);
        printf("ERROR::FRAGMENT_SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    }

    // =========================
    // Criar shader program
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vert_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        printf("ERROR::SHADER_PROGRAM::LINK_FAILED\n%s\n", infoLog);
    }

    // =========================
    // Liberar shaders individuais
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    // Liberar memória das strings lidas
    free(vertex_shader_source);
    free(frag_shader_source);

    // Configurar viewport
    glViewport(0, 0, w_config.width, w_config.height);

    // =========================
    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        process_input(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Usar shader program e VAO para desenhar
        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // =========================
    // Limpar recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return 0;
}
