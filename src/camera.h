#include <glm/vec3.hpp>

typedef struct Camera_t{
    // constructors
    Camera_t();
    Camera_t(glm::vec3 pos);
    Camera_t(glm::vec3 pos, glm::vec3 dir);

    // methods
    void initCamera(glm::vec3 pos, glm::vec3 dir);
    void yawCamera(float d_yaw);
    void pitchCamera(float d_pitch);
    void walkCamera(float d_fly);
    void strafeCamera( float d_strafe );

    // members
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 direction;
    glm::vec3 up;
    glm::vec3 right;

} Camera_t;
