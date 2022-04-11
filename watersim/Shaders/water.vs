#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

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

	//float modifiedX=0.8*cos(aPos.x-time*0.5f)+aPos.x;
	//float modifiedY=0.2*sin(aPos.x-time*0.5f);//amplitude*sin(x-speed*time)
	
	float modifiedX=rand(vec2(aPos.x,aPos.y))*0.1*cos(aPos.x-time*0.5f)+aPos.x;
	float modifiedY=rand(vec2(aPos.y,aPos.z))*0.1*sin(aPos.x-time*0.5f);//amplitude*sin(x-speed*time)
	
    gl_Position = projection * view * model * vec4(modifiedX,modifiedY,aPos.z,1.0f);
}