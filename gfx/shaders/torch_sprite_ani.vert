attribute vec3 vCol;
attribute vec3 vPos;
attribute vec2 vUV;

uniform float factor;
uniform float step;
uniform mat4 MVP;

varying vec3 color;
varying vec2 uv;


void main()
{
    float i = mod(floor(step), 36);
    float du = floor(mod(i, 6.f));
    float dv = floor(i/6.f);

    gl_Position = MVP * vec4(vPos, 1.0);
//     gl_Position = MVP * vec4(vPos.x, vPos.y, vPos.z, 1.0);
    uv[0] = (vUV[0]/6.f) + 1.f/6.f * du;
    uv[1] = (vUV[1]/6.f) + 1.f/6.f * dv;
//     color = vCol;
    color = vec3(vCol[0]*factor, vCol[1]*factor, vCol[2]*factor );

}