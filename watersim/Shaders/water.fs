#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 waterColor;
uniform vec3 cameraPosition;

void main()
{   //Ambient Light
	float ambientStrength=0.1;
	vec3 ambient=ambientStrength*lightColor*vec3(texture(texture_diffuse1, TexCoords));
	
	//Diffuse Light
	vec3 norm=normalize(Normal);
	vec3 lightDir=normalize(lightPosition-FragPos);
	float diff=max(dot(norm,lightDir),0.0);
	vec3 diffuse=diff*lightColor*vec3(texture(texture_diffuse1, TexCoords));
	
	//Specular Light
	float specularStrength=0.5f;
	vec3 viewDir=normalize(cameraPosition-FragPos);
	vec3 reflectDir=(-lightDir,norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specular = specularStrength * spec * lightColor*vec3(texture(texture_diffuse1, TexCoords));  
	
	vec3 result=(ambient+diffuse+specular)*waterColor;
	
    //FragColor = texture(texture_diffuse1, TexCoords)*vec4(result,1.0f);
	FragColor=vec4(result,1.0f);
}