//// exmple from https://capnramses.github.io/opengl/hellotriangle.html ///////////////////

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include "camera.h"

// author:  tosta
// date:    2016-10-25


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    std::cout<<"Key: "<<key<<"Action: "<<action<<std::endl;

    if(key==GLFW_KEY_Q or key==GLFW_KEY_ESCAPE)
    {
        glfwDestroyWindow( window );
    }
}


static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    std::cout << "Position: " << xpos << "|" <<ypos <<std::endl;
}




// int main(int argc, char** argv)
int main() {
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: could not init GLFW3\n");
        return 1;
    }


    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);

    if (!window)
    {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    // start GLEW extension handler
        glewExperimental = GL_TRUE;
    glewInit();

    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"



    // input handling
    glfwSetKeyCallback(window, key_callback);

    //
    glfwSetCursorPosCallback(window, cursor_position_callback);


    while(!glfwWindowShouldClose(window))
    {
        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glUseProgram(shader_programme);
        // glBindVertexArray(vao);
        // // draw points 0-3 from the currently bound VAO with current in-use shader
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // // update other events like input handling

        glfwPollEvents();
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
    }

    // close GL context and any other GLFW resources
    glfwTerminate();
  return 0;
}


















////// Getting started example //////
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include "linmath.h"
// #include <stdlib.h>
// #include <stdio.h>

// static const struct
// {
//     float x, y;
//     float r, g, b;
// } vertices[3] =
// {
//     { -0.6f, -0.4f, 1.f, 0.f, 0.f },
//     {  0.6f, -0.4f, 0.f, 1.f, 0.f },
//     {   0.f,  0.6f, 0.f, 0.f, 1.f }
// };

// static const char* vertex_shader_text =
// "uniform mat4 MVP;\n"
// "attribute vec3 vCol;\n"
// "attribute vec2 vPos;\n"
// "varying vec3 color;\n"
// "void main()\n"
// "{\n"
// "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
// "    color = vCol;\n"
// "}\n";

// static const char* fragment_shader_text =
// "varying vec3 color;\n"
// "void main()\n"
// "{\n"
// "    gl_FragColor = vec4(color, 1.0);\n"
// "}\n";


// static void error_callback(int error, const char* description)
// {
//     fprintf(stderr, "Error: %s\n", description);
// }


// static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
// {
//     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, 1);
//         // glfwSetWindowShouldClose(window, GLFW_TRUE);
// }


// int main(void)
// {
//     GLFWwindow* window;
//     GLuint vertex_buffer, vertex_shader, fragment_shader, program;
//     GLint mvp_location, vpos_location, vcol_location;

//     glfwSetErrorCallback(error_callback);

//     if (!glfwInit())
//         exit(EXIT_FAILURE);

//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//     window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
//     if (!window)
//     {
//         glfwTerminate();
//         exit(EXIT_FAILURE);
//     }

//     glfwSetKeyCallback(window, key_callback);
//     glfwMakeContextCurrent(window);
//      // gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

//     // glewInit();

//     glfwSwapInterval(1);
//     // NOTE: OpenGL error checks have been omitted for brevity
//     glGenBuffers(1, &vertex_buffer);
//     glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//     vertex_shader = glCreateShader(GL_VERTEX_SHADER);
//     glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
//     glCompileShader(vertex_shader);
//     fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
//     glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
//     glCompileShader(fragment_shader);
//     program = glCreateProgram();
//     glAttachShader(program, vertex_shader);
//     glAttachShader(program, fragment_shader);
//     glLinkProgram(program);
//     mvp_location = glGetUniformLocation(program, "MVP");
//     vpos_location = glGetAttribLocation(program, "vPos");
//     vcol_location = glGetAttribLocation(program, "vCol");
//     glEnableVertexAttribArray(vpos_location);
//     glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
//                           sizeof(float) * 5, (void*) 0);
//     glEnableVertexAttribArray(vcol_location);
//     glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
//                           sizeof(float) * 5, (void*) (sizeof(float) * 2));
//     while (!glfwWindowShouldClose(window))
//     {
//         float ratio;
//         int width, height;
//         mat4x4 m, p, mvp;
//         glfwGetFramebufferSize(window, &width, &height);
//         ratio = width / (float) height;
//         glViewport(0, 0, width, height);
//         glClear(GL_COLOR_BUFFER_BIT);
//         mat4x4_identity(m);
//         mat4x4_rotate_Z(m, m, (float) glfwGetTime());
//         mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
//         mat4x4_mul(mvp, p, m);
//         glUseProgram(program);
//         glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
//         glDrawArrays(GL_TRIANGLES, 0, 3);
//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }
//     glfwDestroyWindow(window);
//     glfwTerminate();
//     exit(EXIT_SUCCESS);
// }






