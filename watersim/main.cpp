#include <iostream>
#include "globals.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "model.h"

//screen settings
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

//timing
float deltaTime = 0.0f;
float lastTime = 0.0f;

void main() {
	//intialize GLFW & GLAD
	INIT();

	//Camera Class
	Camera camera(window, glm::vec3(0.0f, 0.0f, 3.0f), -90.0f, 0.0f, SCR_WIDTH / 2, SCR_HEIGHT / 2);
	gCamera = &camera;//update the global camera variable

	//#######################################################################################################################
	//load the model here
	Shader ourShader("Shaders/water.vs", "Shaders/water.fs");
	Model ourModel("resources/plane/plane.obj");

	//load texture
	ourShader.use();
	Texture texture_color("Assets/water.jpg",false,false);
	Texture texture_diffuse("Assets/water_color.jpg", true, false);//diffuse
	Texture texture_specular("Assets/water_specular.jpg", false, false);//specular
	Texture texture_normal("Assets/water_normal.jpg", false, false);//normal
	Texture texture_ambience("Assets/water_ambience.jpg", false, false);//ambience
	//activate the textures
	texture_color.activate(ourShader.ID, "texture_color", 0);
	texture_diffuse.activate(ourShader.ID, "texture_diffuse", 1);
	texture_specular.activate(ourShader.ID, "texture_specular", 2);
	texture_normal.activate(ourShader.ID, "texture_normal", 3);
	texture_ambience.activate(ourShader.ID, "texture_ambience", 4);
	
	//set projection for 3D MODEL
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	ourShader.use();
	glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	//render Loop
	glEnable(GL_DEPTH_TEST);//enable depth testing
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//enable drawing in wireframe mode
	//glEnable(GL_CULL_FACE);
	while (!glfwWindowShouldClose(window)) {
		//get time
		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		//process user input
		processInput(window,deltaTime);

		//all render stuff goes here
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//Update view Matrix
		glm::mat4 view = glm::lookAt(gCamera->position, gCamera->position + gCamera->front, gCamera->up);
		//set view for 3D MODEL
		ourShader.use();
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//set the Time Uniform for 3d MODEL
		glUniform1f(glGetUniformLocation(ourShader.ID, "time"), (float)glfwGetTime());

		//update and render 3D MODEL
		ourShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		//set Color Uniforms
		glUniform3f(glGetUniformLocation(ourShader.ID, "lightColor"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(ourShader.ID, "waterColor"), 0.0235f, 0.2588f, 0.45f);
		glUniform3f(glGetUniformLocation(ourShader.ID, "lightPosition"),9.0f,1.5f,9.0f);
		glUniform3fv(glGetUniformLocation(ourShader.ID, "cameraPosition"),1,glm::value_ptr(camera.position));
		//render
		ourModel.Draw(ourShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//exit
	glfwTerminate();
	return;
}
