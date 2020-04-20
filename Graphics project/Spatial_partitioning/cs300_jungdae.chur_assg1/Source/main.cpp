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
glm::vec3 middlePoint[20] = { glm::vec3(0, 0, 0) };

bool BoundingVolumeShape = false; // false = box, true = sphere
bool HierarchyVolumeShape = false; // false = box, true = sphere

const float PI = 4.0f*atan(1.0f);

// Set camera's position
Camera camera(glm::vec3(0.0f, 10.f, 20.0f));

// Toogler
int Key_Status = 0;
int isMove = 0;
int Shader_Status = 1;
int YourPick = 0;
int ScenarioNum = 1;
int num_object_center = 4;
int Draw_Status = 0;

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
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
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
	GLFWwindow* window = glfwCreateWindow(width, height, "cs350_jungdae.chur_assg", NULL, NULL);
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
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	glEnable(GL_DEPTH_TEST);

	Render object_center[20];

	for (int i = 0; i < num_object_center; ++i)
	{
		if (i == 0)
			object_center[i].position = glm::vec3(1.f, 5.f, 5.f);
		if (i == 1)
			object_center[i].position = glm::vec3(-3.f, 1.f, 4.f);
		if (i == 2)
			object_center[i].position = glm::vec3(3.f, 3.f, 2.f);
		if (i == 3)
			object_center[i].position = glm::vec3(7.f, 2.f, 2.f);
		if (i == 4)
			object_center[i].position = glm::vec3(4.f, 6.f, -3.f);
		if (i == 5)
			object_center[i].position = glm::vec3(3.f, 2.f, -2.f);
		
		object_center[i].color = { 1.f, 0.3f, 0.f };
		object_center[i].scale = { 1.f, 1.f, 1.f };
	}
	Render object_center_debug[20];
	for (int i = 0; i < num_object_center; ++i)
	{
		object_center_debug[i] = object_center[i];
	}
	Render object_bounding_box[20];
	for (int i = 0; i < num_object_center; ++i)
	{
		object_bounding_box[i].position = object_center[i].position;
		object_bounding_box[i].color = { 0.8f, 0.8f, 0.9f };
		object_bounding_box[i].scale = { 1.f, 1.f, 1.f };
		object_bounding_box[i].m_BVtype = Render::BoundingVolumeType::AABB;
	}
	Render object_aabb[20];
	for (int i = 0; i < num_object_center; ++i)
	{
		object_aabb[i].position = object_center[i].position;
		object_aabb[i].color = { 0.8f, 0.8f, 0.9f };
		object_aabb[i].scale = { 1.f, 1.f, 1.f };
		object_aabb[i].m_BVtype = Render::BoundingVolumeType::AABB;
	}

	Render hierarchy;
	hierarchy.position = { 0.f, 0.f, 0.f };
	hierarchy.color = { 1.0f, 0.1f, 0.6f };
	hierarchy.scale = { 1.f, 1.f, 1.f };

	Render objects_around[16];
	for (int i = 0; i < 16; ++i)
	{
		objects_around[i].position = { 0, 0, 0 };
		objects_around[i].color = { 0.0f, 0.0f, 0.0f };
		objects_around[i].scale = { 0.1f, 0.1f, 0.1f };
	}
	Render plane;
	plane.position = { 0.f, -1.f, 0 };
	plane.color = { 0.5f, 0.5f, 0.5f };
	plane.scale = { 10.f, 1.f, 10.f };

	const char* path = "models\\sphere_mid_poly.obj";
	bool isChanged = false;
	bool isHierarchyChanged = false;

	if (!objects_around[0].loadOBJ("models\\sphere_mid_poly.obj", middlePoint, 0, false))
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
	for (int i = 0; i < num_object_center; ++i)
	{
		path = "models\\teapot_mid_poly.obj";

		if (!object_center[i].loadOBJ(path, middlePoint, i, false))
		{
			std::cout << "Failed to read object_center OBJ file!" << std::endl;
			return -1;
		}
		if (!object_bounding_box[i].loadOBJ("models\\cube_low_poly.obj", middlePoint, i, false))
		{
			std::cout << "Failed to read object_center OBJ file!" << std::endl;
			return -1;
		}
		if (!object_aabb[i].loadOBJ("models\\cube_low_poly.obj", middlePoint, i, false))
		{
			std::cout << "Failed to read object_center OBJ file!" << std::endl;
			return -1;
		}
		if (!object_center_debug[i].loadOBJ(path, middlePoint, i, false))
		{
			std::cout << "Failed to read object_center OBJ file!" << std::endl;
			return -1;
		}
	}
	
	if (!plane.loadOBJ("models\\plane_low_poly.obj", middlePoint, 0, false)) {
		std::cout << "Failed to read object_center OBJ file!" << std::endl;
		return -1;
	}
	// Lighting
	Shader shader(GL_FALSE, Shader::S_DF_SHADING);
	Shader shader_around(GL_FALSE, Shader::S_AROUND);
	Shader shader_gbuffer(GL_FALSE, Shader::G_BUFFER);
	Shader shader_bounding_box(GL_FALSE, Shader::N_BOUNDING_BOX);
	Shader shader_octree(GL_FALSE, Shader::S_OCTREE);

	// Normal Shaders
	Shader shader_vertexNormal(GL_TRUE, Shader::N_VERTEX);
	Shader shader_faceNormal(GL_TRUE, Shader::N_FACE);

	for (int i = 0; i < 16; ++i) {
		objects_around[i].Describe(objects_around[i].obj_vertices, objects_around[i].obj_indices, objects_around[i].textureUV,
			objects_around[i].vertexNormals, false);
	}
	std::vector<glm::vec3> all_points;
	std::vector<Render*> center_objects;
	std::vector<Render*> center_models;
	std::vector<std::vector<glm::vec3>> points_in_triangle; // for BSP tree
	std::vector<std::vector<unsigned>> indices_in_triangle;
	std::vector<int> triangle_obj_number;

	// Texture describing
	for (int i = 0; i < num_object_center; ++i)
	{
		if (!object_center[i].loadPPM("models\\metal_roof_diff_512x512.ppm", object_center[i].g_diff_data))
			std::cout << "No PPM diffuse file " << i << " opened" << std::endl;
		if (!object_center[i].loadPPM("models\\metal_roof_spec_512x512.ppm", object_center[i].g_spec_data))
			std::cout << "No PPM specular file " << i << " opened" << std::endl;

		object_center[i].Describe(object_center[i].obj_vertices, object_center[i].obj_indices, object_center[i].textureUV,
			object_center[i].vertexNormals, false);

		object_center[i].DescribeTexture();

		object_bounding_box[i].Describe(object_bounding_box[i].obj_vertices, object_bounding_box[i].obj_indices, object_bounding_box[i].textureUV,
			object_bounding_box[i].vertexNormals, false);

		object_aabb[i].Describe(object_aabb[i].obj_vertices, object_aabb[i].obj_indices, object_aabb[i].textureUV,
			object_aabb[i].vertexNormals, false);

		for (int j = 0; j < object_center[i].obj_vertices.size(); ++j)
		{
			glm::vec3 world_pos_point = object_center[i].obj_vertices[j] + object_center[i].position;
			all_points.push_back(world_pos_point);
		}

		center_objects.push_back(&object_aabb[i]); // only square bounding boxes
		center_models.push_back(&object_center[i]); // real objects

		object_center[i].obj_colors.resize(object_center[i].obj_vertices.size()); // resize color vector same as indices vector

		for (int j = 0; j < object_center[i].obj_indices.size(); j += 3)
		{
			std::vector<glm::vec3> each_indice_to_point;
			std::vector<unsigned> each_indice;
			each_indice_to_point.push_back(object_center[i].obj_vertices[object_center[i].obj_indices[j]] + object_center[i].position);
			each_indice_to_point.push_back(object_center[i].obj_vertices[object_center[i].obj_indices[j + 1]] + object_center[i].position);
			each_indice_to_point.push_back(object_center[i].obj_vertices[object_center[i].obj_indices[j + 2]] + object_center[i].position);
			each_indice.push_back(object_center[i].obj_indices[j]);
			each_indice.push_back(object_center[i].obj_indices[j + 1]);
			each_indice.push_back(object_center[i].obj_indices[j + 2]);
			
			indices_in_triangle.push_back(each_indice);
			points_in_triangle.push_back(each_indice_to_point);
			triangle_obj_number.push_back(i);
		}
	}
	plane.Describe(plane.obj_vertices, plane.obj_indices, plane.textureUV, plane.vertexNormals, false);
	plane.DescribeTexture();

	glm::vec3 pos[16] = {glm::vec3(0,0,0)};
	float theta = 22.5f;
	for (int i = 0; i < num_object_center; ++i)
	{
		object_bounding_box[i].position = object_center[i].position;
		object_bounding_box[i].scale = object_center[i].CalculateBoundingBoxScale() * 0.5f;

		object_aabb[i].position = object_center[i].position;
		object_aabb[i].scale = object_center[i].CalculateBoundingBoxScale() * 0.5f;
	}

	int OcTreeOn = 0;
	int BSPTreeOn = 1;
	BVHType m_type = BSP_TREE;
	ShapeType m_shape = AABB;
	HierarchyManager* top_down_tree;
	int curr_depth = 1; // init depth
	TopDownBVTree(&top_down_tree, all_points, curr_depth, m_shape);

	HierarchyManager* bottom_up_tree = BottomUpBVTree(center_objects, m_shape);

	glm::vec3 initial_center(0);
	float initial_halfWidth = 0;
	glm::vec3 initial_color = glm::vec3(0, 0, 1.f);

	CalculateCenterHalfWidth(all_points, initial_center, initial_halfWidth);
	OctreeNode* m_ocTree = BuildOctree(center_models, initial_center, initial_halfWidth, curr_depth, initial_color);

	BSPNode* m_bspTree = BuildBSPTree(center_models, points_in_triangle, triangle_obj_number, indices_in_triangle, curr_depth);

	for (int i = 0; i < num_object_center; ++i)
	{
		if (m_type == OCTREE)
		{
			object_center_debug[i].obj_colors = object_center[i].obj_colors;
			object_center_debug[i].DescribeDebug(object_center_debug[i].obj_vertices, object_center_debug[i].obj_indices, object_center_debug[i].obj_colors);
		}
		else if (m_type == BSP_TREE)
		{
			object_center_debug[i].obj_colors = object_center[i].obj_colors;
			object_center_debug[i].DescribeDebug(object_center_debug[i].obj_vertices, object_center_debug[i].obj_indices, object_center_debug[i].obj_colors);
		}
	}
	if (m_shape == AABB)
	{
		if (!hierarchy.loadOBJ("models\\cube_low_poly.obj", middlePoint, 0, false))
		{
			std::cout << "Failed to read object_center OBJ file!" << std::endl;
			return -1;
		}
	}
	else
	{
		if (!hierarchy.loadOBJ("models\\sphere_mid_poly.obj", middlePoint, 0, false))
		{
			std::cout << "Failed to read object_center OBJ file!" << std::endl;
			return -1;
		}
	}
	hierarchy.Describe(hierarchy.obj_vertices, hierarchy.obj_indices, hierarchy.textureUV, hierarchy.vertexNormals, false);

	// Light Info
	Light directionalLight;
	directionalLight.num_spheres = 1;
	directionalLight.constant = 1.0f;
	directionalLight.linear = 0.7f;
	directionalLight.quadratic = 1.8f;
	directionalLight.material_ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	Light pointLight;
	pointLight.constant = 1.0f;
	pointLight.linear = 0.7f;
	pointLight.quadratic = 1.8f;
	pointLight.num_spheres = 1;
	pointLight.material_ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	Light spotLight;
	spotLight.constant = 1.0f;
	spotLight.linear = 0.7f;
	spotLight.quadratic = 1.8f;
	spotLight.num_spheres = 1;
	spotLight.material_ambient = glm::vec3(0.1f, 0.1f, 0.1f);

	// Default Light 
	Light Defaultlight;
	Defaultlight.constant = 1.0f;
	Defaultlight.linear = 0.7f;
	Defaultlight.quadratic = 1.8f;
	Defaultlight.num_spheres = 1;
	Defaultlight.material_ambient = glm::vec3(0.1f, 0.1f, 0.1f);

	for (int i = 0; i < 16; ++i)
	{
		directionalLight.color[i] = { 0.2f, 0.5f, 0.8f };
		directionalLight.ambient[i] = 0.1f;
		directionalLight.diffuse[i] = 0.9f;
		directionalLight.specular[i] = 1.f;
		directionalLight.m_type[i] = Light::N_DIRECTIONAL;

		pointLight.color[i] = { 0.5f, 0.2f, 0.1f };
		pointLight.ambient[i] = 0.1f;
		pointLight.specular[i] = 1.0f;
		pointLight.diffuse[i] = 0.9f;
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
		Defaultlight.diffuse[i] = 0.9f;
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

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	float angle = 0.f;
	float orbit_speed = 0.003f;

	// Set all position of spheres 
	for (int i = 0; i < Defaultlight.num_spheres; ++i)
	{
		if (Defaultlight.m_type[i] == Light::N_DIRECTIONAL)
			Defaultlight.Position[i] = -pos[i];
		else
			Defaultlight.Position[i] = pos[i];
	}
	camera.position = glm::vec3(0.0f, 10.f, 20.0f);

	// FrameBuffer 
	unsigned int gbuffer;
	glGenFramebuffers(1, &gbuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer);
	// create a color attachment texture
	unsigned int gPosition, gNormal, gSpec, gDiff;
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	glGenTextures(1, &gSpec);
	glBindTexture(GL_TEXTURE_2D, gSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gSpec, 0);

	glGenTextures(1, &gDiff);
	glBindTexture(GL_TEXTURE_2D, gDiff);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gDiff, 0);

	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

	GLenum DrawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
							  GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, DrawBuffers);
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shader.Use();
	shader.SetInt("gPosition", 0);
	shader.SetInt("gNormal", 1);
	shader.SetInt("gSpec", 2);
	shader.SetInt("gDiff", 3);

	srand(time_t(NULL));
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		float currFrame = (float)glfwGetTime();
		deltaTime = currFrame - lastFrame;
		lastFrame = currFrame;

		// Input
		ProcessInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Draw on frame buffer */
		glBindFramebuffer(GL_FRAMEBUFFER, gbuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// geometry pass
		shader_gbuffer.Use();
		plane.SendTextureInfo(&shader_gbuffer);
		plane.Rendering(&camera, &shader_gbuffer, aspect, GL_TRIANGLES, plane.position, plane.color);
		for (int i = 0; i < num_object_center; ++i)
		{
			object_center[i].SendTextureInfo(&shader_gbuffer);
			object_center[i].Rendering(&camera, &shader_gbuffer, aspect, GL_TRIANGLES, object_center[i].position, object_center[i].color);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// light pass
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gSpec);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gDiff);

		shader.SetInt("DrawStatus", Draw_Status);
		camera.Update(&shader);
		Defaultlight.Update(&shader);
		renderQuad();

		glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write on default Framebuffer

		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Forward Shading (Normals & lights)
		/* Draw Vertex Normal */
		if (Key_Status == 1)
		{
			shader_vertexNormal.Use();
			for (int i = 0; i < num_object_center; ++i)
				object_center[i].Rendering(&camera, &shader_vertexNormal, aspect, GL_TRIANGLES, object_center[i].position, object_center[i].color);
		}
		else if (Key_Status == 2)
		{
			shader_faceNormal.Use();
			for (int i = 0; i < num_object_center; ++i)
				object_center[i].Rendering(&camera, &shader_faceNormal, aspect, GL_TRIANGLES, object_center[i].position, object_center[i].color);
		}
		if (m_type == OCTREE)
		{
			if (OcTreeOn % 2)
			{
				shader_octree.Use();
				for (int i = 0; i < num_object_center; ++i)
					object_center_debug[i].RenderTriangle(&camera, &shader_octree, aspect, object_center[i].position);
			}
		}
		else if (m_type == BSP_TREE)
		{
			if (BSPTreeOn % 2)
			{
				shader_octree.Use();
				for (int i = 0; i < num_object_center; ++i)
					object_center_debug[i].RenderTriangle(&camera, &shader_octree, aspect, object_center[i].position);
			}
		}
		/* Draw bounding boxes */
		shader_bounding_box.Use();
		for (int i = 0; i < num_object_center; ++i)
		{
			if (i == 0)
				object_center[i].position = glm::vec3(1.f, 5.f, 5.f);
			if (i == 1)
				object_center[i].position = glm::vec3(-3.f, 1.f, 4.f);

			object_bounding_box[i].position = object_center[i].position;
			if (object_bounding_box[i].m_BVtype == Render::BoundingVolumeType::AABB)
				object_bounding_box[i].scale = object_center[i].CalculateBoundingBoxScale() * 0.5f;
			else if (object_bounding_box[i].m_BVtype == Render::BoundingVolumeType::Centeroid)
			{
				object_bounding_box[i].m_sphere.m_radius = object_center[i].CalculateCenteroidSphere();
				object_bounding_box[i].scale = glm::vec3(object_bounding_box[i].m_sphere.m_radius,
					object_bounding_box[i].m_sphere.m_radius,
					object_bounding_box[i].m_sphere.m_radius);
			}
			else if (object_bounding_box[i].m_BVtype == Render::BoundingVolumeType::Ritter)
			{
				glm::vec3 offset(0, 0, 0);
				object_bounding_box[i].m_sphere.m_radius = object_center[i].CalculateRitterSphere();
				object_bounding_box[i].position += object_bounding_box[i].m_sphere.m_center;
				object_bounding_box[i].scale = glm::vec3(object_bounding_box[i].m_sphere.m_radius,
					object_bounding_box[i].m_sphere.m_radius,
					object_bounding_box[i].m_sphere.m_radius);
			}
			else if (object_bounding_box[i].m_BVtype == Render::BoundingVolumeType::Larsson)
			{
				object_bounding_box[i].m_sphere.m_radius = object_center[i].CalculateLarssonSphere();
				object_bounding_box[i].position += object_bounding_box[i].m_sphere.m_center;
				object_bounding_box[i].scale = glm::vec3(object_bounding_box[i].m_sphere.m_radius,
					object_bounding_box[i].m_sphere.m_radius,
					object_bounding_box[i].m_sphere.m_radius);
			}
			else if (object_bounding_box[i].m_BVtype == Render::BoundingVolumeType::PCA_sphere)
			{
				object_bounding_box[i].m_sphere.m_radius = object_center[i].CalculatePCASphere();
				object_bounding_box[i].position += object_bounding_box[i].m_sphere.m_center;
				object_bounding_box[i].scale = glm::vec3(object_bounding_box[i].m_sphere.m_radius,
					object_bounding_box[i].m_sphere.m_radius,
					object_bounding_box[i].m_sphere.m_radius);
			}
			else if (object_bounding_box[i].m_BVtype == Render::BoundingVolumeType::PCA_ellipsoid)
			{
				object_bounding_box[i].scale = object_center[i].CalculatePCAElipsoid();
			}
			else if (object_bounding_box[i].m_BVtype == Render::BoundingVolumeType::OBB)
			{
				object_bounding_box[i].scale = object_center[i].CalculatePCAOBB();
			}
			object_bounding_box[i].RenderBoundingBox(&camera, &shader_bounding_box, aspect, object_bounding_box[i].position, object_bounding_box[i].color);
		}
		if (m_type == TOP_DOWN)
		{
			hierarchy.position = top_down_tree->BVposition;
			hierarchy.color = glm::vec3(1, 0, 0);
			hierarchy.RenderBoundingBoxHierarchy(&camera, &shader_bounding_box, aspect, hierarchy.position, hierarchy.color, top_down_tree, curr_depth);
		}
		else if(m_type == BOTTOM_UP)
		{
			hierarchy.position = bottom_up_tree->BVposition;
			hierarchy.color = glm::vec3(0, 0, 1);
			hierarchy.RenderBoundingBoxHierarchy(&camera, &shader_bounding_box, aspect, hierarchy.position, hierarchy.color, bottom_up_tree, curr_depth);
		}
		else if (m_type == OCTREE)
		{
			if(OcTreeOn % 2)
				hierarchy.RenderOcTree(&camera, &shader_bounding_box, aspect, m_ocTree, curr_depth);
		}
		
		/* Draw around objects */
		shader_around.Use();
		for (int i = 0; i < Defaultlight.num_spheres; ++i)
		{
			objects_around[i].position = pos[i];
			objects_around[i].color = Defaultlight.color[i];
			objects_around[i].Rendering(&camera, &shader_around, aspect, GL_TRIANGLES, objects_around[i].position, objects_around[i].color);
		}

		// Click to move and one more click to stop 
		for (int i = 0; i < Defaultlight.num_spheres; ++i)
		{
			if (isMove % 2)
			{
				pos[i].x = 4 * i * (1 + sin(i * 2 * PI / 25 * angle * 3))*sin(PI / (4 + i) * (2 + sin(2 * i * PI / 35 * angle * 3)))*cos(i *PI*(1 + sin(2 * PI / 35 * angle * 3))) * 0.5f;
				pos[i].y = 3 * (4 + sin(i *PI / 25 * angle * 3))*cos(PI / 4 * (2 + sin(i * 2 * PI / 35 * angle * 3)));
				pos[i].z = 5 * (4 + sin(i *PI / 25 * angle * 3))*sin(PI / (4 + i) * (2 + sin(2 * PI / 35 * angle * 3)))*sin(i *PI*(1 + sin(2 * PI / 35 * angle * 3))) * 0.5f;

				angle += (orbit_speed / Defaultlight.num_spheres);
			}
			Defaultlight.Position[i] = pos[i];
			if (ScenarioNum != 3)
			{
				Defaultlight.m_type[i] = Defaultlight.m_type[YourPick];
				if (ScenarioNum == 1)
					Defaultlight.color[i] = Defaultlight.color[YourPick];
			}
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		{
			ImGui::Begin("GUI interface");
			ImGui::Text("imgui interface");

			if (ImGui::Button("Axis Aligned Bounding Box")) {
				if (BoundingVolumeShape)
				{
					isChanged = true;
					BoundingVolumeShape = false;
				}
				for(int i = 0; i < num_object_center; ++i)
					object_bounding_box[i].m_BVtype = Render::BoundingVolumeType::AABB;
			}
			if (ImGui::Button("Bounding Sphere : Centeroid method")) {
				if (!BoundingVolumeShape)
				{
					isChanged = true;
					BoundingVolumeShape = true;
				}
				for (int i = 0; i < num_object_center; ++i)
					object_bounding_box[i].m_BVtype = Render::BoundingVolumeType::Centeroid;
			}
			if (ImGui::Button("Bounding Sphere : Ritter's method")) {
				if (!BoundingVolumeShape)
				{
					isChanged = true;
					BoundingVolumeShape = true;
				}
				for (int i = 0; i < num_object_center; ++i)
					object_bounding_box[i].m_BVtype = Render::BoundingVolumeType::Ritter;
			}
			if (ImGui::Button("Bounding Sphere : Larsson's method")) {
				if (!BoundingVolumeShape)
				{
					isChanged = true;
					BoundingVolumeShape = true;
				}
				for (int i = 0; i < num_object_center; ++i)
					object_bounding_box[i].m_BVtype = Render::BoundingVolumeType::Larsson;
			}
			if (ImGui::Button("Bounding Sphere : PCA based method")) {
				if (!BoundingVolumeShape)
				{
					isChanged = true;
					BoundingVolumeShape = true;
				}
				for (int i = 0; i < num_object_center; ++i)
					object_bounding_box[i].m_BVtype = Render::BoundingVolumeType::PCA_sphere;
			}
			if (ImGui::Button("Bounding Ellipsoid : PCA base method")) {
				if (!BoundingVolumeShape)
				{
					isChanged = true;
					BoundingVolumeShape = true;
				}
				for (int i = 0; i < num_object_center; ++i)
					object_bounding_box[i].m_BVtype = Render::BoundingVolumeType::PCA_ellipsoid;
			}
			if (ImGui::Button("Oriented Bounding Box : PCA based method")) {
				if (BoundingVolumeShape)
				{
					isChanged = true;
					BoundingVolumeShape = false;
				}
				for (int i = 0; i < num_object_center; ++i)
					object_bounding_box[i].m_BVtype = Render::BoundingVolumeType::OBB;
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
			ImGui::Begin("Bounding Volume Hierarchy", &fourth_window);
			ImGui::SliderInt("DEPTH", &curr_depth, 1, 10, "%d");
			if (ImGui::Button("Top Down AABB"))
			{
				m_type = TOP_DOWN;
				m_shape = AABB;
				isHierarchyChanged = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Top Down Sphere"))
			{
				m_type = TOP_DOWN;
				m_shape = SPHERE;
				isHierarchyChanged = true;
			}
			if (ImGui::Button("Bottom Up AABB"))
			{
				m_type = BOTTOM_UP;
				m_shape = AABB;
				isHierarchyChanged = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Bottom Up Sphere"))
			{
				m_type = BOTTOM_UP;
				m_shape = SPHERE;
				isHierarchyChanged = true;
			}
			if (ImGui::Button("Octree"))
			{
				m_type = OCTREE;
				m_shape = AABB;
				++OcTreeOn;
				isHierarchyChanged = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("BSP Tree"))
			{
				m_type = BSP_TREE;
				m_shape = AABB;
				++BSPTreeOn;
				isHierarchyChanged = true;
			}
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
			ImGui::Begin("Control Objects", &sixth_window);
			ImGui::Text("Do you want to add or delete object?");
			if (ImGui::Button("Add Object"))
			{
				AddObject(object_center, num_object_center, middlePoint, object_center_debug, object_bounding_box,
					object_aabb, m_shape, center_objects, center_models, all_points,
					points_in_triangle, indices_in_triangle, triangle_obj_number);
				for (int i = 0; i < num_object_center; ++i)
				{
					object_bounding_box[i].position = object_center[i].position;
					object_bounding_box[i].scale = object_center[i].CalculateBoundingBoxScale() * 0.5f;

					object_aabb[i].position = object_center[i].position;
					object_aabb[i].scale = object_center[i].CalculateBoundingBoxScale() * 0.5f;
				}
				++num_object_center;
				isHierarchyChanged = true;
			}
			ImGui::End();
		}
		if (isChanged)
		{
			if (object_bounding_box[0].m_BVtype == Render::BoundingVolumeType::AABB
				|| object_bounding_box[0].m_BVtype == Render::BoundingVolumeType::OBB)
				path = "models\\cube_low_poly.obj";
			else
				path = "models\\sphere_mid_poly.obj";
			for (int i = 0; i < num_object_center; ++i)
			{
				object_bounding_box[i].obj_vertices.clear();
				object_bounding_box[i].obj_indices.clear();
				object_bounding_box[i].vertexNormals.clear();
				object_bounding_box[i].faceNormals.clear();
				object_bounding_box[i].loadOBJ(path, middlePoint, i, false);
				object_bounding_box[i].Describe(object_bounding_box[i].obj_vertices, object_bounding_box[i].obj_indices,
					object_bounding_box[i].textureUV, object_bounding_box[i].vertexNormals, false);
			}
			isChanged = false;
		}
		if (isHierarchyChanged)
		{
			if (m_shape == Render::BoundingVolumeType::AABB)
				path = "models\\cube_low_poly.obj";
			else
				path = "models\\sphere_mid_poly.obj";
			hierarchy.obj_vertices.clear();
			hierarchy.obj_indices.clear();
			hierarchy.vertexNormals.clear();
			hierarchy.faceNormals.clear();
			hierarchy.loadOBJ(path, middlePoint, 0, false);
			hierarchy.Describe(hierarchy.obj_vertices, hierarchy.obj_indices, hierarchy.textureUV,
				hierarchy.vertexNormals, false);
			if (m_type == TOP_DOWN)
			{
				if (top_down_tree)
					delete top_down_tree;
				TopDownBVTree(&top_down_tree, all_points, curr_depth, m_shape);
				isHierarchyChanged = false;
			}
			else if(m_type == BOTTOM_UP)
			{
				if (bottom_up_tree)
					delete bottom_up_tree;
				bottom_up_tree = BottomUpBVTree(center_objects, m_shape);
				isHierarchyChanged = false;
			}
			else if (m_type == OCTREE)
			{
				if (OcTreeOn % 2)
				{
					if (m_ocTree)
					{
						delete m_ocTree;
						CalculateCenterHalfWidth(all_points, initial_center, initial_halfWidth);
						m_ocTree = BuildOctree(center_models, initial_center, initial_halfWidth, 1, initial_color);
						for (int i = 0; i < num_object_center; ++i)
						{
							object_center_debug[i].obj_colors = object_center[i].obj_colors;
							object_center_debug[i].DescribeDebug(object_center_debug[i].obj_vertices, object_center_debug[i].obj_indices, object_center_debug[i].obj_colors);
						}
					}
				}
				isHierarchyChanged = false;
			}
			else if (m_type == BSP_TREE)
			{
				if (BSPTreeOn % 2)
				{
					if (m_bspTree)
					{
						delete m_bspTree;
						m_bspTree = BuildBSPTree(center_models, points_in_triangle, triangle_obj_number, indices_in_triangle, curr_depth);
						for (int i = 0; i < num_object_center; ++i)
						{
							object_center_debug[i].obj_colors = object_center[i].obj_colors;
							object_center_debug[i].DescribeDebug(object_center_debug[i].obj_vertices, object_center_debug[i].obj_indices, object_center_debug[i].obj_colors);
						}
					}
				}
				isHierarchyChanged = false;
			}
		}

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
