#include <camera.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp> 
#include <glm/gtx/euler_angles.hpp>

CCamera::CCamera()
{
    initCamera(glm::vec3(0,0,0));
}

CCamera::CCamera(glm::vec3 pos)
{
    initCamera(pos);
}



void CCamera::yawCamera(float d_yaw_f )
{
    yaw_f += d_yaw_f * rotSpeed;
    updateMatrizes();
}


void CCamera::pitchCamera(float d_pitch_f )
{
    float newPitch = pitch_f + d_pitch_f * rotSpeed;

    if(newPitch > 1.5707f)
        newPitch = 1.5707f;
    if(newPitch < -1.5707f)
        newPitch = -1.5707f;

    pitch_f = newPitch;

    updateMatrizes();
}



void CCamera::initCamera(glm::vec3 pos)
{
    projection_matrix   = glm::ortho(-1.f, 1.f, -1.f, 1.f, -20.f, 20.f); //0.1f, 100.f);
    
    direction   = glm::vec3(0.f, 0.f,-1.f);
    up          = glm::vec3(0.f, 1.f, 0.f);
    right       = glm::vec3(1.f, 0.f, 0.f);

    position = pos;
    moveSpeed= 0.03f;
    rotSpeed = 0.005f;
    yaw_f = -1.57079f;
    pitch_f = 0.f;

    updateMatrizes();
}




void CCamera::updateMatrizes( void )
{
    // yaw pitch pos might have changed

    direction.x = cos(pitch_f) * cos(yaw_f);
    direction.y = sin(pitch_f);
    direction.z = cos(pitch_f) * sin(yaw_f);

    // direction.x = cos(yaw_f);
    // direction.y = 0.f;
    // direction.z = sin(yaw_f);

    glm::vec3 up_global = glm::vec3(0.f,1.f,0.f); 
    right               = glm::normalize(glm::cross(direction, up_global));
    up                  = glm::cross(right, direction);

    view_matrix = glm::lookAt( position, position+direction, up );


    // std::cout<< "\nview matrix\n"<<glm::to_string(view_matrix) <<std::endl;
    // std::cout<< "\nposition\n"<<glm::to_string(position) <<std::endl;
    // std::cout<< "\nright\n"<<glm::to_string(right) <<std::endl;
    // std::cout<< "\ndirection\n"<<glm::to_string(direction) <<std::endl;
    // std::cout<< "\nup\n"<<glm::to_string(up) <<std::endl;
    // std::cout<< "\nyaw_f: "<<yaw_f <<std::endl;
    // std::cout<< "\npitch_f: "<<pitch_f <<std::endl;


}





glm::mat4 CCamera::getViewProjectionMatrix()
{
    return projection_matrix * view_matrix;
}



/// set projection matrix to perspective
void CCamera::setPerspective( float fovy_f, float aspect_f, float zNear_f, float zFar_f )
{
    projection_matrix   = glm::perspective(fovy_f, aspect_f, zNear_f, zFar_f);
}

/// set projection matrix to orthogonal
void CCamera::setOrtho( float ratio_f, float zNear_f, float zFar_f )
{
    projection_matrix   = glm::ortho(-ratio_f, ratio_f, -1.f, 1.f, zNear_f, zFar_f);
}



void CCamera::moveLocal( glm::vec3 d_location )
{

    // bring in our coordinate system
    glm::vec4 d_loc = view_matrix * glm::vec4(d_location.x, d_location.y, d_location.z, 0.f);
    
    std::cout<< "\nd_loc\n"<<glm::to_string(d_loc) <<std::endl;

    // scale with speed
    d_loc = d_loc * moveSpeed; 

    // new position
    position = position + glm::vec3(d_loc.x, d_loc.y, d_loc.z);

    // update
    updateMatrizes();



    // // d_location = d_location*moveSpeed;
    // // std::cout<<glm::to_string(d_location) <<std::endl;
    // // glm::mat4 translate = glm::translate(glm::mat4(1.0f), d_location);

    // vec4 translate = glm::vec4(d_location.x, d_location.y, d_location.z, 1.f);

    // translate = glm::normalize(view_matrix * translate);

    // glm::mat4 translate = glm::mat4( 1.f, 0.f, 0.f, d_location.x,
    //                                  0.f, 1.f, 0.f, d_location.y,
    //                                  0.f, 0.f, 1.f, d_location.z,
    //                                  0.f, 0.f, 0.f, 1.f );

    // std::cout<<"\n-----------\nView Matrix"<<glm::to_string(view_matrix) <<std::endl;
    // translate = view_matrix*translate;
    // std::cout<<"Translate Matrix"<<glm::to_string(translate) <<std::endl;
    // view_matrix = view_matrix + translate;


    
    // = glm::vec4(d_location.x, d_location.y, d_location.z, 1.f);
}






