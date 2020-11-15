#include "AlgorithmicSphere.h"


AlgorithmicSphere::AlgorithmicSphere()
	:RenderingObject(), mRadius(1)
{

}


AlgorithmicSphere::AlgorithmicSphere(glm::vec4 position, int radius, glm::vec3 nColor)
	: RenderingObject(position, nColor), mRadius(radius)
{

}
AlgorithmicSphere::AlgorithmicSphere(glm::vec4 position, int radius, glm::vec4 nColor, GLfloat reflectivity, GLfloat indexOfRefraction)
	: RenderingObject(position, nColor, reflectivity, indexOfRefraction), mRadius(radius)
{

}



void AlgorithmicSphere::getSurfaceData(const glm::vec3 &Phit, glm::vec3 &Nhit, glm::vec2 &tex) const
{
	Nhit = Phit - glm::vec3(mPosition);
	Nhit = glm::normalize(Nhit);
	// In this particular case, the normal is simular to a point on a unit sphere
	// centred around the origin. We can thus use the normal coordinates to compute
	// the spherical coordinates of Phit.
	// atan2 returns a value in the range [-pi, pi] and we need to remap it to range [0, 1]
	// acosf returns a value in the range [0, pi] and we also need to remap it to the range [0, 1]
	tex.x = (1 + atan2(Nhit.z, Nhit.x) / 3.1459) * 0.5;
	tex.y = acosf(Nhit.y) / 3.1459;
}

glm::vec3 AlgorithmicSphere::mix(const glm::vec3 &a, const glm::vec3& b, const GLfloat &mixValue) const
{
	return a * (1 - mixValue) + b * mixValue;
}

glm::vec3 AlgorithmicSphere::calculateColorOfPointHit(const glm::vec3 &pointHit, const glm::vec3 &NormalOfHit, const glm::vec2 &texCoord, const Ray &ray) const
{
	float scale = 4;
	float pattern = (fmodf(texCoord.x * scale, 1) > 0.5) ^ (fmodf(texCoord.y * scale, 1) > 0.5);
	glm::vec3 colorOfHit = mix(getColor().getColor(), getColor().getColor() * 0.8f, pattern);
	return colorOfHit;
}

