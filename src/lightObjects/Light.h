#pragma once
#include "../Color.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/constants.hpp>
//#include <glm/glm/ext.hpp>
#include <glm/glm/gtx/norm.hpp>
#include <stdio.h>

class Light{

protected:
	Color mColor;
public:
	Light::Light();
	Light(Color color);
	virtual void getDirectionAndIntensity(const glm::vec3 &pointHit, glm::vec3 &lightDir, glm::vec3 &lightIntensity) const = 0;
	virtual void getShadingInfo(const glm::vec3 &pointHit, glm::vec3 &lightDir, glm::vec3 &lightIntensity, float &distance) const = 0;
	glm::vec3 getColor() const;

};