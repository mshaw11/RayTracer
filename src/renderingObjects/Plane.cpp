#include "Plane.h"
#include <stdio.h>
Plane::Plane()
	:RenderingObject(glm::vec4(0.0, 0.0, 0.0, 0), glm::vec3(1.0, 0.0, 0.0)), normal(0.0, 1.0, 0.0) {}
Plane::Plane(glm::vec4 position, glm::vec3 color, glm::vec3 normalPassed)
	:RenderingObject(position, color), normal(normalPassed) {}
Plane::Plane(glm::vec4 position, glm::vec3 color, glm::vec3 normalPassed, GLfloat reflectivity)
	: RenderingObject(position, color, reflectivity), normal(normalPassed) {}
Plane::Plane(glm::vec4 position, glm::vec4 color, glm::vec3 normalPassed, GLfloat reflectivity, GLfloat indexOfRefraction)
	: RenderingObject(position, color, reflectivity, indexOfRefraction), normal(normalPassed) {}

void Plane::getSurfaceData(const glm::vec3 &Phit, glm::vec3 &Nhit, glm::vec2 &tex) const
{
	Nhit = normal;
	float scale = 5;
	tex.s = modff(Phit.x, &scale);
	tex = glm::vec2(0, 0);
}

glm::vec3 Plane::calculateColorOfPointHit(const glm::vec3 &pointHit, const glm::vec3 &NormalOfHit, const glm::vec2 &texCoord, const Ray &ray) const
{
	return getColor().getColor();
}

bool Plane::intersect(const Ray& ray, float & t) const
{
	GLfloat denom = glm::dot(normal, glm::vec3(ray.getDirection()));
	if ( (glm::abs(denom)) > VERY_SMALL_NUMBER) // your favorite epsilon
	{
		t = glm::dot(glm::vec3(mPosition - ray.getOrigin()), normal) / denom;
		if (t >= VERY_SMALL_NUMBER)
		{
			return true; // you might want to allow an epsilon here too
		}
	}
	return false;
}
