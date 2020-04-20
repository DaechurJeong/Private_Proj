/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Light.h
Purpose: Prototype of Light class which send data to shader
Language: MSVC C++
Platform: VS2017, Windows
Project: jungdae.chur_CS300_1
Author: Charlie Jung, jungdae.chur
Creation date: 9/28/2018
End Header --------------------------------------------------------*/
#pragma once
#include "glm/glm.hpp"
#include "Shader.h"
#include <iostream>

class Light
{
public:
	Light() {};
	~Light() {}
	enum LightType {
		N_POINT,
		N_DIRECTIONAL,
		N_SPOT,
	};
	glm::vec3   color[16];
	glm::vec3   Position[16];
	glm::vec3   material_ambient;
	float		ambient[16], diffuse[16], specular[16];
	float       constant, linear, quadratic;
	float       cutOff[16], outercutOff[16], fallOff[16];
	int			num_spheres = 16;
	LightType   m_type[16] = {N_POINT};

	void Update(Shader* shader)
	{
		for (int i = 0; i < 16; ++i)
		{
			std::string number = std::to_string(i);
			if (m_type[i] == N_DIRECTIONAL)
			{
				shader->SetFloat("dirLight[" + number + "].ambientStrength", ambient[i]);
				shader->SetFloat("dirLight[" + number + "].diffuseStrength", diffuse[i]);
				shader->SetFloat("dirLight[" + number + "].specularStrength", specular[i]);
				shader->SetVec3("dirLight[" + number + "].direction", Position[i]);
				shader->SetVec3("dirLight[" + number + "].lightcolor", color[i]);
			}
			if (m_type[i] == N_POINT)
			{
				shader->SetFloat("pointLight[" + number + "].ambientStrength", ambient[i]);
				shader->SetFloat("pointLight[" + number + "].diffuseStrength", diffuse[i]);
				shader->SetFloat("pointLight[" + number + "].specularStrength", specular[i]);
				shader->SetVec3("pointLight[" + number + "].lightPosition", Position[i]);
				shader->SetVec3("pointLight[" + number + "].lightcolor", color[i]);
				shader->SetFloat("pointLight[" + number + "].constant", constant);
				shader->SetFloat("pointLight[" + number + "].linear", linear);
				shader->SetFloat("pointLight[" + number + "].quadratic", quadratic);
			}
			if (m_type[i] == N_SPOT)
			{
				shader->SetFloat("spotLight[" + number + "].ambientStrength", ambient[i]);
				shader->SetFloat("spotLight[" + number + "].diffuseStrength", diffuse[i]);
				shader->SetFloat("spotLight[" + number + "].specularStrength", specular[i]);
				shader->SetVec3("spotLight[" + number + "].lightPosition", Position[i]);
				shader->SetVec3("spotLight[" + number + "].direction", -Position[i]);
				shader->SetVec3("spotLight[" + number + "].lightcolor", color[i]);
				shader->SetFloat("spotLight[" + number + "].constant", constant);
				shader->SetFloat("spotLight[" + number + "].linear", linear);
				shader->SetFloat("spotLight[" + number + "].quadratic", quadratic);
				shader->SetFloat("spotLight[" + number + "].fallOff", fallOff[i]);
				shader->SetFloat("spotLight[" + number + "].cutOff", cutOff[i]);
				shader->SetFloat("spotLight[" + number + "].outercutOff", outercutOff[i]);
			}
			shader->SetInt("type[" + number + "]", m_type[i]);
			shader->SetVec3("material.ambient", material_ambient);
			shader->SetInt("num_obj", num_spheres);
		}
	}
	Light& operator=(Light& rhs)
	{
		*color = *rhs.color;
		*Position = *rhs.Position;
		*ambient = *rhs.ambient;
		*specular = *rhs.specular;
		*diffuse = *rhs.diffuse;
		constant = rhs.constant;
		linear = rhs.linear;
		quadratic = rhs.quadratic;
		*m_type = *rhs.m_type;
		*cutOff = *rhs.cutOff;
		*outercutOff = *rhs.outercutOff;

		return *this;
	}
};