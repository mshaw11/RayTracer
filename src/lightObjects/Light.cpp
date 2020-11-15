#pragma once
#include "Light.h"

Light::Light()
	:mColor(Color(1.0,1.0,1.0,1.0)) {}

Light::Light(Color color)
	:mColor(color){}

glm::vec3 Light::getColor() const
{
	return mColor.getColor();
}


