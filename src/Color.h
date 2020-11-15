#pragma once
#include <glew.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
class Color {
protected:
	double red, green, blue, special;

public:
	Color::Color();
	Color::Color(GLfloat r, GLfloat g, GLfloat b, GLfloat s);
	// method functions
	GLfloat Color::getRed();
	GLfloat Color::getGreen();
	GLfloat Color::getBlue();
	GLfloat Color::getSpecial();
	GLfloat Color::getTransparency();
	glm::vec3 Color::getColor() const;

	void Color::setColorRed(double redValue);
	void Color::setColorGreen(double greenValue);
	void Color::setColorBlue(double blueValue);
	void Color::setColorSpecial(double specialValue);
	void Color::setColor(glm::vec3 color);

	GLfloat Color::brightness();
	Color Color::colorScalar(double scalar);
	Color Color::colorAdd(Color color);
	Color Color::colorMultiply(Color color);
	Color Color::colorAverage(Color color);
	Color Color::clip();


};