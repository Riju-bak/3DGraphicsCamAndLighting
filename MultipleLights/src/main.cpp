#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "awesomeface.h"
#include "camera.h"
#include "ebo.h"
#include "shader.h"
#include "vao.h"
#include "Texture2D.h"


//Defining variables inside unnamed namespace will make them global only to main.cpp
namespace
{
	constexpr int SCREEN_WIDTH = 800;
	constexpr int SCREEN_HEIGHT = 600;
	Camera camera(FLY);
	float deltaTime = 0.0f; // Time between current frame and previous frame
	float previousFrame = 0.0f; // Time of previous frame

	//These values need to be adjusted
	float lastMouseX = 800.0f;
	float lastMouseY = 300.0f;

	bool firstMouse = true;

	float vertices[] = {
		// positions					// normals						// texture coords
		-0.5f,	-0.5f,	-0.5f,			0.0f,	0.0f,	-1.0f,			0.0f,	0.0f,
		0.5f,	-0.5f,	-0.5f,			0.0f,	0.0f,	-1.0f,			1.0f,	0.0f,
		0.5f,	0.5f,	-0.5f,			0.0f,	0.0f,	-1.0f,			1.0f,	1.0f,
		0.5f,	0.5f,	-0.5f,			0.0f,	0.0f,	-1.0f,			1.0f,	1.0f,
		-0.5f,	0.5f,	-0.5f,			0.0f,	0.0f,	-1.0f,			0.0f,	1.0f,
		-0.5f,	-0.5f,	-0.5f,			0.0f,	0.0f,	-1.0f,			0.0f,	0.0f,

		-0.5f,	-0.5f,	0.5f,			0.0f,	0.0f,	1.0f,			0.0f,	0.0f,
		0.5f,	-0.5f,	0.5f,			0.0f,	0.0f,	1.0f,			1.0f,	0.0f,
		0.5f,	0.5f,	0.5f,			0.0f,	0.0f,	1.0f,			1.0f,	1.0f,
		0.5f,	0.5f,	0.5f,			0.0f,	0.0f,	1.0f,			1.0f,	1.0f,
		-0.5f,	0.5f,	0.5f,			0.0f,	0.0f,	1.0f,			0.0f,	1.0f,
		-0.5f,	-0.5f,	0.5f,			0.0f,	0.0f,	1.0f,			0.0f,	0.0f,

		-0.5f,	0.5f,	0.5f,			-1.0f,	0.0f,	0.0f,			1.0f,	0.0f,
		-0.5f,	0.5f,	-0.5f,			-1.0f,	0.0f,	0.0f,			1.0f,	1.0f,
		-0.5f,	-0.5f,	-0.5f,			-1.0f,	0.0f,	0.0f,			0.0f,	1.0f,
		-0.5f,	-0.5f,	-0.5f,			-1.0f,	0.0f,	0.0f,			0.0f,	1.0f,
		-0.5f,	-0.5f,	0.5f,			-1.0f,	0.0f,	0.0f,			0.0f,	0.0f,
		-0.5f,	0.5f,	0.5f,			-1.0f,	0.0f,	0.0f,			1.0f,	0.0f,

		0.5f,	0.5f,	0.5f,			1.0f,	0.0f,	0.0f,			1.0f,	0.0f,
		0.5f,	0.5f,	-0.5f,			1.0f,	0.0f,	0.0f,			1.0f,	1.0f,
		0.5f,	-0.5f,	-0.5f,			1.0f,	0.0f,	0.0f,			0.0f,	1.0f,
		0.5f,	-0.5f,	-0.5f,			1.0f,	0.0f,	0.0f,			0.0f,	1.0f,
		0.5f,	-0.5f,	0.5f,			1.0f,	0.0f,	0.0f,			0.0f,	0.0f,
		0.5f,	0.5f,	0.5f,			1.0f,	0.0f,	0.0f,			1.0f,	0.0f,

		-0.5f,	-0.5f,	-0.5f,			0.0f,	-1.0f,	0.0f,			0.0f,	1.0f,
		0.5f,	-0.5f,	-0.5f,			0.0f,	-1.0f,	0.0f,			1.0f,	1.0f,
		0.5f,	-0.5f,	0.5f,			0.0f,	-1.0f,	0.0f,			1.0f,	0.0f,
		0.5f,	-0.5f,	0.5f,			0.0f,	-1.0f,	0.0f,			1.0f,	0.0f,
		-0.5f,	-0.5f,	0.5f,			0.0f,	-1.0f,	0.0f,			0.0f,	0.0f,
		-0.5f,	-0.5f,	-0.5f,			0.0f,	-1.0f,	0.0f,			0.0f,	1.0f,

		-0.5f,	0.5f,	-0.5f,			0.0f,	1.0f,	0.0f,			0.0f,	1.0f,
		0.5f,	0.5f,	-0.5f,			0.0f,	1.0f,	0.0f,			1.0f,	1.0f,
		0.5f,	0.5f,	0.5f,			0.0f,	1.0f,	0.0f,			1.0f,	0.0f,
		0.5f,	0.5f,	0.5f,			0.0f,	1.0f,	0.0f,			1.0f,	0.0f,
		-0.5f,	0.5f,	0.5f,			0.0f,	1.0f,	0.0f,			0.0f,	0.0f,
		-0.5f,	0.5f,	-0.5f,			0.0f,	1.0f,	0.0f,			0.0f,	1.0f
	};

	int stride = 8 * sizeof(float);

	int vertexLayout = 0;
	int vertexOffset = 0;

	int normalLayout = 1;
	int normalOffset = 3 * sizeof(float);

	int textureLayout = 2;
	int textureOffset = 6 * sizeof(float);

	unsigned int indices[] = {
			0, 1, 2,
			0, 2, 3,
	};

	glm::vec3 lampPos{ 0.0f, 0.0f, -2.0f };

	//Translation vectors for different cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f),
		lampPos
	};
	int numCubes = sizeof(cubePositions) / (3 * sizeof(float));

	glm::vec3 lightColor{ 1.0f, 1.0f, 1.0f };
	glm::vec3 lightDirection{ 0, 0, 10.0f };
	glm::vec3 lightAmbient = 0.05f * lightColor;
	glm::vec3 lightDiffuse = 0.5f * lightColor;
	glm::vec3 lightSpecular = lightColor;
	float lightAttenuationConstant = 1.0f;
	float lightAttenuationLinear = 0.027f;
	float lightAttenuationQuadratic = 0.0028f;
	float materialShininess = 2000.0f;
	float spotlightCutoff = glm::cos(glm::radians(8.5f));
	float spotlightOuterCutoff = glm::cos(glm::radians(12.5f));
	glm::vec3 dirLightDirection{1.0f, 0.0f, 1.0f};
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	const float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - previousFrame;
	previousFrame = currentFrame;
	camera.speed = 2.0f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(MOVE_FORWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(MOVE_BACKWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(STRAFE_LEFT);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(STRAFE_RIGHT);
	}
}

void LoadVertexDataIntoGPUMemory(VAO& vao, VAO& lampVao, float vertices[], int verticesSize, unsigned int indices[], int indicesSize)
{
	//Generate and bind the VBO
	VBO vbo = VBO(vertices, verticesSize);

	//Generate and bind the EBO
	// EBO ebo = EBO(indices, indicesSize);

	vao.LinkVBO(vbo, vertexLayout, stride, vertexOffset);
	vao.LinkVBO(vbo, normalLayout, stride, normalOffset);
	vao.LinkVBO(vbo, textureLayout, stride, textureOffset, true);

	lampVao.LinkVBO(vbo, vertexLayout, stride, vertexOffset);
}


void InitializeGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* CreateWindow(int screenWidth, int screenHeight, const char* title)
{
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, title, nullptr, nullptr);
	return window;
}

void SetWindowRenderProperties(GLFWwindow* window)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	//State setting function
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			//State using function
}


bool LoadGlad()
{
	// GLAD manages function pointers for OpenGL so we
	// want to initialize GLAD before we call any OpenGL function
	return gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
}


void DoTransformations(Shader& shader, glm::vec3 cubePositions[], int i)
{
	shader.Activate();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, cubePositions[i]);
	model = glm::rotate(model, (i % 3 && i != 1) ? 0 : (static_cast<float>(i) + 1.0f) * sin(static_cast<float>(glfwGetTime())), glm::vec3(0.5f, 1.0f, 0.0f));

	glm::mat4 view = camera.GetViewMatrix();

	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
		static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f,
		100.0f);

	shader.SetUniformMatrix4fv("model", model);
	shader.SetUniformMatrix4fv("view", view);
	shader.SetUniformMatrix4fv("projection", projection);
}


void RenderLamp(VAO& lampVao, Shader& shader, Shader& woodenContainerShader)
{
	lampVao.Bind();
	shader.Activate();
	double angle = glfwGetTime() / 2.0;
	float lightSourceOrbitRadius = 2.0f;
	glm::mat4 model = glm::mat4(1.0f);
	lampPos.x = lightSourceOrbitRadius * static_cast<float>(cos(angle));
	lampPos.z = lightSourceOrbitRadius * static_cast<float>(sin(angle));

	woodenContainerShader.SetUniformVec3("lampPos", lampPos);	//Update lampPos;
	model = glm::translate(model, lampPos);
	model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube

	glm::mat4 view = camera.GetViewMatrix();

	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
		static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f,
		100.0f);

	shader.SetUniformMatrix4fv("model", model);
	shader.SetUniformMatrix4fv("view", view);
	shader.SetUniformMatrix4fv("projection", projection);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	lampVao.UnBind();
}


void SetSpotLightUniformVals(Shader& shader)
{
	shader.SetUniformVec3("spotLight.ambient", lightAmbient);
	shader.SetUniformVec3("spotLight.diffuse", lightDiffuse);
	shader.SetUniformVec3("spotLight.specular", lightSpecular);
	shader.SetUniformf("spotLight.constant", lightAttenuationConstant);
	shader.SetUniformf("spotLight.linear", lightAttenuationLinear);
	shader.SetUniformf("spotLight.quadratic", lightAttenuationQuadratic);
	shader.SetUniformVec3("spotLight.position", camera.position);
	shader.SetUniformVec3("spotLight.direction", camera.direction);
	shader.SetUniformf("spotLight.cutoff", spotlightCutoff);
	shader.SetUniformf("spotLight.outerCutoff", spotlightOuterCutoff);
}

void SetDirLightUniformVals(Shader& shader)
{
	shader.SetUniformVec3("dirLight.direction", dirLightDirection);
	shader.SetUniformVec3("dirLight.ambient", lightAmbient);
	shader.SetUniformVec3("dirLight.diffuse", lightDiffuse);
	shader.SetUniformVec3("dirLight.specular", lightSpecular);
}

void SetMaterialUniformVals(Shader& shader)
{
	shader.SetUniformi("material.diffuse", 0);
	shader.SetUniformi("material.specular", 1);
	shader.SetUniformi("material.emission", 2);
	shader.SetUniformf("material.shininess", materialShininess);
}


void Run(GLFWwindow* window)
{
	//TODO: See if the shaders can be initialized in the anonymous namespace
	Shader woodenContainerShader = Shader("resources/shaders/vertex.glsl", "resources/shaders/fragment.glsl");
	Shader lampShader = Shader("resources/shaders/lampVertex.glsl", "resources/shaders/lampFragment.glsl");

	VAO vao;
	VAO lampVao;
	LoadVertexDataIntoGPUMemory(vao, lampVao, vertices, sizeof(vertices), indices, sizeof(indices));
	Texture2D diffuseMap = Texture2D("resources/textures/woodenContainerMetalBorder.png");
	Texture2D specularMap = Texture2D("resources/textures/woodenContainerMetalBorderSpecular.png");
	Texture2D emissionMap = Texture2D("resources/textures/matrix.jpg");
	// Texture2D awesomefaceTexture = Texture2D("resources/textures/awesomeface.png");
	// unsigned int awesomeFaceTextureId = SetupAwesomeFace();

	while (!glfwWindowShouldClose(window))
	{
		SetWindowRenderProperties(window);

		// input
		processInput(window);

		//rendering commands here

		woodenContainerShader.Activate();
		SetSpotLightUniformVals(woodenContainerShader);
		SetDirLightUniformVals(woodenContainerShader);
		SetMaterialUniformVals(woodenContainerShader);
		woodenContainerShader.SetUniformVec3("viewPos", camera.position);


		//Don't forget to bind the textures before calling glDrawElements
		diffuseMap.Bind(GL_TEXTURE0);
		specularMap.Bind(GL_TEXTURE1);
		emissionMap.Bind(GL_TEXTURE2);

		// awesomefaceTexture.Bind(GL_TEXTURE1);
		// BindAwesomeFaceTexture(awesomeFaceTextureId, GL_TEXTURE1);


		vao.Bind(); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//TODO: Upper limit of for loop is dependent on cubePositions array, improve this
		for (int i = 0; i < numCubes - 1; ++i)
		{
			DoTransformations(woodenContainerShader, cubePositions, i);
			//TODO: Use EBO for better performance
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// RenderLamp(lampVao, lampShader, woodenContainerShader);

		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	//(xpos, ypos) is the coordinate of the mouse pointer
	camera.ProcessMouseMovement(xpos, ypos, lastMouseX, lastMouseY, firstMouse);
}

int main()
{
	InitializeGLFW();

	GLFWwindow* window = CreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MultipleLights");
	if (!window)
	{
		std::cout << "Failed to initialize window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!LoadGlad())
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, MouseCallback);

	//This will ensure that the mouse becomes hidden and gets captured when the window is on focus.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Run(window);

	glfwTerminate();

	return 0;
}
