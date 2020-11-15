#include "Camera.h"

Camera::Camera(int width, int height)
{
	staticCamera = false;
	cameraPosition = glm::vec3(10.0f, 10.0f, 50.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	upVector = glm::vec3(0.0f, 1.0f, 0.0f);

	viewMatrix = glm::lookAt(
		cameraPosition,
		cameraPosition + cameraFront,
		upVector);

	projectionMatrix = glm::perspective(
		45.0f, (float)((float)width / (float)height), 1.0f, 500000.0f);

	projectionViewMatrix = projectionMatrix * viewMatrix;
}

Camera::Camera(glm::vec3 newCameraPosition, glm::vec3 newCameraTarget)
{
	staticCamera = true;
	cameraPosition = newCameraPosition;
	cameraTarget = newCameraTarget;
	upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	calculateLookAtForStaticCamera();

}

Camera::~Camera()
{

}

void Camera::calculateLookAt()
{
	viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, upVector);
	projectionViewMatrix = projectionMatrix * viewMatrix;
}

// Public - getters
glm::vec3 Camera::getCameraPosition()
{
	return cameraPosition;
}

glm::mat4 Camera::getView()
{
	return viewMatrix;
}

glm::mat4 Camera::getProjection()
{
	return projectionMatrix;
}

glm::mat4 Camera::getProjectionViewMatrix()
{
	return projectionViewMatrix;
}

// Mutators
void Camera::setKey(GLint key, GLboolean value)
{
	keys[key] = value;
}

void Camera::addCameraSpeed(GLint speed)
{
	cameraSpeed += speed;
}

void Camera::move(GLfloat deltaTime)
{
	GLfloat speed = cameraSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		cameraPosition += speed * cameraFront;
	}
	if (keys[GLFW_KEY_S])
		cameraPosition -= speed * cameraFront;

	if (keys[GLFW_KEY_A])
		cameraPosition -= glm::normalize(glm::cross(cameraFront, upVector)) * speed;

	if (keys[GLFW_KEY_D])
		cameraPosition += glm::normalize(glm::cross(cameraFront, upVector)) * speed;

	calculateLookAt();
}


/*Set camera position of a static camera - one that is not updated by mouse and keyboard movements*/
void Camera::setCameraPosition(glm::vec3 newCameraPosition)
{
	cameraPosition = newCameraPosition;
	calculateFrontVectorForStaticCamera();
	calculateLookAtForStaticCamera();
}
/*Set camera target position of a static camera - one that is not updated by mouse and keyboard movements*/
void Camera::setCameraTarget(glm::vec3 newCameraTarget)
{
	cameraTarget = newCameraTarget;
	calculateFrontVectorForStaticCamera();
	calculateLookAtForStaticCamera();
}

void Camera::calculateFrontVectorForStaticCamera()
{
	glm::vec3 addedVector = cameraTarget - cameraPosition;
	printf("Front is: X %f, Y %f Z %f", addedVector.x, addedVector.y, addedVector.z);
	cameraFront = glm::normalize(addedVector);
}

void Camera::calculateLookAtForStaticCamera()
{
	viewMatrix = glm::lookAt(cameraPosition, cameraTarget, upVector);
	projectionViewMatrix = projectionMatrix * viewMatrix;
}

glm::vec3 Camera::getCameraFront()
{
	return cameraFront;
}
