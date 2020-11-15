#pragma once
#include "Ray.h"


glm::vec4 Ray::getOrigin() const
{ 
	return mSource;
}
glm::vec4 Ray::getDirection() const
{ 
	return mDirection;
}

Ray::Ray()
{
	mSource = glm::vec4(0, 0, 0, 0);
	mDirection = glm::vec4(1, 0, 0, 0);
}

Ray::Ray(glm::vec4 nSource, glm::vec4 nDirection)
	: mSource(nSource), mDirection(nDirection)
{

}