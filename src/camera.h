#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


class CCamera{

public:

    // constructors
    CCamera();
    CCamera(glm::vec3 pos);

    ~CCamera();

    /// set projection matrix to perspective
    void setPerspective( float fovy_f, float aspect_f, float zNear_f, float zFar_f );

    /// set projection matrix to orthogonal
    void setOrtho( float ratio_f, float zNear_f, float zFar_f ); 

    /// move within local coordiante system
    void moveLocal( glm::vec3 d_location );

    void updateMatrizes( void );

    void yawCamera( float d_yaw_f );
    void pitchCamera( float d_pitch_f );

    /// get the View*Projection Matrix
    glm::mat4 getViewProjectionMatrix();



private:

    // defining the view direction
    float yaw_f;
    float pitch_f;

    float rotSpeed;    
    float moveSpeed;

    glm::vec3 position;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 direction;


    void initCamera(glm::vec3 pos);

    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;

};
