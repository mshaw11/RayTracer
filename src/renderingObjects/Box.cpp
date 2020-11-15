#include "Box.h"
#include <stdio.h>
#include "Plane.h"
#include <stdio.h>
Box::Box()
	:RenderingObject(glm::vec4(0.0, 0.0, 0.0, 0), glm::vec3(1.0, 0.0, 0.0)), normal(0.0, 1.0, 0.0)
{

}
Box::Box(glm::vec4 position, glm::vec3 color, glm::vec3 normalPassed)
	:RenderingObject(position, color), normal(normalPassed)
{

}

void Box::getSurfaceData(const glm::vec3 &Phit, glm::vec3 &Nhit, glm::vec2 &tex) const
{
	Nhit = normal;
	tex = glm::vec2(0, 0);
}

glm::vec3 Box::calculateColorOfPointHit(const glm::vec3 &pointHit, const glm::vec3 &NormalOfHit, const glm::vec2 &texCoord, const Ray &ray) const
{
	return getColor().getColor();
}
bool Box::intersect(const Ray& ray, float & t) const
{
	//printf("ray direction is: %5f, %5f, %5f\n", ray.getDirection().x, ray.getDirection().y, ray.getDirection().z);
	GLfloat denom = glm::dot(normal, glm::vec3(ray.getDirection()));
	//printf("Denom is: %5f\n", denom);
	if ( (glm::abs(denom)) > VERY_SMALL_NUMBER) // your favorite epsilon
	{
		//printf("Normal is: %2f, %2f, %2f", normal.x, normal.y, normal.z);


		t = glm::dot(glm::vec3(mPosition - ray.getOrigin()), normal) / denom;
		//printf("t is: %f\n", t);
		if (t >= VERY_SMALL_NUMBER)
		{
			printf("Returning true from plane intersect");
			return true; // you might want to allow an epsilon here too
		}
	}
	//printf("Returning false\n");
	return false;
}