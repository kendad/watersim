#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 waterColor;

void main()
{   //Ambient Light
	float ambientStrength=0.1;
	vec3 ambient=ambientStrength*lightColor;
	
	//Diffuse Light
	vec3 norm=normalize(Normal);
	vec3 lightDir=normalize(lightPosition-FragPos);
	float diff=max(dot(norm,lightDir),0.0);
	vec3 diffuse=diff*lightColor;
	
	vec3 result=(ambient+diffuse)*waterColor;
	
    //FragColor = texture(texture_diffuse1, TexCoords);
	FragColor=vec4(result,1.0f);
}