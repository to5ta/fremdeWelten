#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtx/string_cast.hpp> 
// #include "linmath.h"
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
#include <bmpImage.h>

// enables image loading
#include <SOIL/SOIL.h>


CCamera* camera;

GLfloat lastX = 0.f;
GLfloat lastY = 0.f;
bool firstMouse = false;


static struct
{
public:
    float x, y, z;
    float r, g, b;
    float u, v;
} vertices[3] =
{
    { -0.6f, 0.1f, 0.f, 1.f, 0.f, 0.f, 1.f,  0.f },
    {  0.6f, 0.1f, 0.f, 0.f, 1.f, 0.f, 1.f,  1.f },
    {   0.f,  1.1f, 0.f, 0.f, 0.f, 1.f, 0.f,  0.5f }
};

static const char* vertex_shader_text =
"uniform float factor;\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec3 vPos;\n"
"attribute vec2 vUV;\n"
"varying vec3 color;\n"
"varying vec2 uv;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"    uv = vUV;\n"
// "    color = vCol;\n"
"    color = vec3(vCol[0]*factor, vCol[1]*factor, vCol[2]*factor );\n"
"}\n";

static const char* fragment_shader_text =
"varying vec3 color;\n"
"varying vec2 uv;\n"
"uniform sampler2D Tex;\n"
"vec2 uvpos = vec2(0.5, 0.5);\n" 
"void main()\n"
"{\n"
"    gl_FragColor = texture(Tex, uvpos);\n"
// "    vec4 tex = texture(Tex, uv);\n"
// "    gl_FragColor = vec4(color, 1.0);\n"
// "    gl_FragColor = vec4(uvpos,1.0, 1.0);\n"
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
        camera->strafeCamera( -1.f);
    }
    else if (key == GLFW_KEY_RIGHT )
    {
        camera->strafeCamera( 1.f );
    }    
    else if (key == GLFW_KEY_UP )
    {
        camera->pushCamera( 1.f );

    }
    else if (key == GLFW_KEY_DOWN )
    {
        camera->pushCamera( -1.f );
    }

    else if (key == GLFW_KEY_A )
    {
        camera->yawCamera( 0.1f );
    }
    else if (key == GLFW_KEY_D )
    {
        camera->yawCamera( -0.1f );
    }    
    else if (key == GLFW_KEY_W )
    {
        camera->pitchCamera( -0.1f );
    }
    else if (key == GLFW_KEY_S )
    {
        camera->pitchCamera( 0.1f );
    }
}



static int initFremdeWelten( void )
{
    std::cout << "initFremdeWelten..." << std::endl;
    camera = new CCamera(glm::vec3(0.f, 1.5f, 10.f));
    return 0;
}





int main(void)
{
    // own init
    initFremdeWelten();
    

    GLFWwindow* window;
  
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


    // fullscreen stuff - does not work right now
    // GLFWmonitor* primary = glfwGetPrimaryMonitor();
    // glfwSetWindowMonitor( window, primary, 0, 0, 1920, 1080, 60 );   


    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    // seems to activate v-sync
    glfwSwapInterval(1);


    // NOTE: OpenGL error checks have been omitted for brevity
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    printf("Vertex_Buffer=%i\n", vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



    //////////////////////////////////
    // actual shader creation
    GLuint vertex_shader, fragment_shader, program;

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
    /////////////////////////////////



    // get location ("id/adress") access
    GLint mvp_location = glGetUniformLocation(program, "MVP");
    printf("mvp_location=%i\n", mvp_location );
    GLint vpos_location = glGetAttribLocation(program, "vPos");
    printf("vpos_location=%i\n", vpos_location );
    GLint vcol_location = glGetAttribLocation(program, "vCol");
    printf("vcol_location=%i\n", vcol_location );


    // scale vertex color later with that variable
    GLint fact_location;
    fact_location = glGetUniformLocation(program, "factor");
    glUniform1f(fact_location, 1.);

    // negative when failed?
    // std::cout << fact_location << std::endl;
    //////////////////////////////////



    //////////////////////////////////
    // enable VertexAttributes
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(  vpos_location, 
                            3, 
                            GL_FLOAT, 
                            GL_FALSE,
                            sizeof(GLfloat) * 8,    // stride!
                            (GLvoid*) 0               // vermutlich Position im Buffer? siehe oben
                        );

    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(  vcol_location,              // GLuint index                                    
                            3,                          // GLint size                        
                            GL_FLOAT,                   // GLenum type                                 
                            GL_FALSE,                   // GLboolean normalized                                 
                            sizeof(GLfloat) * 8,        // GLsizei stride                                        
                            (GLvoid*) (sizeof(GLfloat) * 3) // const GLvoid * glVertexAttribPointer                                                 
                        );
    // glEnableVertexAttribArray(fact_location);
    //////////////////////////////////




    //////////////////////////////////
    // create texture
    int width, height;
    unsigned char* image = NULL;
    image = SOIL_load_image("../gfx/fire.png", &width, &height, 0, SOIL_LOAD_RGB); 

    if(!image)
    {
        printf("Image loading failed!\n");
    }


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    GLuint texture;
    glGenTextures(1, &texture); 

    glBindTexture(GL_TEXTURE_2D, texture);  

    // Now that the texture is bound, we can start generating a 
    // texture using the previously loaded image data. Textures 
    // are generated with glTexImage2D: 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);


    // After we're done generating the texture and its corresponding mipmaps, 
    // it is good practice to free the image memory and unbind the texture object: 
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);


    GLint uv_location = glGetAttribLocation(program, "vUV");

    printf("uv_location=%i\n", uv_location );

    glEnableVertexAttribArray(uv_location);  
    glVertexAttribPointer(  uv_location,    // index of attrib, Order or hw dependend
                            2,              // number of elements per vert of that attribute 
                            GL_FLOAT,       // 
                            GL_FALSE,       // 
                            8 * sizeof(GLfloat),            // stride, internal offset
                            (GLvoid*)(6 * sizeof(GLfloat))  // offset in buffer for first element
                        );


    //////////////////////////////////





    // GLfloat color_factors[9];
    // glVertexAttribPointer(  vcol_location,              // GLuint index                                    
    //                         3,                          // GLint size                        
    //                         GL_FLOAT,                   // GLenum type                                 
    //                         GL_FALSE,                   // GLboolean normalized                                 
    //                         sizeof(float) * 6,          // GLsizei stride                                        
    //                         (void*) (sizeof(float) * 3) // const GLvoid * glVertexAttribPointer                                                 
    //                     );


    
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


    
        glUseProgram(program);

        // camera->yawCamera( 0.01f );
        camera->setPerspective(0.4f, ratio, 0.1f, 100.f );


        vp_array = glm::value_ptr( camera->getViewProjectionMatrix() );
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, vp_array );


        // // std::cout << glfwGetTime() << std::endl;

        // ...call the amublances :D
        glUniform1f(fact_location, (1.f + sin(glfwGetTime()*2.f))/2.f );
        


        glBindTexture(GL_TEXTURE_2D, texture);
        // glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0);









        // bad-evil immediate mode :D

        glUseProgram(0);

        float scale = 0.2f;
        int length = 10;

        glLoadMatrixf( vp_array );

        glBegin(GL_LINES);
            for(int i=-length; i<=length; i++)
            {
                for(int j=-length; j<=length; j++)
                {
                    glVertex3f(float(length)*scale, 0, float(j)*scale );
                    glVertex3f(float(-length)*scale, 0, float(j)*scale );
                }

                glVertex3f(float(i)*scale, 0, float(length)*scale );
                glVertex3f(float(i)*scale, 0, float(-length)*scale);
            }
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
