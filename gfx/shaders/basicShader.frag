#version 130

varying vec2 texCoordinate;

varying vec3 FragPos;
varying vec3 FragNormal;


vec3 lightPos = vec3(2.f, 2.f, 2.f);
vec3 lightColor = vec3(1, 1, 1);

// uniform sampler2D texture_diffuse1;

void main()
{    
	vec3 norm 		= FragNormal / sqrt(FragNormal.x*FragNormal.x + FragNormal.y*FragNormal.y + FragNormal.z*FragNormal.z);
	vec3 lightDir 	= normalize(lightPos - FragPos); 

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor; 

    // gl_FragColor = vec4( texture2D(texture_diffuse1, texCoordinate));
    // gl_FragColor = vec4(1,1,1,1);
    gl_FragColor = vec4(diffuse,1);

    // gl_FragColor = vec4(diffuse_color, 1.0f);
    // gl_FragColor = gl_Color;
}

