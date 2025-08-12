#include "glad.h"
#include "glfw3.h"
#include <stdio.h>

typedef struct {
    int height;
    int width;
    char *title;
} window_config;

int main(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_config w_config = {600, 800, "Learn OpenGL"};

    GLFWwindow* window = glfwCreateWindow(w_config.width, w_config.height, 
    w_config.title, NULL, NULL);

    if (window == NULL) {
        printf("Fail to create GLFW Windows\n");
        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Fail to initialize GLAD");
        return -1;
    }

    glViewport(0, 0, w_config.width, w_config.height);

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    
    return 0;
}
