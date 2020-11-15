#pragma once
#include "Light.h"

class PointLight : public Light
{
protected:
	GLfloat intensity;
	GLfloat scale;
	glm::vec4 mPosition;
public:
	PointLight();
	PointLight(glm::vec4 position, glm::vec3 color);
	PointLight(glm::vec4 position, glm::vec3 color, GLfloat intensity, GLfloat scale);

	void getDirectionAndIntensity(const glm::vec3 &pointHit, glm::vec3 &lightDir, glm::vec3 &lightIntensity) const;
	void getShadingInfo(const glm::vec3 &pointHit, glm::vec3 &lightDir, glm::vec3 &lightIntensity, float &distance) const;
	glm::vec3 getDirection(const glm::vec3 &positionOfhit) const;
	glm::vec4 getPosition();
};



