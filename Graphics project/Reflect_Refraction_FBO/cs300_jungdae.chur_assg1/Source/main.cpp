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
Camera camera(glm::vec3(0.0f, .7f, 4.0f));

// Toogler
int Key_Status = 0;
int isMove = 0;
int Shader_Status = 1;
int YourPick = 0;
int ScenarioNum = 1;

const float Air = 1.000293f;
const float Hydrogen = 1.000132f;
const float Ice = 1.31f;
const float Water = 1.333f;
const float Quartz = 1.46f;
const float OliveOil = 1.47f;
const float Acrylic = 1.49f;
const float Diamond = 2.42f;
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

	std::vector<std::string> faces
	{
		"CubeMappingMode\\badomen_rt.tga",
		"CubeMappingMode\\badomen_lf.tga",
		"CubeMappingMode\\badomen_up.tga",
		"CubeMappingMode\\badomen_dn.tga",
		"CubeMappingMode\\badomen_ft.tga",
		"CubeMappingMode\\badomen_bk.tga",
	};

	Render skybox;
	skybox.position = { 0, 0, 0 };
	skybox.color = { 0.0f, 0.0f, 0.0f };
	skybox.scale = { 1.f, 1.f, 1.f };
	if (!skybox.loadOBJ("models\\cube_low_poly.obj", middlePoint, false))
	{
		std::cout << "Failed to read object_center OBJ file!" << std::endl;
		return -1;
	}
	skybox.Describe(skybox.obj_vertices, skybox.obj_indices, skybox.textureUV);
	skybox.LoadTGAFile(faces);

	Render object_center;
	object_center.position = { 0, 0, 0 };
	object_center.color = { 0.2f, 0.3f, 0.5f };
	object_center.scale = { 1.f, 1.f, 1.f };
	object_center.refractionMaterial = Air;

	int num_objects = 4;
	Render objects_around[16];
	for (int i = 0; i < 16; ++i)
	{
		objects_around[i].position = { 0, 0, 0 };
		objects_around[i].color = { 0.0f, 0.0f, 1.0f };
		objects_around[i].scale = { 0.1f, 0.1f, 0.1f };
	}

	const char* path = "models\\sphere_high_poly.obj";
	bool isChanged = false;

	if (!object_center.loadOBJ(path, middlePoint, false))
	{
		std::cout << "Failed to read object_center OBJ file!" << std::endl;
		return -1;
	}
	if (!objects_around[0].loadOBJ("models\\sphere_mid_poly.obj", middlePoint, false))
	{
		std::cout << "Failed to read objects_around OBJ file!" << std::endl;
		return -1;
	}
	for (int i = 1; i < 16; ++i)
	{
		objects_around[i].obj_vertices = objects_around[0].obj_vertices;
		objects_around[i].obj_indices = objects_around[0].obj_indices;
		objects_around[i].vertexNormals = objects_around[0].vertexNormals;
	}
	Shader shader_skybox(GL_FALSE, Shader::S_SKYBOX);
	// Lighting
	Shader shader(GL_FALSE, Shader::S_REFLECTION);
	Shader shader_phong_shading(GL_FALSE, Shader::S_PH_SHADING);
	Shader shader_around(GL_FALSE, Shader::S_AROUND);

	// Normal Shaders
	Shader shader_vertexNormal(GL_TRUE, Shader::N_VERTEX);
	Shader shader_faceNormal(GL_TRUE, Shader::N_FACE);

	object_center.Describe(object_center.obj_vertices, object_center.obj_indices, object_center.textureUV);
	object_center.lightStatus = 0;
	object_center.usingPhong = 0;
	
	for (int i = 0; i < 16; ++i)
	{
		objects_around[i].Describe(objects_around[i].obj_vertices, objects_around[i].obj_indices, objects_around[i].textureUV);
	}

	glm::vec3 pos[16] = { glm::vec3(0,0,0) };
	glm::vec3 orbit_pos = glm::vec3(2.6 * sinf(1), 0, 2.6 * cosf(1));

	float theta = 90.f; // default number of objects
	for (unsigned i = 0; i < 16; ++i)
	{
		pos[i].x = 2.5f * sinf(i * glm::radians(theta));
		pos[i].z = 2.5f * cosf(i * glm::radians(theta));
	}
	/* Orbit trace */
	unsigned orbit_vao, orbit_vbo;
	glGenVertexArrays(1, &orbit_vao);
	glGenBuffers(1, &orbit_vbo);

	Light directionalLight;
	directionalLight.num_spheres = 4;
	directionalLight.material_ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	for (int i = 0; i < 16; ++i)
	{
		directionalLight.color[i] = { 0.2f, 0.5f, 0.8f };
		directionalLight.ambient[i] = 0.1f;
		directionalLight.diffuse[i] = 0.7f;
		directionalLight.specular[i] = 0.7f;
		directionalLight.m_type[i] = Light::N_DIRECTIONAL;
	}

	for (int i = 0; i < directionalLight.num_spheres; ++i)
	{
		if (directionalLight.m_type[i] == Light::N_DIRECTIONAL)
			directionalLight.Position[i] = -pos[i];
		else
			directionalLight.Position[i] = pos[i];
	}
	float angle = 0.f;
	float orbit_speed = 0.002f;

	bool show_demo_window = false;
	bool second_window = false;
	bool third_window = true;
	bool forth_window = true;
	bool seventh_window = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	unsigned int textureColorbuffer[6];
	for (int i = 0; i < 6; ++i)
	{
		glGenTextures(1, &textureColorbuffer[i]);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureColorbuffer[i], 0);
	}

	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		float currFrame = (float)glfwGetTime();
		deltaTime = currFrame - lastFrame;
		lastFrame = currFrame;

		// Input
		ProcessInput(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
		if (object_center.lightStatus == 3)
		{
			if (third_window)
			{
				ImGui::Begin("Object Color", &third_window);
				ImGui::Text("Which color do you want?");

				ImGui::SliderFloat("color R", &object_center.color.x, 0.f, 1.f);
				ImGui::SliderFloat("color G", &object_center.color.y, 0.f, 1.f);
				ImGui::SliderFloat("color B", &object_center.color.z, 0.f, 1.f);
				ImGui::End();
			}
		}

		if (seventh_window)
		{
			ImGui::Begin("Number of around objects", &seventh_window);
			ImGui::Text("How many objects do you want?");
			if (ImGui::Button("Add object"))
			{
				if (num_objects < 16)
				{
					++num_objects;
					theta = 360.f / num_objects;
					directionalLight.num_spheres = num_objects;
				}
					
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove object"))
			{
				if (num_objects > 1)
				{
					--num_objects;
					theta = 360.f / num_objects;
					directionalLight.num_spheres = num_objects;
				}
			}
			ImGui::End();
		}
		if (forth_window)
		{
			ImGui::Begin("Refraction type", &forth_window);
			ImGui::Text("Which Refraction material do you want?");

			if (ImGui::Button("Reflection"))
				object_center.lightStatus = 0;
			ImGui::SameLine();
			if (ImGui::Button("Refraction"))
				object_center.lightStatus = 1;
			if (ImGui::Button("Reflection & Refraction"))
				object_center.lightStatus = 2;
			if (ImGui::Button("+ Phong modeling"))
				object_center.usingPhong = true;
			ImGui::SameLine();
			if (ImGui::Button("- Phong modeling"))
				object_center.usingPhong = false;
			if (object_center.lightStatus)
			{
				if (ImGui::Button("Air"))
					object_center.refractionMaterial = Air;
				if (ImGui::Button("Hydrogen"))
					object_center.refractionMaterial = Hydrogen;
				if (ImGui::Button("Ice"))
					object_center.refractionMaterial = Ice;
				if (ImGui::Button("Water"))
					object_center.refractionMaterial = Water;
				if (ImGui::Button("Quartz"))
					object_center.refractionMaterial = Quartz;
				if (ImGui::Button("OliveOil"))
					object_center.refractionMaterial = OliveOil;
				if (ImGui::Button("Acrylic"))
					object_center.refractionMaterial = Acrylic;
				if (ImGui::Button("Diamond"))
					object_center.refractionMaterial = Diamond;
				ImGui::Text("Refraction Material index");
				ImGui::SliderFloat("", &object_center.refractionMaterial, 0.f, 100.f);
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
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		/* Draw on frame buffer */
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glm::vec3 savePos = camera.position;
		float savePitch = camera.pitch, saveYaw = camera.yaw;
		
		for (int i = 0; i < 6; ++i)
		{
			camera.position = glm::vec3(0, 0, 0);
			if (i == 0)
			{
				/* Case front */
				camera.pitch = 0.f;
				camera.yaw = 0.f;
				camera.Update(&shader_skybox);
			}
			else if (i == 1)
			{
				/* Case back */
				camera.yaw = 180.f;
				camera.pitch = 0.f;
				camera.Update(&shader_skybox);
			}
			else if (i == 2)
			{
				/* Case right */
				camera.yaw = 90.f;
				camera.pitch = 0.f;
				camera.Update(&shader_skybox);
			}
			else if (i == 3)
			{
				/* Case left */
				camera.yaw = 270.f;
				camera.pitch = 0.f;
				camera.Update(&shader_skybox);
			}
			else if (i == 4)
			{
				/* Case up */
				camera.yaw = 0.f;
				camera.pitch = 90.f;
				camera.Update(&shader_skybox);
			}
			else if (i == 5)
			{
				/* Case bottom */
				camera.yaw = 0.f;
				camera.pitch = 270.f;
				camera.Update(&shader_skybox);
			}
			glDisable(GL_DEPTH_TEST);

			shader_skybox.Use();
			camera.zoom = 90.f; // fov
			skybox.SendTextureInfo(&shader_skybox, skybox.m_textures[0]);
			skybox.Rendering(&camera, &shader_skybox, 1.f, GL_TRIANGLES, skybox.position, true); // aspect = 1

			shader_around.Use();
			for (int k = 0; k < num_objects; ++k)
			{
				objects_around[k].position = pos[k];
				objects_around[k].Rendering(&camera, &shader_around, 1.f, GL_TRIANGLES, objects_around[k].position, false);
			}
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureColorbuffer[i], 0);
		}
		camera.zoom = 45.f;
		camera.position = savePos;
		camera.yaw = saveYaw;
		camera.pitch = savePitch;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDisable(GL_DEPTH_TEST);

		shader_skybox.Use();
		camera.Update(&shader_skybox);
		skybox.SendTextureInfo(&shader_skybox, skybox.m_textures[0]);
		skybox.Rendering(&camera, &shader_skybox, aspect, GL_TRIANGLES, skybox.position, true);

		glEnable(GL_DEPTH_TEST);

		shader_around.Use();
		for (int i = 0; i < num_objects; ++i)
		{
			objects_around[i].position = pos[i];
			objects_around[i].color = directionalLight.color[i];
			objects_around[i].Rendering(&camera, &shader_around, aspect, GL_TRIANGLES, objects_around[i].position, false);
		}
		/* Draw Vertex Normal */
		if (Key_Status == 1)
		{
			shader_vertexNormal.Use();
			object_center.Rendering(&camera, &shader_vertexNormal, aspect, GL_TRIANGLES, object_center.position, true);
		}
		else if (Key_Status == 2)
		{
			/* Draw FaceNormal */
			shader_faceNormal.Use();
			object_center.Rendering(&camera, &shader_faceNormal, aspect, GL_TRIANGLES, object_center.position, true);
		}
		shader.Use();
		shader.SetInt("lightStatus", object_center.lightStatus);
		shader.SetInt("isUsingPhong", object_center.usingPhong);
		if (object_center.usingPhong)
			directionalLight.Update(&shader);
		camera.Update(&shader);
		/* Draw Center object */
		object_center.SendTextureInfo(&shader, textureColorbuffer[0]);
		object_center.Rendering(&camera, &shader, aspect, GL_TRIANGLES, object_center.position, false);

		for (int i = 0; i < num_objects; ++i)
		{
			pos[i].x = 2.5f * sinf(i * glm::radians(theta) + angle);
			pos[i].z = 2.5f * cosf(i * glm::radians(theta) + angle);

			angle += (orbit_speed / (num_objects * 2));

			directionalLight.Position[i] = pos[i];
		}

		ImGui::Render();

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