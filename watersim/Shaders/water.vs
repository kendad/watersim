#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    TexCoords = aTexCoords;
	Normal=mat3(transpose(inverse(model))) * aNormal;
	FragPos=vec3(model*vec4(aPos,1.0f));

	//float modifiedX=0.1*cos(1-time*0.9f)+1;
	//float modifiedY=0.3*sin(modifiedX-time*0.9f);//amplitude*sin(x-speed*time)
	
	float modifiedX=rand(vec2(aPos.x,aPos.y))*0.1*cos(aPos.x-time*0.9f)+aPos.x;
	float modifiedY=rand(vec2(aPos.z,aPos.y))*0.1*sin(modifiedX-time*0.9f);//amplitude*sin(x-speed*time)
	
	vec4 finalPos=vec4(modifiedX,modifiedY,aPos.z,1.0f);
	
    gl_Position = projection * view * model * finalPos;
}