
attribute vec3 position;
attribute vec2 texCoords;
attribute vec3 normal;

varying vec2 TexCo;

uniform mat4 MVP;

void main()
{
    // gl_Position = MVP * vec4(normal, 1.0f);
    gl_Position = MVP * vec4(position*0.4f, 1.0f);
    TexCo = texCoords;
    // TexCo = vec2(texCoords.x ,1);
}