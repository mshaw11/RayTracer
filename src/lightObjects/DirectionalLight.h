#pragma once
#include "Light.h"

class DirectionalLight : public Light {
protected:
	glm::vec4 mLightDirection;
	GLfloat mLightIntensity;

public:
	DirectionalLight();
	DirectionalLight::DirectionalLight(glm::vec4 lightDirection, glm::vec3 color);
	DirectionalLight::DirectionalLight(glm::vec4 lightDirection, glm::vec3 color, GLfloat intensity);
	void getDirectionAndIntensity(const glm::vec3 &pointHit, glm::vec3 &lightDir, glm::vec3 &lightIntensity) const;
	void getShadingInfo(const glm::vec3 &pointHit, glm::vec3 &lightDir, glm::vec3 &lightIntensity, float &distance) const;

};