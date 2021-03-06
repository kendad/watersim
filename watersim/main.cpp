#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

	//Activate IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//Some Variables for controling through IMGUI
	float lColors[4] = { 1.0f,1.0f,1.0f,1.0f };
	float wColors[4] = { 0.023f,0.258f,0.450f,1.0f };
	float lPosx=9.0f;  float lPosy=1.5f;  float lPosz=9.0f;
	float wSpeed = 0.9f;
	float randX = 0.1f;
	float randY = 0.1f;

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
		//set up IMGUI for rendering
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

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
		//set Uniforms
		glUniform1f(glGetUniformLocation(ourShader.ID, "speed"), wSpeed);
		glUniform1f(glGetUniformLocation(ourShader.ID, "randX"), randX);
		glUniform1f(glGetUniformLocation(ourShader.ID, "randY"), randY);
		glUniform3f(glGetUniformLocation(ourShader.ID, "lightColor"), lColors[0], lColors[1], lColors[2]);
		glUniform3f(glGetUniformLocation(ourShader.ID, "waterColor"), wColors[0], wColors[1], wColors[2]);
		glUniform3f(glGetUniformLocation(ourShader.ID, "lightPosition"),lPosx, lPosy, lPosz);
		glUniform3fv(glGetUniformLocation(ourShader.ID, "cameraPosition"),1,glm::value_ptr(camera.position));
		//render
		ourModel.Draw(ourShader);

		//render IMGUI stuff here
		ImGui::Begin("Controls");
		ImGui::ColorEdit4("Light Color", lColors);
		ImGui::ColorEdit4("Water Color", wColors);
		ImGui::SliderFloat("LightXPos", &lPosx, -30, 30);
		ImGui::SliderFloat("LightYPos", &lPosy, -10, 10);
		ImGui::SliderFloat("LightZPos", &lPosz, -30, 30);
		ImGui::SliderFloat("Speed", &wSpeed, 0, 15);
		ImGui::SliderFloat("AmplitudeX", &randX, 0, 0.4);
		ImGui::SliderFloat("AmplitudeY", &randY, 0,0.4);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//End IMGUI here
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	//exit
	glfwTerminate();
	return;
}
