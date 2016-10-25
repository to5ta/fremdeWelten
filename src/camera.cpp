#include <camera.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>


Camera_t::Camera_t()
{
    initCamera(glm::vec3(0,0,0), glm::vec3(0,0,-1));
}


Camera_t::Camera_t(glm::vec3 pos)
{
    initCamera(pos, glm::vec3(0,0,-1));
}


Camera_t::Camera_t(glm::vec3 pos, glm::vec3 dir)
{
    initCamera(pos, dir);
}


void Camera_t::initCamera(glm::vec3 pos, glm::vec3 dir)
{
    position    = pos;
    direction   = dir;
    target      = pos + dir;
    up          = glm::vec3(0,1,0);
    right       = glm::cross(up, direction);
}


void Camera_t::yawCamera(float d_yaw)
{
    direction   = glm::rotateY(direction, d_yaw);
    target      = position + direction;
}


void Camera_t::pitchCamera(float d_pitch)
{
    right       = glm::normalize(glm::cross(direction, up));
    direction   = glm::rotate(direction, -d_pitch, right);
    target      = position + direction;

}

void Camera_t::strafeCamera(float d_strafe)
{
    position = position + right*-d_strafe;
    target      = position + direction;
}

void Camera_t::walkCamera(float d_walk)
{
    position = position + direction*d_walk;
    target      = position + direction;
}


