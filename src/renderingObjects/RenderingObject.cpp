#include "RenderingObject.h"
#include <algorithm>

RenderingObject::RenderingObject()
	:mPosition(glm::vec4(0, 0, 0, 0)),
		mColor(1.0, 1.0, 1.0, 0.0),
		mReflectivity(0.0)
{

}
RenderingObject::RenderingObject(glm::vec4 nPosition, glm::vec3 nColor)
	: mPosition(nPosition), mColor(nColor.r, nColor.g, nColor.b, 0.0), mReflectivity(0.0)
{

}
RenderingObject::RenderingObject(glm::vec4 nPosition, glm::vec3 nColor, GLfloat reflectivity)
	:mPosition(nPosition), mColor(nColor.r, nColor.g, nColor.b, 0.0), mReflectivity(reflectivity)
{
}

RenderingObject::RenderingObject(glm::vec4 nPosition, glm::vec4 nColor, GLfloat reflectivity, GLfloat indexOfRefraction)
	: mPosition(nPosition), mColor(nColor.r, nColor.g, nColor.b, nColor.w), mReflectivity(reflectivity), mIndexOfRefraction(indexOfRefraction)
{

}

glm::vec4 RenderingObject::getPosition()
{
	return mPosition;
}

void RenderingObject::setPosition(glm::vec4 nPosition)
{
	mPosition = nPosition;
}
Color RenderingObject::getColor() const
{
	return mColor;
}

void RenderingObject::setColor(glm::vec3 nColor)
{
	mColor.setColor(nColor);
}

bool RenderingObject::solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1) const
{
	float discr = b * b - 4 * a * c;
	if (discr < 0) return false;
	else if (discr == 0) x0 = x1 = -0.5 * b / a;
	else {
		float q = (b > 0) ?
			-0.5 * (b + sqrt(discr)) :
			-0.5 * (b - sqrt(discr));
		x0 = q / a;
		x1 = c / q;
	}
	if (x0 > x1) std::swap(x0, x1);

	return true;
}


glm::vec3 RenderingObject::getAlbedo() const
{
	return mAlbedo;
}


GLfloat RenderingObject::getReflectivity() const
{
	return mReflectivity;
}
void RenderingObject::setReflectivity(GLfloat newReflectivity)
{
	mReflectivity = newReflectivity;
}

GLfloat RenderingObject::getIndexOfRefraction() const
{
	return mIndexOfRefraction;
}
void RenderingObject::setIndexOfRefraction(GLfloat newIndexOfRefraction)
{
	mIndexOfRefraction = newIndexOfRefraction;
}