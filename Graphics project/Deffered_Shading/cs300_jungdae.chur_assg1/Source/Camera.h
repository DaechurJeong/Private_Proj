/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.h
Purpose: Prototype of Camera class which moves and calculates camera stuff
Language: MSVC C++
Platform: VS2017, Windows
Project: jungdae.chur_CS300_1
Author: Charlie Jung, jungdae.chur
Creation date: 9/28/2018
End Header --------------------------------------------------------*/
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	LOOK_UP,
	LOOK_DOWN,
	LOOK_RIGHT,
	LOOK_LEFT,
};

const float YAW = -90.0f;
const float PITCH = -25.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Shader;
class Camera
{
public:

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	// Constructor with vectors
	Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f),
		float _yaw = YAW, float _pitch = PITCH);
	glm::mat4 GetViewMatrix();
	void Update(Shader* _shader);
	void ProcessKeyboard(Camera_Movement _direction, float _deltaTime);

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yoffset);
	void updateCameraVectors();

};