#define STB_IMAGE_IMPLEMENTATION

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <stdio.h>

#include "ErrorCheck.h"
#include "Shader.h"
#include "Camera.h"
//#include "CommonValues.h"
#include "Terrain.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "stb_image.h"

Camera camera;
bool isGUIHovered;

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW Initialization Error");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glEnable(GL_DEPTH_TEST);

    int octave = 2;
    float GUIscale = 1;
    float elevation = 0;
    float distance = 1;

    Terrain terrain;
    terrain.generatePerlinTerrain(128);
    //terrain.generateSmoothNoise(256);

    Shader shader("res/Shader.shader");
    shader.useShader();
    shader.setUniform4f("u_Color", 0.55f, 0.40f, 0.30f, 1.0f);

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    shader.setUniformMat4("u_Projection", proj);

    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, elevation, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    shader.setUniformMat4("u_View", camera.getView());

    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    double xPos = 0;
    double yPos = 0;

    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCall(glClearColor(0.8f, 0.3f, 0.2f, 1.0f));

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        shader.useShader();
        double newXpos;
        double newYpos;
        glfwGetCursorPos(window, &newXpos, &newYpos);
        double dXpos = newXpos - xPos;
        double dYpos = newYpos - yPos;

        isGUIHovered = ImGui::IsWindowHovered();
        
        ImGui::SliderInt("Octave", &octave, 1, 8);
        ImGui::SliderFloat("Elevation", &elevation, -5, 5);
        ImGui::SliderFloat("Scale", &GUIscale, 0, 10);
        ImGui::SliderFloat("Camera Distance", &distance, 0.75f, 1.5f);

        camera.update(-dXpos, -dYpos, elevation, distance);
        xPos = newXpos;
        yPos = newYpos;
        shader.setUniformMat4("u_View", camera.getView());

        if (ImGui::Button("Regenerate"))
        {
            terrain.clearSeed();
            terrain.update(octave, GUIscale * 10);
        }

        terrain.update(octave, GUIscale*10);

        terrain.draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !isGUIHovered)
        camera.setShouldMove(true);
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        camera.setShouldMove(false);
}