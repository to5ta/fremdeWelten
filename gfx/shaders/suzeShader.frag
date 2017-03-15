varying vec2 TexCo;

uniform sampler2D texture_diffuse1;

void main()
{    
    gl_FragColor = vec4( texture2D(texture_diffuse1, TexCo));
}