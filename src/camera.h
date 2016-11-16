#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


class CCamera{

public:

    // constructors
    CCamera();
    CCamera(glm::vec3 pos);
    CCamera(glm::vec3 pos, glm::vec3 dir);

    ~CCamera();

    /// set projection matrix to perspective
    void setPerspective( float fovy_f, float aspect_f, float zNear_f, float zFar_f );

    /// set projection matrix to orthogonal
    void setOrtho( float ratio_f, float zNear_f, float zFar_f ); 


    // setRotation?
    // setPosition?

    /// move within local coordiante system
    void moveLocal( glm::vec3 d_location );

    void updateMatrizes( void );

    /// move in global coordinate system
    // void moveGlobal( glm::vec3 d_location );

    /// rotate with d_rad arround axis
    // void rotateGlobal( float d_rad_f, glm::vec3 axis );

    void yawCamera( float d_yaw_f );
    void pitchCamera( float d_pitch_f );

    /// get the View*Projection Matrix
    glm::mat4 getViewProjectionMatrix();



    // void printProjection( void )
    // {
    //     float* proj = glm::value_ptr( this->projection_matrix );
    //     std::cout << "Projection\n"<<proj[0]<<" "<<proj[1]<<" "<<proj[1]<<" "<<proj[3]<< std::endl;
    //     std::cout <<proj[4]<<" "<<proj[5]<<" "<<proj[6]<<" "<<proj[7]<< std::endl;
    //     std::cout <<proj[8]<<" "<<proj[9]<<" "<<proj[10]<<" "<<proj[11]<< std::endl;
    //     std::cout <<proj[12]<<" "<<proj[13]<<" "<<proj[14]<<" "<<proj[15]<< std::endl;
    //     std::cout << std::endl;
    // }
    // void printView( void )
    // {
    //     float* proj = glm::value_ptr( this->view_matrix );
    //     std::cout << "\n-----\nView\n"<<proj[0]<<" "<<proj[1]<<" "<<proj[1]<<" "<<proj[3]<< std::endl;
    //     std::cout <<proj[4]<<" "<<proj[5]<<" "<<proj[6]<<" "<<proj[7]<< std::endl;
    //     std::cout <<proj[8]<<" "<<proj[9]<<" "<<proj[10]<<" "<<proj[11]<< std::endl;
    //     std::cout <<proj[12]<<" "<<proj[13]<<" "<<proj[14]<<" "<<proj[15]<< std::endl;
    //     std::cout << std::endl;
    // }

    // void printVectors( void )
    // {
    //     std::cout << "right:" <<" "<<right.x<<" "<<right.y<<" "<<right.z << std::endl;
    //     std::cout << "direction:" <<" "<<direction.x<<" "<<direction.y<<" "<<direction.z << std::endl;
    //     std::cout << "target:" <<" "<<target.x<<" "<<target.y<<" "<<target.z << std::endl;
    //     std::cout << "up:" <<" "<<up.x<<" "<<up.y<<" "<<up.z << std::endl;
    //     std::cout << "position:" <<" "<<position.x<<" "<<position.y<<" "<<position.z << std::endl;

    // }


private:

    // defining the view direction
    float yaw_f;
    float pitch_f;
    float rotSpeed;
    glm::mat4 rotation_matrix;
    
    float moveSpeed;
    glm::vec3 position;

    void initCamera(glm::vec3 pos, glm::vec3 dir);

    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;

};
