/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: main.cpp
Purpose: Assignment 1 graphics project reading several obj files
Language: MSVC C++
Platform: VS2017, Windows
Project: jungdae.chur_CS300_1
Author: Charlie Jung, jungdae.chur
Creation date: 9/28/2018
End Header --------------------------------------------------------*/
#include "Shader.h"
#include "GLFW\glfw3.h"
#include "Render.h"
#include "Light.h"
#include "Camera.h"
#include "imgui-master\imgui.h"
#include "imgui-master\imgui_impl_glfw.h"
#include "imgui-master\imgui_impl_opengl3.h"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

#include <vector>

// Global variables
int width = 1280, height = 800;
float deltaTime, lastFrame = 0.f;
float aspect = float(width) / float(height);
float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;
glm::vec3 middlePoint = glm::vec3(0, 0, 0);

// Set camera's position
Camera camera(glm::vec3(0.0f, 2.f, 5.0f));

// Toogler
int Key_Status = 0;
int isMove = 0;
int Shader_Status = 1;
int YourPick = 0;
int ScenarioNum = 1;

// Helper function
void ProcessInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.ProcessKeyboard(LOOK_RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.ProcessKeyboard(LOOK_LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.ProcessKeyboard(LOOK_UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.ProcessKeyboard(LOOK_DOWN, deltaTime);
}
void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	UNREFERENCED_PARAMETER(window);
	float f_xpos = float(xpos);
	float f_ypos = float(ypos);

	if (firstMouse)
	{
		lastX = f_xpos;
		lastY = f_ypos;
		firstMouse = false;
	}

	float xoffset = f_xpos - lastX;
	float yoffset = lastY - f_ypos;

	lastX = f_xpos;
	lastY = f_ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(xoffset);
	camera.ProcessMouseScroll(float(yoffset));
}
void FrameBufferSizeCallback(GLFWwindow* window, int _width, int _height)
{
	UNREFERENCED_PARAMETER(window);
	glViewport(0, 0, _width, _height);
}
int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(width, height, "cs300_jungdae.chur_assg", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Render object_center;
	object_center.position = { 0, 0, 0};
	object_center.color = { 0.2f, 0.3f, 0.3f };
	object_center.scale = { 1.f, 1.f, 1.f };

	Render objects_around[16];
	for (int i = 0; i < 16; ++i)
	{
		objects_around[i].position = { 0, 0, 0 };
		objects_around[i].color = { 0.0f, 0.0f, 0.0f };
		objects_around[i].scale = { 0.1f, 0.1f, 0.1f };
	}
	Render orbit_;
	orbit_.position = { 2.f, 0, 0 };
	orbit_.color = { 1.f, 1.f, 0.f };
	orbit_.scale = { 1.f, 1.f, 1.f };
	Render plane;
	plane.position = { 0.f, -1.f, 0 };
	plane.color = { .2f, .5f, .7f };
	plane.scale = { 1.f, 1.f, 1.f };

	const char* path = "models\\sphere_high_poly.obj";
	bool isChanged = false;
	bool isInitialized = false; // To draw once

	if (!objects_around[0].loadOBJ("models\\sphere_mid_poly.obj", middlePoint, false))
	{
		std::cout << "Failed to read objects_around OBJ file!" << std::endl;
		return -1;
	}
	for (int i = 1; i < 16; ++i)
	{
		objects_around[i].obj_vertices = objects_around[0].obj_vertices;
		objects_around[i].obj_indices = objects_around[0].obj_indices;
	}

	if (!object_center.loadOBJ(path, middlePoint, true))
	{
		std::cout << "Failed to read object_center OBJ file!" << std::endl;
		return -1;
	}
	if (!plane.loadOBJ("models\\plane_low_poly.obj", middlePoint, false))
	{
		std::cout << "Failed to read object_center OBJ file!" << std::endl;
		return -1;
	}
	// Lighting
	Shader shader(GL_FALSE, Shader::S_PH_LIGHT);
	Shader shader_phong_lighting(GL_FALSE, Shader::S_PH_LIGHT);
	Shader shader_phong_shading(GL_FALSE, Shader::S_PH_SHADING);
	Shader shader_blinn_shading(GL_FALSE, Shader::S_BLINN_SHADING);
	Shader shader_around(GL_FALSE, Shader::S_AROUND);

	// Normal Shaders
	Shader shader_vertexNormal(GL_TRUE, Shader::N_VERTEX);
	Shader shader_faceNormal(GL_TRUE, Shader::N_FACE);

	for (int i = 0; i < 16; ++i)
	{
		objects_around[i].Describe(objects_around[i].obj_vertices, objects_around[i].obj_indices, objects_around[i].textureUV);
	}
	if (!object_center.loadPPM("models\\metal_roof_diff_512x512.ppm", object_center.g_diff_data))
	{
		std::cout << "No PPM diffuse file opened" << std::endl;
	}

	if (!object_center.loadPPM("models\\metal_roof_spec_512x512.ppm", object_center.g_spec_data))
	{
		std::cout << "No PPM specular file opened" << std::endl;
	}
	object_center.Describe(object_center.obj_vertices, object_center.obj_indices, object_center.textureUV);

	object_center.DescribeTexture();

	plane.Describe(plane.obj_vertices, plane.obj_indices, plane.textureUV);

	glm::vec3 pos[16] = {glm::vec3(0,0,0)};
	glm::vec3 orbit_pos = glm::vec3(2.6 * sinf(1) , 0, 2.6 * cosf(1));
	float theta = 22.5f;
	for (unsigned i = 0; i < 16; ++i)
	{
		pos[i].x = 2.5f * sinf(i * glm::radians(theta));
		pos[i].z = 2.5f * cosf(i * glm::radians(theta));
	}
	/* Orbit trace */
	unsigned orbit_vao, orbit_vbo;
	glGenVertexArrays(1, &orbit_vao);
	glGenBuffers(1, &orbit_vbo);

	/* Light Info */
	Light directionalLight;
	directionalLight.num_spheres = 1;
	directionalLight.material_ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	Light pointLight;
	pointLight.constant = 1.0f;
	pointLight.linear = 0.09f;
	pointLight.quadratic = 0.032f;
	pointLight.num_spheres = 1;
	pointLight.material_ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	Light spotLight;
	spotLight.constant = 1.0f;
	spotLight.linear = 0.09f;
	spotLight.quadratic = 0.032f;
	spotLight.num_spheres = 1;
	spotLight.material_ambient = glm::vec3(0.1f, 0.1f, 0.1f);

	/* Default Light */
	Light Defaultlight;
	Defaultlight.constant = 1.0f;
	Defaultlight.linear = 0.09f;
	Defaultlight.quadratic = 0.032f;
	Defaultlight.num_spheres = 1;
	Defaultlight.material_ambient = glm::vec3(0.1f, 0.1f, 0.1f);

	for (int i = 0; i < 16; ++i)
	{
		directionalLight.color[i] = { 0.2f, 0.5f, 0.8f };
		directionalLight.ambient[i] = 0.1f;
		directionalLight.diffuse[i] = 0.7f;
		directionalLight.specular[i] = 0.7f;
		directionalLight.m_type[i] = Light::N_DIRECTIONAL;

		pointLight.color[i] = { 0.5f, 0.2f, 0.1f };
		pointLight.ambient[i] = 0.1f;
		pointLight.specular[i] = 1.0f;
		pointLight.diffuse[i] = 0.7f;
		pointLight.m_type[i] = Light::N_POINT;

		spotLight.color[i] = { 0.3f, 0.6f, 0.2f };
		spotLight.ambient[i] = 0.1f;
		spotLight.specular[i] = 1.f;
		spotLight.diffuse[i] = 0.9f;
		spotLight.cutOff[i] = glm::cos(glm::radians(7.5f));
		spotLight.outercutOff[i] = glm::cos(glm::radians(15.f));
		spotLight.fallOff[i] = 1.f;
		spotLight.m_type[i] = Light::N_SPOT;

		Defaultlight.color[i] = { 0.5f, 0.2f, 0.1f };
		Defaultlight.ambient[i] = 0.1f;
		Defaultlight.specular[i] = 1.0f;
		Defaultlight.diffuse[i] = 0.7f;
		Defaultlight.cutOff[i] = glm::cos(glm::radians(7.5f));
		Defaultlight.outercutOff[i] = glm::cos(glm::radians(15.f));
		Defaultlight.fallOff[i] = 1.f;
		Defaultlight.m_type[i] = Light::N_POINT;
	}

	bool show_demo_window = false;
	bool second_window = true;
	bool third_window = true;
	bool fourth_window = true;
	bool fifth_window = false;
	bool sixth_window = true;
	bool seventh_window = true;
	bool eighth_window = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	float angle = 0.f;
	float orbit_speed = 0.005f;

	/* Set all position of spheres */
	for (int i = 0; i < Defaultlight.num_spheres; ++i)
	{
		if (Defaultlight.m_type[i] == Light::N_DIRECTIONAL)
			Defaultlight.Position[i] = -pos[i];
		else
			Defaultlight.Position[i] = pos[i];
	}

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		float currFrame = (float)glfwGetTime();
		deltaTime = currFrame - lastFrame;
		lastFrame = currFrame;

		// Input
		ProcessInput(window);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		{
			ImGui::Begin("GUI interface");
			ImGui::Text("imgui interface");

			/* Load several obj files by case */
			if (ImGui::Button("bunny_high_poly.obj")) // bunny high
			{
				isChanged = true;
				path = "models\\bunny_high_poly.obj";
			}
			if (ImGui::Button("cube_high_poly.obj"))  // cube high
			{
				isChanged = true;
				path = "models\\cube_high_poly.obj";
			}
			if (ImGui::Button("cube_low_poly.obj")) // cube low
			{
				isChanged = true;
				path = "models\\cube_low_poly.obj";
			}
			if (ImGui::Button("horse_high_poly.obj")) // horse high
			{
				isChanged = true;
				path = "models\\horse_high_poly.obj";
			}
			if (ImGui::Button("menger_sponge_level_1_high_poly.obj")) // menger high
			{
				isChanged = true;
				path = "models\\menger_sponge_level_1_high_poly.obj";
			}
			if (ImGui::Button("menger_sponge_level_1_low_poly.obj")) // menger low
			{
				isChanged = true;
				path = "models\\menger_sponge_level_1_low_poly.obj";
			}
			if (ImGui::Button("plane_low_poly.obj")) // plane low
			{
				isChanged = true;
				path = "models\\plane_low_poly.obj";
			}
			if (ImGui::Button("sphere_high_poly.obj")) // sphere high
			{
				isChanged = true;
				path = "models\\sphere_high_poly.obj";
			}
			if (ImGui::Button("sphere_mid_poly.obj")) // sphere high
			{
				isChanged = true;
				path = "models\\sphere_mid_poly.obj";
			}
			if (ImGui::Button("teapot_mid_poly.obj")) // teapot mid
			{
				isChanged = true;
				path = "models\\teapot_mid_poly.obj";
			}
			ImGui::End();
		}
		if (second_window)
		{
			ImGui::Begin("Normal Windows", &second_window);
			ImGui::Text("Which normal to draw?");

			if (ImGui::Button("Vertex Normal"))
				Key_Status = 1;

			if (ImGui::Button("Face Normal"))
				Key_Status = 2;

			if (ImGui::Button("Clear Normal"))
				Key_Status = 0;

			ImGui::End();
		}
		if (third_window)
		{
			ImGui::Begin("Shader Mode", &third_window);
			ImGui::Text("Which Light do you want?");

			if (ImGui::Button("Phong Lighting"))
				Shader_Status = 0;

			if (ImGui::Button("Phong Shading"))
				Shader_Status = 1;

			if (ImGui::Button("Blinn Shading"))
				Shader_Status = 2;
			ImGui::End();
		}
		
		if (fourth_window)
		{
			ImGui::Begin("Light Mode", &fourth_window);
			if (ImGui::Button("Rotate / Stop"))
				++isMove;
			if (ImGui::TreeNode("Light Controller"))
			{
				ImGui::Text("Pick one sphere to control");
				for (int i = 0; i < Defaultlight.num_spheres; ++i)
				{
					std::string s = std::to_string(i);
					char const* pchar = s.c_str();
					if (ImGui::SmallButton(pchar))
					{
						fifth_window = true;
						YourPick = i;
						objects_around[i].color = Defaultlight.color[YourPick];
					}
				}
				ImGui::TreePop();
			}
			if (ImGui::Button("Add 1 sphere"))
			{
				if (Defaultlight.num_spheres < 16)
				{
					++Defaultlight.num_spheres;
					theta = 360.f / Defaultlight.num_spheres;
					isMove = 1;
					objects_around[Defaultlight.num_spheres - 1].color = Defaultlight.color[YourPick];
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove 1 sphere"))
			{
				if (Defaultlight.num_spheres > 1)
				{
					--Defaultlight.num_spheres;
					theta = 360.f / Defaultlight.num_spheres;
					isMove = 1;
					objects_around[Defaultlight.num_spheres - 1].color = Defaultlight.color[YourPick];
				}
			}
			if (ImGui::Button("Point lights"))
			{
				if (ScenarioNum == 1)
				{
					Defaultlight = pointLight;
				}
				else if (ScenarioNum == 2)
				{
					Defaultlight = pointLight;
					Defaultlight.m_type[YourPick] = Light::N_POINT;
				}
				else
				{
					Defaultlight.m_type[YourPick] = Light::N_POINT;
				}
			}
			if (ImGui::Button("Directional lights"))
			{
				if (ScenarioNum == 1)
				{
					Defaultlight = directionalLight;
				}
				if (ScenarioNum == 2)
				{
					Defaultlight = directionalLight;
					Defaultlight.m_type[YourPick] = Light::N_DIRECTIONAL;
				}
				else
				{
					Defaultlight.m_type[YourPick] = Light::N_DIRECTIONAL;
				}
			}
			if (ImGui::Button("Spotlights"))
			{
				if (ScenarioNum == 1)
				{
					Defaultlight = spotLight;
				}
				if (ScenarioNum == 2)
				{
					Defaultlight = spotLight;
					Defaultlight.m_type[YourPick] = Light::N_SPOT;
				}
				else
				{
					Defaultlight.m_type[YourPick] = Light::N_SPOT;
				}
			}
			ImGui::End();
		}
		if (fifth_window)
		{
			ImGui::Begin("Light Color Mode", &fifth_window);
			ImGui::Text("Which Light do you want?");

			ImGui::SliderFloat("light color R", &Defaultlight.color[YourPick].x, 0.f, 1.f, "color_R", 1.f);
			ImGui::SliderFloat("light color G", &Defaultlight.color[YourPick].y, 0.f, 1.f, "color_G", 1.f);
			ImGui::SliderFloat("light color B", &Defaultlight.color[YourPick].z, 0.f, 1.f, "color_B", 1.f);

			ImGui::SliderFloat("Material ambient value R", &Defaultlight.material_ambient.x, 0.f, .7f, "ambient_R", 1.f);
			ImGui::SliderFloat("Material ambient value G ", &Defaultlight.material_ambient.y, 0.f, .7f, "ambient_G", 1.f);
			ImGui::SliderFloat("Material ambient value B", &Defaultlight.material_ambient.z, 0.f, .7f, "ambient_B", 1.f);

			if(Defaultlight.m_type[YourPick] == Light::N_SPOT)
			{
				ImGui::SliderFloat("cutOff", &Defaultlight.cutOff[YourPick], 0.f, 15.f, "cutOff", 1.f);
				ImGui::SliderFloat("outer cutOff", &Defaultlight.outercutOff[YourPick], 0.f, 25.f, "outer cutOff", 1.f);
				ImGui::SliderFloat("fallOff", &Defaultlight.fallOff[YourPick], 0.2f, 30.f, "fall cutOff", 1.f);
			}

			ImGui::End();
		}
		if (sixth_window)
		{
			ImGui::Begin("Three Scenarios", &sixth_window);
			ImGui::Text("Which Scenario do you want?");
			if (ImGui::Button("Scenario 1"))
			{
				ScenarioNum = 1;
				Defaultlight.num_spheres = 16;
				theta = 360.f / Defaultlight.num_spheres;
				for (int i = 0; i < Defaultlight.num_spheres; ++i)
				{
					Defaultlight.color[i] = glm::vec3(0.6, 0.2f, 0.5f);
				}
				isMove = 1;
			}
			if (ImGui::Button("Scenario 2"))
			{
				ScenarioNum = 2;
				Defaultlight.num_spheres = 12;
				theta = 360.f / Defaultlight.num_spheres;
				for (int i = 0; i < Defaultlight.num_spheres; ++i)
				{
					if(i < 4)
						Defaultlight.color[i] = glm::vec3((i + 1) * 0.2f, 0.f, 0.f);
					if(i < 8 && i > 4)
						Defaultlight.color[i] = glm::vec3(0.f, (i%4) * 0.2f, 0.f);
					if(i > 8 && i < 12)
						Defaultlight.color[i] = glm::vec3(0.f, 0.f, (i/4) * 0.2f);
				}
				isMove = 1;
			}
			if (ImGui::Button("Scenario 3"))
			{
				ScenarioNum = 3;
				Defaultlight.num_spheres = 6;
				theta = 360.f / Defaultlight.num_spheres;
				for (int i = 0; i < Defaultlight.num_spheres; ++i)
				{
					if (i % 3 == 0)
					{
						Defaultlight.m_type[i] = Light::N_POINT;
						Defaultlight.color[i] = glm::vec3((i + 1) * 0.3f, 0.2f, (i + 1) * 0.2f);
					}
					else if (i % 3 == 1)
					{
						Defaultlight.m_type[i] = Light::N_DIRECTIONAL;
						Defaultlight.color[i] = glm::vec3((i + 1) * 0.12f, (i + 1) * 0.09f, 0.f);
					}
					else if (i % 3 == 2)
					{
						Defaultlight.m_type[i] = Light::N_SPOT;
						Defaultlight.color[i] = glm::vec3(0.1f, (i + 1) * 0.05f, (i + 1) * 0.17f);
					}
				}
				isMove = 1;
			}
			ImGui::End();
		}
		if (seventh_window)
		{
			ImGui::Begin("Texture projection shape", &seventh_window);
			ImGui::Text("Which Texture projection shape do you want?");
			if (ImGui::Button("Planar by Vertex"))
			{
				object_center.whichProjection = 0;
				object_center.calculateByNormal = 0;
			}
			ImGui::SameLine();
			if (ImGui::Button("Planar by Normal"))
			{
				object_center.whichProjection = 0;
				object_center.calculateByNormal = 1;
			}
			if (ImGui::Button("Cylindrical by Vertex"))
			{
				object_center.whichProjection = 1;
				object_center.calculateByNormal = 0;
			}
			ImGui::SameLine();
			if (ImGui::Button("Cylindrical by Normal"))
			{
				object_center.whichProjection = 1;
				object_center.calculateByNormal = 1;
			}
			if (ImGui::Button("Sphere by Vertex"))
			{
				object_center.whichProjection = 2;
				object_center.calculateByNormal = 0;
			}
			ImGui::SameLine();
			if (ImGui::Button("Sphere by Normal"))
			{
				object_center.whichProjection = 2;
				object_center.calculateByNormal = 1;
			}
			ImGui::End();
		}
		if (eighth_window)
		{
			ImGui::Begin("Reload Shader", &seventh_window);
			ImGui::Text("Do you want to reload the shader?");
			if (ImGui::Button("Reload shader"))
			{
				if (shader.UseGeometry)
				{
					if (shader.m_type == Shader::N_VERTEX)
						shader.CreateShader("ShaderCodes\\VertexShader_Normal.vs",
							"ShaderCodes\\FragmentShader_Normal.fs",
							"ShaderCodes\\GeometryShader_VertexNormal.gs");
					else if (shader.m_type == Shader::N_FACE)
						shader.CreateShader("ShaderCodes\\VertexShader_Normal.vs",
							"ShaderCodes\\FragmentShader_Normal.fs",
							"ShaderCodes\\GeometryShader_FaceNormal.gs");
				}
				else
				{
					// Default is Phone lighting
					if ((shader.m_type == Shader::S_PH_LIGHT) || (shader.m_type == Shader::N_NONE))
						shader.CreateShader("ShaderCodes\\Phong_Lighting.vs",
							"ShaderCodes\\Phong_Lighting.fs", nullptr);
					else if (shader.m_type == Shader::S_PH_SHADING)
						shader.CreateShader("ShaderCodes\\Phong_Shading.vs",
							"ShaderCodes\\Phong_Shading.fs", nullptr);
					else if (shader.m_type == Shader::S_BLINN_SHADING)
						shader.CreateShader("ShaderCodes\\Blinn_Shading.vs",
							"ShaderCodes\\Blinn_Shading.fs", nullptr);
					else if (shader.m_type == Shader::S_AROUND)
						shader.CreateShader("ShaderCodes\\Phong_Shading.vs",
							"ShaderCodes\\FragmentShader_Around.fs", nullptr);
				}
			}
			ImGui::End();
		}
		/* Load information changed from imgui */
		if (isChanged)
		{
			object_center.obj_vertices.clear();
			object_center.obj_indices.clear();
			object_center.textureUV.clear();
			object_center.loadOBJ(path, middlePoint, true);
			object_center.Describe(object_center.obj_vertices, object_center.obj_indices, object_center.textureUV);
			isChanged = false;
		}
		if (Shader_Status == 0)
		{
			shader = shader_phong_lighting;
		}
		else if (Shader_Status == 1)
		{
			shader = shader_phong_shading;
		}
		else if (Shader_Status == 2)
		{
			shader = shader_blinn_shading;
		}

		camera.Update(&shader);

		Defaultlight.Update(&shader);

		/* Draw Vertex Normal */
		if (Key_Status == 1)
		{
			shader_vertexNormal.Use();
			object_center.Rendering(&camera, &shader_vertexNormal, aspect, GL_TRIANGLES, object_center.position, object_center.color);
		}
		else if (Key_Status == 2)
		{
			/* Draw FaceNormal */
			shader_faceNormal.Use();
			object_center.Rendering(&camera, &shader_faceNormal, aspect, GL_TRIANGLES, object_center.position, object_center.color);
		}
		/* Draw around objects */
		shader_around.Use();

		for (int i = 0; i < Defaultlight.num_spheres; ++i)
		{
			objects_around[i].position = pos[i];
			objects_around[i].color = Defaultlight.color[i];
			objects_around[i].Rendering(&camera, &shader_around, aspect, GL_TRIANGLES, objects_around[i].position, objects_around[i].color);
		}

		shader.Use();
		/* Draw Center object */
		plane.Rendering(&camera, &shader, aspect, GL_TRIANGLES, plane.position, plane.color);
		object_center.SendTextureInfo(&shader);
		object_center.Rendering(&camera, &shader, aspect, GL_TRIANGLES, object_center.position, object_center.color);
		
		/* Click to move and one more click to stop */
		for (int i = 0; i < Defaultlight.num_spheres; ++i)
		{
			if (isMove % 2)
			{
				pos[i].x = 2.5f * sinf(i * glm::radians(theta) + angle);
				pos[i].z = 2.5f * cosf(i * glm::radians(theta) + angle);

				angle += (orbit_speed / Defaultlight.num_spheres);
			}
			Defaultlight.Position[i] = pos[i];
			if (ScenarioNum != 3)
			{
				Defaultlight.m_type[i] = Defaultlight.m_type[YourPick];
				if(ScenarioNum == 1)
					Defaultlight.color[i] = Defaultlight.color[YourPick];
			}
		}

		for (unsigned i = 0; i < 360; ++i)
		{
			float f_index = (float)i;
			orbit_pos.x = 2.5f * sinf(glm::radians(f_index + 1));
			orbit_pos.z = 2.5f * cosf(glm::radians(f_index + 1));
			orbit_.obj_vertices.push_back(orbit_pos);
		}
		if (!isInitialized)
		{
			glBindVertexArray(orbit_vao);
			glBindBuffer(GL_ARRAY_BUFFER, orbit_vbo);
			glBufferData(GL_ARRAY_BUFFER, orbit_.obj_vertices.size() * sizeof(glm::vec3), &orbit_.obj_vertices[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			isInitialized = true;
		}
		glBindVertexArray(orbit_vao);
		glDrawArrays(GL_LINES, (GLuint)orbit_.obj_vertices[0].x, (GLsizei)orbit_.obj_vertices.size());

		ImGui::Render();

		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}
