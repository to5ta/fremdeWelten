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
    initCamera(glm::vec3(0,0,0), glm::vec3(0,0,-1));
}


CCamera::CCamera(glm::vec3 pos)
{

    initCamera(pos, glm::vec3(0,0,-1));
}


CCamera::CCamera(glm::vec3 pos, glm::vec3 dir)
{
    initCamera(pos, dir);
}



void CCamera::initCamera(glm::vec3 pos, glm::vec3 dir)
{
    projection_matrix   = glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.1f, 100.f); //0.1f, 100.f);
    // view_matrix         = glm::lookAt( pos, pos+dir, glm::vec3(0.f,1.f,0.f));

    position = pos;
    std::cout<< "\n----\nposition\n"<<glm::to_string(position) <<std::endl;
    moveSpeed= 0.03f;
    rotSpeed = 0.03f;

    yaw_f = 0.f;
    pitch_f = 0.f;

    rotation_matrix = glm::mat4(1.f);

    std::cout<<"View Matrix"<<glm::to_string(view_matrix) <<std::endl;

    updateMatrizes();
}



// void CCamera::yawCamera(float d_yaw_f )
// {
//     glm::vec3 up = glm::vec3(0.f,1.f,0.f);
//     view_matrix = glm::rotate( view_matrix, d_yaw_f, up );
// }



// void CCamera::pitchCamera(float d_pitch_f )
// {
//     glm::vec4 right = glm::vec4(1.f, 0.f,0.f,1.f);
//     right = view_matrix * right;
//     view_matrix = glm::rotate( view_matrix, d_pitch_f, glm::vec3(right.x,right.y,right.z) );
// }


void CCamera::yawCamera(float d_yaw_f )
{
    yaw_f += d_yaw_f;
    updateMatrizes();
}


void CCamera::pitchCamera(float d_pitch_f )
{
    pitch_f += d_pitch_f;
    updateMatrizes();
}



void CCamera::updateMatrizes( void )
{

    std::cout<<"\n----"<<std::endl;

    glm::mat4 pitch_matrix          = glm::rotate(glm::mat4(1.0f), pitch_f, glm::vec3(1.0f, 0.0f, 0.0f));
    rotation_matrix                 = glm::rotate(pitch_matrix, yaw_f, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 translation_matrix    = glm::translate(glm::mat4(1.0f), position);
    
    std::cout<< "\ntranslation_matrix\n"<<glm::to_string(translation_matrix) <<std::endl;

    view_matrix                     = translation_matrix * rotation_matrix;

    std::cout<< "\nrotation matrix\n"<<glm::to_string(rotation_matrix) <<std::endl;
    std::cout<< "\nposition\n"<<glm::to_string(position) <<std::endl;
    std::cout<< "\nview matrix\n"<<glm::to_string(view_matrix) <<std::endl;
    std::cout<< "\nyaw_f: "<<yaw_f <<std::endl;
    std::cout<< "\npitch_f: "<<pitch_f <<std::endl;




    // /// calc new rotation matrix
    // rotation_matrix = glm::eulerAngleYXZ( yaw_f, pitch_f, 0.f );

    // std::cout<< "\n----\nrotmat\n"<<glm::to_string(rotation_matrix) <<std::endl;

    // float* rm = glm::value_ptr( rotation_matrix );
    // float* p  = glm::value_ptr( position );

    // /// recompose ViewMatrix from rm and p
    // view_matrix = glm::mat4( rm[0], rm[1], rm[2], p[0],
    //                          rm[3], rm[4], rm[5], p[1],
    //                          rm[6], rm[7], rm[8], p[2],
    //                          0.f,   0.f,   0.f,   1.f  );
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
    glm::vec4 d_loc = rotation_matrix * glm::vec4(d_location.x, d_location.y, d_location.z, 1.f);

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






