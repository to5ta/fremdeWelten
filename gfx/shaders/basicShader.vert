#version 130 

attribute vec3 position;
attribute vec2 texCoords;
attribute vec3 normal;


// varying vec2 texCoordinate;

// varying vec4 gl_Color;

uniform mat4 MVP;

varying vec3 FragPos;
varying vec3 FragNormal;

void main()
{
    // gl_Position = MVP * vec4(normal, 1.0f);
    gl_Position = MVP * vec4(position, 1.0f);
    // texCoordinate = texCoords;
    gl_FrontColor = gl_Color;
    // TexCo = vec2(texCoords.x ,1);

    // FragPos = vec3(model * vec4(position, 1.0f));
    FragPos = position;
    FragNormal = normal;
}





// attribute vec3 position;
// attribute vec2 uv;
// attribute vec3 normal;

// varying vec2 texCoordinate;

// uniform mat4 MVP;

// void main()
// {
//     gl_Position = MVP * vec4(position, 1.0f);
//     texCoordinate = uv;
// }

