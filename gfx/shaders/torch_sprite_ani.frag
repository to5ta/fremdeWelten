uniform float factor;
uniform sampler2D Tex;

varying vec3 color;
varying vec2 uv;


vec2 uvpos = vec2(0.5, 0.5); 
void main()
{
    vec4 tex = texture2D(Tex, uv);
    if(tex[0]<0.08f) discard;
    tex[3] = tex[0];
    gl_FragColor = tex*factor;
}