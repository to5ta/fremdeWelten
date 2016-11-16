#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/gtx/string_cast.hpp> 
#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>


#include <cstdlib> // for random numbers

#include <CShader.hpp>
#include <camera.h>




CCamera* camera;


GLfloat lastX = 0.f;
GLfloat lastY = 0.f;
bool firstMouse = false;



static struct
{
public:
    float x, y, z;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 0.f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 0.f, 1.f }
};

static const char* vertex_shader_text =
"uniform float factor;\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec3 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
// "    color = vCol;\n"
"    color = vec3(vCol[0]*factor, vCol[1]*factor, vCol[2]*factor );\n"
"}\n";

static const char* fragment_shader_text =
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";






static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}





static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera->yawCamera(-xoffset);
    camera->pitchCamera(-yoffset);

}  



static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS || key == GLFW_KEY_Q && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

    else if (key == GLFW_KEY_LEFT )
    {
        camera->moveLocal( glm::vec3(-1, 0, 0) );
    }
    else if (key == GLFW_KEY_RIGHT )
    {
        camera->moveLocal( glm::vec3(1, 0, 0) );
    }    
    else if (key == GLFW_KEY_UP )
    {
        camera->moveLocal( glm::vec3(0, 0, -1) );
    }
    else if (key == GLFW_KEY_DOWN )
    {
        camera->moveLocal( glm::vec3(0, 0, 1) );
    }

    else if (key == GLFW_KEY_A )
    {
        camera->yawCamera( 0.04f );
    }
    else if (key == GLFW_KEY_D )
    {
        camera->yawCamera( -0.04f );
    }    
    else if (key == GLFW_KEY_W )
    {
        camera->pitchCamera( -0.04f );
    }
    else if (key == GLFW_KEY_S )
    {
        camera->pitchCamera( 0.04f );
    }
}



static int initFremdeWelten( void )
{
    std::cout << "initFremdeWelten..." << std::endl;
    camera = new CCamera(glm::vec3(0.f, 0.f, 10.f));
    return 0;
}





int main(void)
{
    
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(640, 480, "Fremde Welten", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // fullscreen stuff
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    // glfwSetWindowMonitor( window, primary, 0, 0, 1920, 1080, 60 );   


    initFremdeWelten();

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);


    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    GLint fact_location;
    fact_location = glGetUniformLocation(program, "factor");
    glUniform1f(fact_location, 1.);

    std::cout << fact_location << std::endl;

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 6, (void*) 0);

    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 6, (void*) (sizeof(float) * 3));

    // glEnableVertexAttribArray(fact_location);


    GLfloat color_factors[9];
    
    const float* vp_array;

    // mat4x4 m, p, mvp;
    
    glm::mat4 m, p, mvp, v;

    while (!glfwWindowShouldClose(window))
    {

        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);


        #if 1
        glUseProgram(program);

        // camera->yawCamera( 0.01f );
        camera->setPerspective(0.4f, ratio, 0.1f, 100.f );


        vp_array = glm::value_ptr( camera->getViewProjectionMatrix() );
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, vp_array );

        // camera->printView();
        // camera->printVectors();

        // camera->pitchCamera( 0.01f );

        
        /// very first approach
        // mat4x4_identity(m);
        // mat4x4_rotate_Y(m, m, (float) glfwGetTime());
        // mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        // mat4x4_mul(mvp, p, m);
        // glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);

        /// own approach with glm math
        // m = glm::mat4(1.f);

        // v = glm::mat4(1.f);
        // // v = glm::rotate( v, (float) glfwGetTime()*0.2f, glm::vec3( 0.f, 1.f, 0.f) );
        // v = glm::rotate( v, 3.141f, glm::vec3( 0.f, 1.f, 0.f) );
        
        // vp_array = glm::value_ptr( camera->getViewProjectionMatrix() );
        
        // v = glm::translate(v, glm::vec3(1,0,10));
        
        // p = glm::perspective( 45.f, ratio, 0.1f, 100.f );
        // // p = glm::ortho( -ratio, ratio, -1.f, 1.f, -20.f, 20.f );

        // mvp = p * v * m;
        // mvp = p * m;

        // std::cout<< "cam mvp" << glm::to_string(camera->getViewProjectionMatrix()) << std::endl;
        // std::cout<< "gen mvp" << glm::to_string(mvp) << std::endl;
        // glUniformMatrix4fv( mvp_location, 1, GL_FALSE, glm::value_ptr(mvp) );
        


      
        // float f;
        // f = (float)( std::rand() / (float)(RAND_MAX) );

        // // std::cout << glfwGetTime() << std::endl;
        glUniform1f(fact_location, 1.f);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // // let color jitter
        // for(int i=0;i<3;i++)
        // {
        //     vertices[i].r = std::rand();
        //     // vertices[i].g = std::rand();
        //     vertices[i].b = std::rand();
        // }
        // m = glm::rotate( m, (float) glfwGetTime()*2.f, glm::vec3( 1.f, 0.f, 0.f) );
        // mvp = p * v * m;
        // glUniformMatrix4fv( mvp_location, 1, GL_FALSE, glm::value_ptr(mvp) );
        
        // // glUniformMatrix4fv( mvp_location, 1, GL_FALSE, glm::value_ptr(mvp) );
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // // glDrawArrays(GL_LINE_LOOP, 0, 3);


        #else
        glLoadIdentity();
        glRotatef(0.1,1,1,0);
        glTranslatef(-0.1,0,0);
        glPushMatrix();
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        glBegin(GL_TRIANGLES);
            glColor3f(1,0,0);
            glVertex3f( 1,-1, 0);
            glVertex3f(-1,-1, 0);  
            glVertex3f( 0, 1, 0);
        glEnd();
        glPopMatrix();
        #endif 


        float scale = 0.2f;
        int length = 10;

        glLoadMatrixf( vp_array );

        glBegin(GL_LINES);
            for(int i=-length; i<=length; i++)
            {
                for(int j=-length; j<=length; j++)
                {
                    glVertex3f(float(length)*scale, float(j)*scale, 0 );
                    glVertex3f(float(-length)*scale, float(j)*scale, 0 );
                }

                glVertex3f(float(i)*scale, float(length)*scale, 0 );
                glVertex3f(float(i)*scale, float(-length)*scale, 0 );
            }
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
