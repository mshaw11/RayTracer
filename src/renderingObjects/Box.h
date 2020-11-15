#pragma once
#pragma once
#include "RenderingObject.h"

class Box : public RenderingObject {
private:
	glm::vec3 normal;

	const float VERY_SMALL_NUMBER = 1e-6;



public:
	Box();
	Box::Box(glm::vec4 position, glm::vec3 color, glm::vec3 normalPassed);

	void Box::getSurfaceData(const glm::vec3 &Phit, glm::vec3 &Nhit, glm::vec2 &tex) const;

	bool intersect(const Ray& ray, float & t) const;
	glm::vec3 Box::calculateColorOfPointHit(const glm::vec3 &pointHit, const glm::vec3 &NormalOfHit, const glm::vec2 &texCoord, const Ray &ray) const;



};