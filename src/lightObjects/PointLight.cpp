#include "PointLight.h"

PointLight::PointLight(glm::vec4 position, glm::vec3 color, GLfloat intensity, GLfloat scale)
	: Light(Color(color.x, color.y, color.z, 0)), mPosition(position), intensity(intensity), scale(scale) {}

PointLight::PointLight(glm::vec4 position, glm::vec3 color) 
	: PointLight(position, color, 1.0, 100) {}

PointLight::PointLight():
	PointLight(glm::vec4(0, 0, 0, 0), glm::vec3(1.0, 1.0, 1.0), 1.0, 100){}


glm::vec4 PointLight::getPosition()
{
	return mPosition;
}

void PointLight::getDirectionAndIntensity(const glm::vec3 &pointHit, glm::vec3 &lightDir, glm::vec3 &lightIntensity) const
{
	lightDir = getDirection(pointHit);
	float r2 = glm::l1Norm(lightDir);
	lightIntensity = intensity * mColor.getColor() / (GLfloat)(4 * glm::pi<float>() * r2);
}


void PointLight::getShadingInfo(const glm::vec3 &pointHit, glm::vec3 &lightDir, glm::vec3 &lightIntensity, float &distance) const
{
	lightDir = getDirection(glm::vec3(pointHit));
	distance = glm::distance(pointHit, glm::vec3(mPosition));
	GLfloat r2 = (distance / scale) * (distance / scale);
	Color lightIntensityColor;
	lightIntensity = intensity * mColor.getColor() / (4 * glm::pi<float>() * (r2));
	lightIntensityColor.setColor(lightIntensity);
	lightIntensityColor.clip();
	lightIntensity = lightIntensityColor.getColor();
}

glm::vec3 PointLight::getDirection(const glm::vec3 &positionOfhit) const
{
	return glm::normalize(glm::vec3(mPosition) - positionOfhit);
}