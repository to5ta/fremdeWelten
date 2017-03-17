#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtx/string_cast.hpp> 
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

#include <cstdlib> 
// for random numbers

// enables image loading
#include <SOIL/SOIL.h>

// #include <assimp>


#include <Shader.hpp>
#include <Model.h>

// my camera
#include <Camera.h>

// my Modell
#include <myModel.h>


Camera* camera;


GLfloat lastX = 0.f;
GLfloat lastY = 0.f;

bool firstMouse = false;
// bool fullscreen;


static struct
{
public:
    float x, y, z;
    float r, g, b;
    float u, v;
} vertices[4] =
{
    {  0.5f, 0.6f, 0.f,  0.f, 1.f, 0.f,  0.f,  1.f },
    {  0.5f, 1.6f, 0.f,  0.f, 0.f, 1.f,  0.f,  0.f },
    { -0.5f, 0.6f, 0.f,  1.f, 0.f, 0.f,  1.f,  1.f },
    { -0.5f, 1.6f, 0.f,  0.f, 0.f, 1.f,  1.f, 0.0f }
};



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

    camera->yawCamera(xoffset);
    camera->pitchCamera(yoffset);

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

    else if (key == GLFW_KEY_R )
        camera->setHeight( 1.5f );


    // does not work as glfwSetWindowMonitor is not defined?!
    // else if (key == GLFW_KEY_F )
    // {
    //     if (fullscreen)
    //     {
    //         glfwSetWindowMonitor(   window, // the glfw window were using
    //                                 NULL,   // monitor to use, NONE for window mode
    //                                 0,      // xpos,
    //                                 0,      // ypos,
    //                                 800,    // width,
    //                                 600,    // height,
    //                                 30 );    // refreshRate 
    //     } 
    //     else
    //     {
    //         glfwSetWindowMonitor(   window, // the glfw window were using
    //                                 glfwGetPrimaryMonitor(),   // monitor
    //                                 0,      // xpos,
    //                                 0,      // ypos,
    //                                 800,    // width,
    //                                 600,    // height,
    //                                 30 );    // refreshRate 
    //     }
    // }

}



static int initFremdeWelten( void )
{
    std::cout << "initFremdeWelten..." << std::endl;
    camera = new Camera(glm::vec3(0.f, 1.f, 4.f));
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);



    // window mode
    window = glfwCreateWindow(1920, 950, "Fremde Welten", NULL, NULL);
    


    // fullscreen mode due to glfwGetPrimaryMonitor()
    // window = glfwCreateWindow(1920, 1080, "Fremde Welten", glfwGetPrimaryMonitor(), NULL);

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


    // enable depth testing, Very important
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 

    // antialiasing: has this realy an impact
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);  

    // blending for transparancy
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 



    std::cout << glGetString(GL_VERSION) << std::endl;


    Shader torchShader( "../gfx/shaders/torch_sprite_ani.vert", "../gfx/shaders/torch_sprite_ani.frag" );


    // NOTE: OpenGL error checks have been omitted for brevity
    GLuint vertex_buffer, vertex_array;
    
    glGenVertexArrays(1, &vertex_array );

    glBindVertexArray( vertex_array );

    glGenBuffers(1, &vertex_buffer);
    printf("Vertex_Buffer=%i\n", vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // this seems to be a Buffer* thing
    GLint vcol_location = glGetAttribLocation(torchShader.programID, "vCol");
    GLint vpos_location = glGetAttribLocation(torchShader.programID, "vPos");
    GLint uv_location   = glGetAttribLocation(torchShader.programID, "vUV");

    // printf("IDs torchShader: %i %i %i\n", vpos_location, vcol_location, uv_location );
    
    // enable VertexAttributes, NON-oFF SHADER BUSSINESS
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

    // printf("uv_location=%i\n", uv_location );
    glEnableVertexAttribArray(uv_location);  
    glVertexAttribPointer(  uv_location,    // index of attrib, Order or hw dependend
                            2,              // number of elements per vert of that attribute 
                            GL_FLOAT,       // 
                            GL_FALSE,       // 
                            8 * sizeof(GLfloat),            // stride, internal offset
                            (GLvoid*)(6 * sizeof(GLfloat))  // offset in buffer for first element
                        );

    // glEnableVertexAttribArray(fact_location);

    glBindVertexArray( 0 );

    // glBindBuffer(GL_ARRAY_BUFFER, 0);



    // ////////////////////////////////
    // // actual shader creation
    // GLuint vertex_shader, fragment_shader, program;

    // vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    // glCompileShader(vertex_shader);

    // fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    // glCompileShader(fragment_shader);

    // program = glCreateProgram();
    // glAttachShader(program, vertex_shader);
    // glAttachShader(program, fragment_shader);
    // glLinkProgram(program);

    // //////////
    // // get location ("id/adress") access
    // GLint mvp_location = glGetUniformLocation(program, "MVP");
    // printf("mvp_location=%i\n", mvp_location );
    // GLint vpos_location = glGetAttribLocation(torchShader.programID, "vPos");
    // printf("vpos_location=%i\n", vpos_location );
    // GLint vcol_location = glGetAttribLocation(torchShader.programID, "vCol");
    // printf("vcol_location=%i\n", vcol_location );

    // GLint step_location = glGetUniformLocation(program, "step");
    // printf("step_location=%i\n", step_location );
    // glUniform1f(step_location, 0.f);
  
    // // enter time
    // GLint time_location = glGetUniformLocation(program, "time");
    // printf("time_location=%i\n", time_location );
    // glUniform1f(time_location, 0.f);

    // // scale vertex color later with that variable
    // GLint fact_location;
    // fact_location = glGetUniformLocation(program, "factor");
    // glUniform1f(fact_location, 1.);

    // // negative when failed?
    // // std::cout << fact_location << std::endl;
    /////////
    //////////////////////////////////



   

    //////////////////////////////////
    // create texture
    int width, height;
    unsigned char* image = NULL;
    image = SOIL_load_image("../gfx/torch_spritesheet.jpg", &width, &height, 0, SOIL_LOAD_RGB); 

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
    // glBindTexture(GL_TEXTURE_2D, 0);

    /////////////////////////////////



    //////////////////////////////////





    // GLfloat color_factors[9];
    // glVertexAttribPointer(  vcol_location,              // GLuint index                                    
    //                         3,                          // GLint size                        
    //                         GL_FLOAT,                   // GLenum type                                 
    //                         GL_FALSE,                   // GLboolean normalized                                 
    //                         sizeof(float) * 6,          // GLsizei stride                                        
    //                         (void*) (sizeof(float) * 3) // const GLvoid * glVertexAttribPointer                                                 
    //                     );


    #define SUZE 0

    #if SUZE
        // Shader suzeShader("../gfx/shaders/basicShader.vert", "../gfx/shaders/basicShader.frag" );
        Shader suzeShader( "../gfx/shaders/suzeShader.vert", "../gfx/shaders/suzeShader.frag" );
        Model suzeModel("../gfx/assets/wache_frame143.obj");
        // Model suzeModel("../gfx/assets/monkey_with_chrown.obj");
    #endif

    Shader basicShader("../gfx/shaders/basicShader.vert", "../gfx/shaders/basicShader.frag" );
    
    const float* mvp_array;

    // mat4x4 m, p, mvp;
    
    glm::mat4 m, p, mvp, v;



    double elapsed = 0.0;
    char title[100];

    #define MYMODEL 1
    #if MYMODEL

        #if 1
        // CModel* monkey = new CModel("../gfx/assets/sphere.obj");
        // CModel* monkey = new CModel("../gfx/assets/can.obj");
        // CModel* monkey = new CModel("../gfx/assets/money_with_chrown.obj");
        CModel* monkey = new CModel("../gfx/assets/money_with_chrown.obj");
        // CModel* monkey = new CModel("../gfx/assets/pyr.obj");
        #else


        vector<glm::vec3> allpos;
        vector<glm::vec3> allnormals;
        vector<glm::vec2> alluvs;
        vector<triangle_t> allTris;

        allpos.push_back( glm::vec3( -1,0, -0.1 ));
        allpos.push_back( glm::vec3( -1,1, -0.1 ));
        allpos.push_back( glm::vec3(  1,1, -0.1 ));
        allpos.push_back( glm::vec3(  1,0, -0.1 ));

        triangle_t tri1 = triangle_t {{0,1,2}, {0,0,0}, {0,0,0}};
        triangle_t tri2 = triangle_t {{0,2,3}, {0,0,0}, {0,0,0}};

        allTris.push_back( tri1 );
        allTris.push_back( tri2);

        CModel* monkey = new CModel( &allpos,
                                     &allnormals,
                                     &alluvs,
                                     &allTris );

        #endif
    #endif


    int frames = 0;

    while (!glfwWindowShouldClose(window))
    {

        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        camera->setPerspective(0.4f, ratio, 0.1f, 100.f );
        glViewport(0, 0, width, height);
        glClearColor(0.02f, 0.02f, 0.04f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

        mvp_array = glm::value_ptr( camera->getViewProjectionMatrix() );



        // bad-evil immediate mode to draw grid:D
        glUseProgram(0);
        float scale = 1.f;
        int length = 10;
        glLoadMatrixf( mvp_array );
        glColor3f(0.1f,0.1f,0.1f);
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



        // draw FIRE
        
        #if 1 

        glBindVertexArray( vertex_array );
        // glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        torchShader.use();

        glUniform1f(glGetUniformLocation(torchShader.programID, "step"), elapsed*33.f);

        // glUniformMatrix4fv(mvp_location, 1, GL_FALSE, mvp_array );
        glUniformMatrix4fv(glGetUniformLocation(torchShader.programID, "MVP"), 1, GL_FALSE, mvp_array);

        // make it sparkling
        glUniform1f(glGetUniformLocation(torchShader.programID, "factor"), 
                    (1.f + sin(glfwGetTime()*15.33f)*sin(glfwGetTime()*7.777f))/3.f + 0.6f );
        

        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray( 0 );

        #endif



        #if SUZE
        suzeShader.use();
        glUniformMatrix4fv(glGetUniformLocation(suzeShader.programID, "MVP"), 1, GL_FALSE, mvp_array);
        suzeModel.Draw(suzeShader);
        #endif

        #if MYMODEL 
            monkey->updateMVP( mvp_array, frames++ );
        #endif

        // std::cout << "MVP Monkey: " << monkey->MVP << std::endl;
        // std::cout << "MVP       : " << mvp_array << std::endl;
        // printf("mvp:\n % 4.1f % 4.1f % 4.1f % 4.1f\n", mvp_array[0], mvp_array[1], mvp_array[2], mvp_array[3] );
        // printf("% 4.1f % 4.1f % 4.1f % 4.1f\n", mvp_array[4], mvp_array[5], mvp_array[6], mvp_array[7] );
        // printf("% 4.1f % 4.1f % 4.1f % 4.1f\n", mvp_array[8], mvp_array[9], mvp_array[10], mvp_array[11] );
        // printf("% 4.1f % 4.1f % 4.1f % 4.1f\n", mvp_array[12], mvp_array[13], mvp_array[14], mvp_array[15] );
        
        #if MYMODEL
            #if 0
            // basicShader.use();
            // glUniformMatrix4fv(glGetUniformLocation(basicShader.programID, "MVP"), 1, GL_FALSE, mvp_array);
            
            // glUseProgram( monkey->meshes[0]->material->shader->programID );
            monkey->meshes[0]->material->shader->use();
            glUniformMatrix4fv(glGetUniformLocation(monkey->meshes[0]->material->shader->programID, "MVP"), 1, GL_FALSE, mvp_array);

            glBindVertexArray(monkey->meshes[0]->VAO);
            glDrawArrays(GL_TRIANGLES, 0, monkey->meshes[0]->vertices.size() );
            glBindVertexArray( 0 );
            #else
            monkey->draw( );
            #endif
        #endif

        glfwSwapBuffers(window);
        glfwPollEvents();

        sprintf(title, "Fremde Welten | FPS % 6.2f", 1.f/(glfwGetTime()-elapsed));

        glfwSetWindowTitle(window, title);
        elapsed = glfwGetTime();


    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
