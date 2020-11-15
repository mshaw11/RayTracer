#include "DirectionalLight.h"


DirectionalLight::DirectionalLight()
	:DirectionalLight(glm::vec4(-0.25,-1,-0.25,0),glm::vec3(1.0,1.0,1.0), 0.25)
{

}
DirectionalLight::DirectionalLight(glm::vec4 lightDirection, glm::vec3 color)
	: DirectionalLight(lightDirection, color, 0.25) {}

DirectionalLight::DirectionalLight(glm::vec4 lightDirection, glm::vec3 color, GLfloat intensity)
	: Light(Color(color.x, color.y, color.z, 0)), mLightDirection(lightDirection), mLightIntensity(intensity) {}

void DirectionalLight::getDirectionAndIntensity(const glm::vec3 &pointHit, glm::vec3 &lightDir, glm::vec3 &lightIntensity) const
{
	lightDir = glm::vec3(-mLightDirection);
	lightIntensity = mColor.getColor() * mLightIntensity;
}

void DirectionalLight::getShadingInfo(const glm::vec3 &pointHit, glm::vec3 &lightDir, glm::vec3 &lightIntensity, float &distance) const
{
	lightDir = glm::vec3(-mLightDirection);
	lightIntensity = mColor.getColor() * mLightIntensity;
	distance = 0.0f;
}