#pragma once
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "../Ray.h"
#include "../Color.h"



class RenderingObject {
protected:
	glm::vec4 mPosition;
	Color mColor;
	glm::vec3 mAlbedo = glm::vec3(0.18);
	GLfloat mReflectivity = 0.0f;
	GLfloat mIndexOfRefraction = 0.0f;

public:
	// virtual functions need to be overloaded by derived class
	/*Return the point of intersection between the ray and your object*/
	virtual bool intersect(const Ray&, float &) const = 0;
	/*Return the surface data based on the point hit on your object, return the normal and texture coordinates (if appropriate).*/
	virtual void getSurfaceData(const glm::vec3 &Phit, glm::vec3 &Nhit, glm::vec2 &tex) const = 0;
	/*Return the color of the point hit on your object, applying any relevant teexture calculations.*/
	virtual glm::vec3 calculateColorOfPointHit(const glm::vec3 &pointHit, const glm::vec3 &NormalOfHit, const glm::vec2 &texCoord, const Ray &ray) const = 0;

	RenderingObject();
	RenderingObject(glm::vec4 nPosition, glm::vec3 nColor);
	RenderingObject(glm::vec4 nPosition, glm::vec3 nColor, GLfloat reflectivity);
	RenderingObject(glm::vec4 nPosition, glm::vec4 nColor, GLfloat reflectivity, GLfloat indexOfRefraction);

	glm::vec4 getPosition();
	void setPosition(glm::vec4 nPosition);
	Color getColor() const;
	void setColor(glm::vec3 nColor);
	bool RenderingObject::solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1) const;
	glm::vec3 getAlbedo() const;
	GLfloat getReflectivity() const;
	void RenderingObject::setReflectivity(GLfloat newReflectivity);
	GLfloat getIndexOfRefraction() const;
	void setIndexOfRefraction(GLfloat newIndexOfRefraction);


	

};