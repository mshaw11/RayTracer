#include "Color.h"


Color::Color() {
	red = 0.5;
	green = 0.5;
	blue = 0.5;
}

Color::Color(GLfloat r, GLfloat g, GLfloat b, GLfloat s) {
	red = r;
	green = g;
	blue = b;
	special = s;
}
// method functions
GLfloat Color::getRed() { return red; }
GLfloat Color::getGreen() { return green; }
GLfloat Color::getBlue() { return blue; }
GLfloat Color::getSpecial() { return special; }
GLfloat Color::getTransparency() { return special; }
glm::vec3 Color::getColor() const { return glm::vec3(red, green, blue); }

void Color::setColorRed(double redValue) { red = redValue; }
void Color::setColorGreen(double greenValue) { green = greenValue; }
void Color::setColorBlue(double blueValue) { blue = blueValue; }
void Color::setColorSpecial(double specialValue) { special = specialValue; }
void Color::setColor(glm::vec3 color) { red = color.r; blue = color.b; green = color.g; }

GLfloat Color::brightness() {
	return(red + green + blue) / 3;
}

Color Color::colorScalar(double scalar) {
	return Color(red*scalar, green*scalar, blue*scalar, special);
}

Color Color::colorAdd(Color color) {
	return Color(red + color.getRed(), green + color.getGreen(), blue + color.getBlue(), special);
}

Color Color::colorMultiply(Color color) {
	return Color(red*color.getRed(), green*color.getGreen(), blue*color.getBlue(), special);
}

Color Color::colorAverage(Color color) {
	return Color((red + color.getRed()) / 2, (green + color.getGreen()) / 2, (blue + color.getBlue()) / 2, special);
}

Color Color::clip() {
	GLfloat alllight = red + green + blue;
	GLfloat excesslight = alllight - 3;
	if (excesslight > 0) {
		red = red + excesslight*(red / alllight);
		green = green + excesslight*(green / alllight);
		blue = blue + excesslight*(blue / alllight);
	}
	if (red > 1) { red = 1; }
	if (green > 1) { green = 1; }
	if (blue > 1) { blue = 1; }
	if (red < 0) { red = 0; }
	if (green < 0) { green = 0; }
	if (blue < 0) { blue = 0; }

	return Color(red, green, blue, special);
}
