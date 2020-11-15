#pragma once
#include "RenderingObject.h"
#include <algorithm>
#include <math.h>
class AlgorithmicSphere : public RenderingObject {
	int mRadius;
public:
	bool intersect(const Ray& ray, float & t) const
	{
		float t0, t1; // solutions for t if the ray intersects 
					  // analytic solution
		glm::vec4 L = ray.getOrigin() - mPosition;
		float a = glm::dot(ray.getDirection(), ray.getDirection());
		float b = 2 * glm::dot(ray.getDirection(), L);
		float c = glm::dot(L, L) - (mRadius * mRadius);
		if (!solveQuadratic(a, b, c, t0, t1))
		{
			return false;
		}

		if (t0 > t1)
		{
			std::swap(t0, t1);
		}

		if (t0 < 0) {
			t0 = t1; // if t0 is negative, let's use t1 instead 
			if (t0 < 0)
			{
				return false; // both t0 and t1 are negative 
			}
		}
		//T is point of intersection
		t = t0;

		return true;
	}
	glm::vec3 mix(const glm::vec3 &a, const glm::vec3& b, const GLfloat &mixValue) const;
	glm::vec3 AlgorithmicSphere::calculateColorOfPointHit(const glm::vec3 &pointHit, const glm::vec3 &NormalOfHit, const glm::vec2 &texCoord, const Ray &ray) const;
	void AlgorithmicSphere::getSurfaceData(const glm::vec3 &Phit, glm::vec3 &Nhit, glm::vec2 &tex) const;
	AlgorithmicSphere::AlgorithmicSphere();
	AlgorithmicSphere::AlgorithmicSphere(glm::vec4 position, int radius, glm::vec3 nColor);
	AlgorithmicSphere::AlgorithmicSphere(glm::vec4 position, int radius, glm::vec4 nColor, GLfloat reflectivity, GLfloat indexOfRefraction);
};