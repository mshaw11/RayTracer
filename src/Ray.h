#pragma once
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

class Ray {
protected:
	glm::vec4 mSource, mDirection;

public:
	glm::vec4 getOrigin() const;
	glm::vec4 getDirection() const;
	Ray();
	Ray(glm::vec4 nSource, glm::vec4 nDirection);
};
