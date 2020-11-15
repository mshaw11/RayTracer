#pragma warning(disable:4996)
#pragma once
#include <glew.h>
#include "Camera.h"
#include "Ray.h"
#include "renderingObjects\AlgorithmicSphere.h"
#include "renderingObjects\RenderingObject.h"
#include "renderingObjects\Plane.h"
#include "lightObjects\Light.h"
#include "lightObjects\PointLight.h"
#include "lightObjects\DirectionalLight.h"
#include <ctime>

#include <vector>
#include <memory>
#include <sstream>
#include <string>
#include <iostream>

const int BITMAP_FILE_HEADER_SIZE = 14;
const int BITMAP_INFO_HEADER_SIZE = 40;
const int PIXEL_ARRAY_OFFSET = 54;
const int NUMBER_OF_COLOR_PLANES = 1;
const int BITS_PER_PIXEL = 24;
const int MAX_COLOR_VALUE = 255;
const int DOTS_PER_INCH = 72;
const double INCHES_IN_A_METER = 39.3701;
const int FOV = 90;
const float K_INFINITY = std::numeric_limits<float>::max();
const float VERY_SMALL_NUMBER = 1e-6;
const GLfloat INDEX_OF_REFRACTION_AIR = 1.0f;
struct RGBType {
	double r;
	double g;
	double b;
};

int main();
void saveBMP(const char *filename, int w, int h, int dpi, RGBType *data);
bool trace(const Ray &ray, const std::vector<std::unique_ptr<RenderingObject>> &objects, float &tNear, const RenderingObject *&hitObject, bool shadowRay);
glm::vec3 doRay(const Ray &ray, glm::vec3 currentColor, std::vector<std::unique_ptr<RenderingObject>> &renderingObjectList, std::vector<std::unique_ptr<Light>> & lightList, const glm::mat4 &worldSpaceMatrix, const int recursionDepth, const int MAX_DEPTH);
glm::vec3 refract(const glm::vec3 &I, const glm::vec3 &N, const GLfloat &ior);
Ray reflect(const Ray &ray, const glm::vec3 &normalOfHit, const glm::vec3 &pointHit, const GLfloat &bias, const glm::mat4 &worldSpaceMatrix);
void fresnel(const glm::vec3 &I, const glm::vec3 &N, const GLfloat &ior, GLfloat &kr);
void printMat4(glm::mat4 mat4Object);
void printVector(glm::vec3 vector);
void printVector(glm::vec4 vector);
int welcomeAndGetSceneNumber();
void renderSceneOne(std::vector<std::unique_ptr<RenderingObject>> &objectList, std::vector<std::unique_ptr<Light>> & lightList);
void renderSceneTwo(std::vector<std::unique_ptr<RenderingObject>> &objectList, std::vector<std::unique_ptr<Light>> & lightList);
void renderSceneThree(std::vector<std::unique_ptr<RenderingObject>> &objectList, std::vector<std::unique_ptr<Light>> & lightList);
void renderSceneFour(std::vector<std::unique_ptr<RenderingObject>> &objectList, std::vector<std::unique_ptr<Light>> & lightList);
void renderSceneFive(std::vector<std::unique_ptr<RenderingObject>> &objectList, std::vector<std::unique_ptr<Light>> & lightList);
void renderSceneSix(std::vector<std::unique_ptr<RenderingObject>> &objectList, std::vector<std::unique_ptr<Light>> & lightList);


int main()
{
	printf("*** Matthew Logan Shaw - 40101817 - ELE 4014 - Simple Ray Tracer Program ***\n");
	/*Adjustable Figures*/
	int width = 840;
	int height = 480;
	float imageAspectRatio = (float)width / (float)height;
	float scale = glm::tan(glm::radians(FOV * 0.5));	//Not recomended to touch this
	scale = 1.001705f;
	int n = width * height;
	int maximumRecursiveDepth = 5;
	clock_t t1, t2;
	RGBType *defaultPicture = new RGBType[n];
	int currentPixel = 0;
	Camera camera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, -10));
	
	//Load models here
	std::vector<std::unique_ptr<RenderingObject>> objectList;
	//Load lights here
	std::vector<std::unique_ptr<Light>> lightList;

	int sceneSelectionNumber = welcomeAndGetSceneNumber();

	switch (sceneSelectionNumber) {
	case 1: renderSceneOne(objectList, lightList);
		break;
	case 2: renderSceneTwo(objectList, lightList);
		break;
	case 3: renderSceneThree(objectList, lightList);
		break;
	case 4: renderSceneFour(objectList, lightList);
		maximumRecursiveDepth = 10;
		break;
	case 5: renderSceneFive(objectList, lightList);
		break;
	case 6: renderSceneSix(objectList, lightList);
		break;
	default: printf("Not a scene - close the program and try again");
	}
	
	t1 = clock();
	//Apply viewing transformation
	glm::vec4 rayOrigin(0, 0, 0, 1);
	glm::vec4 rayWorldSpaceOrigin;
	glm::mat4 projectionViewMatrix = camera.getProjectionViewMatrix();
	rayWorldSpaceOrigin = projectionViewMatrix * rayOrigin;
	
	printf("Ray tracing scene %i with maximum recursion depth set to %i. Generated bitmap will be %i pixels in width and %i pixels in height.\n", sceneSelectionNumber, maximumRecursiveDepth, width, height);
	
	//Repeat for all rows j
	for (int pixelRow = 0; pixelRow < width; pixelRow++)
	{
		//Repeat for all columns i
		for (int pixelHeight = 0; pixelHeight < height; pixelHeight++)
		{
			glm::vec3 rayColor(0, 0, 0);
			glm::vec4 rayDirection(0, 0, 0, 0);
			float x = (2 * (pixelRow + 0.5) / (float)width - 1) * imageAspectRatio * scale;
			float y = (1 - 2 * (pixelHeight + 0.5) / (float)height) * scale;
			rayDirection = glm::vec4(x, y, -1, 0);
			//printf("Ray direction before change is: %5f,%5f,%5f\n", rayDirection.x, rayDirection.y, rayDirection.z);

			//Convert ray origin and ray direction to world space coordinates
			glm::vec4 rayWorldSpaceDirection;
			rayWorldSpaceDirection = rayDirection * projectionViewMatrix;
			rayWorldSpaceDirection = glm::normalize(rayWorldSpaceDirection);
			//printf("Ray direction before change is: %5f,%5f,%5f\n", rayDirection.x, rayDirection.y, rayDirection.z);
			Ray primaryRay(rayWorldSpaceOrigin, rayWorldSpaceDirection);

			rayColor = doRay(primaryRay, rayColor, objectList, lightList, projectionViewMatrix, 0, maximumRecursiveDepth);

		 

			currentPixel = pixelHeight * width + pixelRow;
			defaultPicture[currentPixel].r = rayColor.r; // MAX_COLOR_VALUE;
			defaultPicture[currentPixel].g = rayColor.g;		/// MAX_COLOR_VALUE;
			defaultPicture[currentPixel].b = rayColor.b; // MAX_COLOR_VALUE;
		}
	}
	saveBMP( (("generatedRayTraceScene" + std::to_string(sceneSelectionNumber) + ".bmp").c_str()) , width, height, DOTS_PER_INCH, defaultPicture);
	t2 = clock();
	GLfloat diff = ((float)t2 - (float)t1) / 1000;
	printf("Tracing complete, with a render time of %2f seconds.\n", diff);
	system("pause");
	return 0;
}

/*The primary recursive function of the ray tracer.*/
glm::vec3 doRay(const Ray &ray, glm::vec3 currentColor, std::vector<std::unique_ptr<RenderingObject>> &renderingObjectList, std::vector<std::unique_ptr<Light>> & lightList, const glm::mat4 &worldSpaceMatrix, const int recursionDepth, const int MAX_DEPTH)
{
	if (recursionDepth > MAX_DEPTH)
	{
		/**This color is the default color of the render space.
		 **We return this when we have recursed to the max depth and don't want to spend more time computing values.*/
		return glm::vec3(0.0,0.0,0.0);
	}

	GLfloat bias = 1e-4;
	Color hitColor(0.0f, 0.0f, 0.0f, 0.0f);
	const RenderingObject *hitObject = nullptr; // this is a pointer to the hit object 
	float t; // this is the intersection distance from the ray origin to the hit point 
	if (trace(ray, renderingObjectList, t, hitObject, false)) {
		glm::vec3 pointHit = glm::vec3(ray.getOrigin()) + glm::vec3(ray.getDirection()) * ((GLfloat)t);
		glm::vec3 normalOfHit;
		glm::vec2 textureCoords;
		hitObject->getSurfaceData(pointHit, normalOfHit, textureCoords);

		//If reflective and not transparent
		if (hitObject->getReflectivity() > VERY_SMALL_NUMBER && hitObject->getColor().getTransparency() <= VERY_SMALL_NUMBER)
		{
			Ray reflectionRay = reflect(ray, normalOfHit, pointHit, bias, worldSpaceMatrix);
			hitColor.setColor(hitColor.getColor() + (GLfloat) 0.8 * doRay(reflectionRay, currentColor, renderingObjectList, lightList, worldSpaceMatrix, recursionDepth + 1, MAX_DEPTH));
		}
		//Refractive surface - calculate reflection on the surface and refraction based on fresnel value of point hit
		else if (hitObject->getReflectivity() > VERY_SMALL_NUMBER && hitObject->getColor().getTransparency() >= VERY_SMALL_NUMBER)
		{
			glm::vec3 refractionColor(0);
			GLfloat kr;
			fresnel(glm::vec3(ray.getDirection()), normalOfHit, hitObject->getIndexOfRefraction(), kr);
			bool outside = false;
			//Are we inside or outside an object?
			if (glm::dot(glm::vec3(ray.getDirection()), normalOfHit) < 0)
			{
				outside = true;
			}
			glm::vec3 refractionBias = bias * normalOfHit;
			// compute refraction if it is not a case of total internal reflection
			if (kr < 1) {
				glm::vec3 refractionDirectionVec3(0);
				if (outside)
				{
					refractionDirectionVec3 = glm::normalize(glm::refract(glm::vec3(ray.getDirection()), normalOfHit, INDEX_OF_REFRACTION_AIR / hitObject->getIndexOfRefraction()));
				}
				else
				{
					refractionDirectionVec3 = glm::normalize(glm::refract(glm::vec3(ray.getDirection()), normalOfHit,  hitObject->getIndexOfRefraction() / INDEX_OF_REFRACTION_AIR));
				}
				glm::vec4 refractionDirection = glm::vec4(refract(glm::vec3(ray.getDirection()), normalOfHit, hitObject->getIndexOfRefraction()), 0);
				refractionDirection = glm::normalize(refractionDirection);
				glm::vec3 refractionRayOrig(0);
				if (outside)
				{
					refractionRayOrig = pointHit - refractionBias;
				}
				else
				{
					refractionRayOrig = pointHit + refractionBias;
				}
				glm::vec4 refractionRayOriginVec4 = glm::vec4(refractionRayOrig, 1);
				Ray refractedRay(refractionRayOriginVec4, refractionDirection);
				refractionColor = doRay(refractedRay, currentColor, renderingObjectList, lightList, worldSpaceMatrix, recursionDepth + 1, MAX_DEPTH);
			} //End of refraction calc - now calculate reflection of the surface.
			glm::vec3 reflectionDirectionVec3 = glm::reflect(glm::vec3(ray.getDirection()), normalOfHit);
			glm::vec4 reflectionDirection = glm::vec4(reflectionDirectionVec3.x, reflectionDirectionVec3.y, reflectionDirectionVec3.z, 0);
			reflectionDirection = reflectionDirection * worldSpaceMatrix;
			reflectionDirection = glm::normalize(reflectionDirection);
			glm::vec3 calculatedBias = bias * normalOfHit;
			glm::vec3 reflectionRayOrig;
			if (outside)
			{
				reflectionRayOrig = pointHit + calculatedBias;
			}
			else
			{
				reflectionRayOrig = pointHit - calculatedBias;
			}
			Ray reflectionRay(glm::vec4(reflectionRayOrig, 1), reflectionDirection);
			glm::vec3 reflectionColor = doRay(reflectionRay, currentColor, renderingObjectList, lightList, worldSpaceMatrix, recursionDepth + 1, MAX_DEPTH);

			// mix the reflection and refraction
			hitColor.setColor(hitColor.getColor() + reflectionColor * kr + refractionColor * (1 - kr));
		}
		/*Standard Diffuse surface. Surface is not reflective or transparent.*/
		else
		{ 
			/*For every light calculate the color of the point hit
			  and identify whether it is in shadow from this light source or not.*/
			for (uint32_t i = 0; i < lightList.size(); ++i) {
				glm::vec3 lightDir, lightIntensity;
				GLfloat distance;
				lightList.at(i)->getShadingInfo(pointHit, lightDir, lightIntensity, distance);
				glm::vec3 biasCalc = normalOfHit * bias;
				glm::vec4 shadowCheckOrigin = glm::vec4(pointHit + biasCalc, 1.0);
				glm::vec4 shadowCheckDirection = glm::vec4(lightDir,0);
				shadowCheckDirection = shadowCheckDirection * worldSpaceMatrix;
				Ray shadowCheckRay(shadowCheckOrigin, shadowCheckDirection);
				float shadowT = distance;
				const RenderingObject *shadowHitObject = nullptr;
				if (!trace(shadowCheckRay, renderingObjectList, shadowT, shadowHitObject, true))
				{
					glm::vec3 colorOfPointHitOnObject = hitObject->calculateColorOfPointHit(pointHit, normalOfHit, textureCoords, ray);
					GLfloat dotResult = glm::dot(normalOfHit, glm::vec3(shadowCheckDirection));
					hitColor.setColor(hitColor.getColor() + (colorOfPointHitOnObject * lightIntensity * glm::max((GLfloat)0.f, dotResult)));
				}
			}
		}
	}
	hitColor.clip();
	return hitColor.getColor();
}
/*Used for calculating the simple reflection directions (**NOT** for calculating total internal reflections caused by refraction)*/
Ray reflect(const Ray &ray, const glm::vec3 &normalOfHit, const glm::vec3 &pointHit, const GLfloat &bias, const glm::mat4 &worldSpaceMatrix)
{
	glm::vec3 reflectionDirectionVec3 = glm::reflect(glm::vec3(ray.getDirection()), normalOfHit);
	glm::vec4 reflectionDirection = glm::vec4(reflectionDirectionVec3.x, reflectionDirectionVec3.y, reflectionDirectionVec3.z, 0);
	reflectionDirection = reflectionDirection * worldSpaceMatrix;
	reflectionDirection = glm::normalize(reflectionDirection);
	glm::vec4 reflectionOrigin = glm::vec4(pointHit + normalOfHit * bias, 1.0);
	reflectionOrigin = reflectionOrigin * worldSpaceMatrix;
	Ray reflectionRay(reflectionOrigin, reflectionDirection);
	return reflectionRay;
}

/*Calculate the refraction direction based on the incident, normal of the surface hit and the index of refraction for the object.
 Referenced and based on code derived from: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel */
glm::vec3 refract(const glm::vec3 &incidentVector, const glm::vec3 &normalOfRefractiveSurface, const GLfloat &ior)
{
	GLfloat cosi = glm::clamp(glm::dot(incidentVector, normalOfRefractiveSurface), (GLfloat)-1,(GLfloat) 1);
	GLfloat etai = 1, etat = ior; // etai is the index of refraction of the medium the ray is in before entering the second medium
	glm::vec3 n = normalOfRefractiveSurface;
	if (cosi < 0)
	{
		cosi = -cosi;
	}
	else 
	{ 
		std::swap(etai, etat);
		n = -normalOfRefractiveSurface;
	}

	GLfloat eta = etai / etat;
	GLfloat k = 1 - eta * eta * (1 - cosi * cosi);
	if (k < 0)
	{
		//Total internal reflection
		return glm::vec3(0);
	}
	else
	{
		glm::vec3 result = eta * incidentVector + (eta * cosi - glm::sqrt(k)) * n;
		return result;
	}

}
/*One of the most critical funtions. Traces a ray's path and identifies the closest opaque object in it's path (if it hits any objects at all)*/
bool trace(const Ray &ray, const std::vector<std::unique_ptr<RenderingObject>> &objects, float &tNear, const RenderingObject *&hitObject, bool shadowRay)
{
	if (!shadowRay || tNear == 0.0)
	{
		tNear = K_INFINITY;
	}
	std::vector<std::unique_ptr<RenderingObject>>::const_iterator iter = objects.begin();
	for (; iter != objects.end(); ++iter) {
		float t = K_INFINITY;
		if ((*iter)->intersect(ray, t) && t < tNear) {
			//Don't cast shadows on transparent surfaces.
			if (shadowRay && iter->get()->getColor().getTransparency() > VERY_SMALL_NUMBER) continue;
			hitObject = iter->get();
			tNear = t;
		}
	}
	return (hitObject != nullptr);
}
/* This function decides how much of a refraction should be colored by the
  refraction of light and how much should be a reflection at the surface of the object*/
void fresnel(const glm::vec3 &incidentVector, const glm::vec3 &normalOfRefractiveSurface, const GLfloat &ior, GLfloat &kr)
{
	GLfloat cosi = glm::clamp( glm::dot(incidentVector, normalOfRefractiveSurface), (GLfloat)-1,(GLfloat) 1);
	GLfloat etai = 1, etat = ior;
	if (cosi > 0)
	{
		std::swap(etai, etat);
	}
	// Compute sini using Snell's law
	GLfloat sint = etai / etat * glm::sqrt(glm::max(0.f, 1 - cosi * cosi));
	// Total internal reflection
	if (sint >= 1) {
		kr = 1;
	}
	else {
		GLfloat cost = glm::sqrt(glm::max(0.f, 1 - sint * sint));
		cosi = glm::abs(cosi);
		GLfloat Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		GLfloat Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		kr = (Rs * Rs + Rp * Rp) / 2;
	}
	// As a consequence of the conservation of energy, transmittance is given by:
	// kt = 1 - kr;
}
/*Sourced from: https://sourceforge.net/p/rasterrain/code/ci/master/tree/main.cpp#l32
 This writes to the BMP file I render the scene for, rendering to a picture allows flexibility in what I do with my rendered scene.*/
void saveBMP(const char *filename, int w, int h, int dpi, RGBType *data) {
	FILE *f;
	int area = w*h;
	int s = 4 * area;
	int filesize = PIXEL_ARRAY_OFFSET + s;
	int m = static_cast<int>(INCHES_IN_A_METER);

	int ppm = dpi*m;
	//BM - BM – Windows   -   The size fo the BMP file - offset
	unsigned char bmpfileheader[BITMAP_FILE_HEADER_SIZE] = { 'B','M', 0,0,0,0,	0,0,0,0,	PIXEL_ARRAY_OFFSET,0,0,0 };
	unsigned char bmpinfoheader[BITMAP_INFO_HEADER_SIZE] = { BITMAP_INFO_HEADER_SIZE,0,0,0,	0,0,0,0,	0,0,0,0,	NUMBER_OF_COLOR_PLANES,0,BITS_PER_PIXEL,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(w);
	bmpinfoheader[5] = (unsigned char)(w >> 8);
	bmpinfoheader[6] = (unsigned char)(w >> 16);
	bmpinfoheader[7] = (unsigned char)(w >> 24);
	
	int negativeHeight = -h; //For filling from top to bottom
	bmpinfoheader[8] = (unsigned char)(negativeHeight);
	bmpinfoheader[9] = (unsigned char)(negativeHeight >> 8);
	bmpinfoheader[10] = (unsigned char)(negativeHeight >> 16);
	bmpinfoheader[11] = (unsigned char)(negativeHeight >> 24);

	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s >> 8);
	bmpinfoheader[23] = (unsigned char)(s >> 16);
	bmpinfoheader[24] = (unsigned char)(s >> 24);

	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm >> 8);
	bmpinfoheader[27] = (unsigned char)(ppm >> 16);
	bmpinfoheader[28] = (unsigned char)(ppm >> 24);

	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm >> 8);
	bmpinfoheader[31] = (unsigned char)(ppm >> 16);
	bmpinfoheader[32] = (unsigned char)(ppm >> 24);

	f = fopen(filename, "wb");

	fwrite(bmpfileheader, 1, BITMAP_FILE_HEADER_SIZE, f);
	fwrite(bmpinfoheader, 1, BITMAP_INFO_HEADER_SIZE, f);

	for (int i = 0; i < area; i++) {
		RGBType rgb = data[i];

		double red = (data[i].r) * 255;
		double green = (data[i].g) * 255;
		double blue = (data[i].b) * 255;

		unsigned char color[3] = { (int)glm::floor(blue),(int)glm::floor(green),(int)glm::floor(red) };

		fwrite(color, 1, 3, f);
	}

	fclose(f);
}
/*Utility print functions*/
void printMat4(glm::mat4 mat4Object)
{
	std::cout << "Object: \n";
	std::cout << mat4Object[0].x << "\t" << mat4Object[0].y << "\t" << mat4Object[0].z << "\t" << mat4Object[0].w << "\n";
	std::cout << mat4Object[1].x << "\t" << mat4Object[1].y << "\t" << mat4Object[1].z << "\t" << mat4Object[1].w << "\n";
	std::cout << mat4Object[2].x << "\t" << mat4Object[2].y << "\t" << mat4Object[2].z << "\t" << mat4Object[2].w << "\n";
	std::cout << mat4Object[3].x << "\t" << mat4Object[3].y << "\t" << mat4Object[3].z << "\t" << mat4Object[3].w << "\n";
}
void printVector(glm::vec3 vector)
{
	printf("Vector print %3f,%3f,%3f\n", vector.x, vector.y, vector.z);
}
void printVector(glm::vec4 vector)
{
	printf("Vector print: %3f,%3f,%3f,%3f\n", vector.x, vector.y, vector.z, vector.w);
}

/*Purely for demo purposes*/
int welcomeAndGetSceneNumber()
{
	printf("Hi There, welcome to Matthew's Ray Tracer. You have an option of drawing a selection of scenes: \n");
	printf("Scene 1 ~10 seconds render - a long room lit by a single pointlight with three spheres casting shadows on the walls around it. The simplest scene in this demo. \n");
	printf("Scene 2 ~20 seconds render - a long mirrored forward wall reflecting red, green and blue pointlights lighting a white ball and a white light lighting a patch of the wall behind the camera. \n");
	printf("Scene 3 ~25 seconds render - an X axis aligned series of reflective spheres, reflecting a minimual scene of floor and wall. \n");
	printf("Scene 4 ~60 seconds render - a hall of mirrors with spheres aligned on the -Z axis. The roof, front wall and sides are all mirrors. Recommended depth is 10 for this scene.\n");
	printf("Scene 5 ~80 seconds render - the same hall of spheres as 4 but with three transparent light refracting spheres floating in front of the scene with hard walls.\n");
	printf("Scene 6 ~20 seconds render - shows a sphere lodged in a transparent floor with pointlight and directional light illuminating it.\n");
	printf("Render times based on time taken for my pc to process this. Your times may vary.\n");
	printf("\n\nSometimes the output can stick - mash the space bar in this window every 20 seconds or so to check if it's complete rendering\n");

	std::string choiceString;
	int sceneSelectionNumber = 0;
	printf("\n\nEnter the number of the scene you would like to render: \n");
	while (1)
	{
		std::getline(std::cin, choiceString);
		std::stringstream(choiceString) >> sceneSelectionNumber;
		printf("Scene selection number is : %i\n", sceneSelectionNumber);
		if (sceneSelectionNumber > 0 && sceneSelectionNumber <= 6)
		{
			break;
		}
		else
		{
			printf("Invalid scene number, try entering again (must be between 1 and 6 inclusive): \n");
		}
	}
	return sceneSelectionNumber;
}


void renderSceneOne(std::vector<std::unique_ptr<RenderingObject>> &objectList, std::vector<std::unique_ptr<Light>> & lightList)
{
	//Planes
	//																			--Position--					--Color--			--Normal--	
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.0, 0.0, -100.0, 1.0), glm::vec3(1.0, 0.0, 1.0), glm::vec3(0.0, 0.0, 1.0)))); //Back wall
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.0, -40.0, 0.0, 1.0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0, 1.0, 0.0)))); // Floor
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(-40.0, 0.0, 0.0, 1.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0)))); //Left wall
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(40.0, 0.0, 0.0, 1.0), glm::vec3(0.0, 0.5, 1.0), glm::vec3(-1.0, 0.0, 0.0)))); //Right wall
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.00, 40.0, 0.0, 1.0), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, -1.0, 0.0)))); //Roof
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.00, 0.0, 10.0, 1.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, -1.0))));//Wall behind camera
	
	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(-20, -15, -60, 1.0), 10, glm::vec3(1.0, 0.0, 0.0))));
	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(20, -15, -60, 1.0), 10, glm::vec3(0.0, 1.0, 0.0))));
	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(0, 15, -60, 1.0), 10, glm::vec3(0.0, 0.0, 1.0))));

	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(0, 0, -55, 1.0), glm::vec3(1.0, 1.0, 1.0), 0.9, 100)));
}

void renderSceneTwo(std::vector<std::unique_ptr<RenderingObject>> &objectList, std::vector<std::unique_ptr<Light>> & lightList)
{
	//Planes
	//																			--Position--					--Color--			--Normal--	--Reflectivity--
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.0, 0.0, -100.0, 1.0), glm::vec3(1.0, 0.0, 1.0), glm::vec3(0.0, 0.0, 1.0), 1.0))); //Back wall - mirror
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.0, -40.0, 0.0, 1.0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0, 1.0, 0.0), 0.0))); // Floor
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(-40.0, 0.0, 0.0, 1.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0), 0.0))); //Left wall
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.00, 0.0, 10.0, 1.0), glm::vec3(0.3, 0.6, 0.6), glm::vec3(0.0, 0.0, -1.0), 0.0)));//Wall behind camera
	//Spheres
	//objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(-20, -20, -40, 1.0), 5, glm::vec3(1.0, 0.0, 0.0))));
	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(10, -10, -50, 1.0), 5, glm::vec3(1.0, 1.0, 1.0))));
	//objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4( 20,   0, -40, 1.0), 5, glm::vec3(0.0, 0.0, 1.0))));
	//objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4( 40,  10, -40, 1.0), 5, glm::vec3(1.0, 1.0, 0.0))));
	//objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4( 60,  20,  10, 1.0), 5, glm::vec3(0.0, 1.0, 1.0))));

	//Lights
	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(-20, -20, -50, 1.0), glm::vec3(1.0, 0.0, 0.0), 1.0, 100)));
	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(10, -20, -50, 1.0), glm::vec3(0.0, 1.0, 0.0), 1.0, 100)));
	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(40, -20, -50, 1.0), glm::vec3(0.0, 0.0, 1.0), 1.0, 100)));
	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(50, 40, 0.0, 1.0), glm::vec3(1.0, 1.0, 1.0), 1.0, 100)));
}

void renderSceneThree(std::vector<std::unique_ptr<RenderingObject>> &objectList, std::vector<std::unique_ptr<Light>> & lightList)
{
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.0, 0.0, -100.0, 1.0), glm::vec3(1.0, 0.0, 1.0), glm::vec3(0.0, 0.0, 1.0), 0.0))); //Back wall - mirror
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.0, -25.0, 0.0, 1.0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0, 1.0, 0.0), 0.0))); // Floor

	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(-60, 0, -60, 1.0), 10, glm::vec4(1.0, 0.0, 0.0, 0.0), 1.0, 0.0)));
	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(-30, 0, -60, 1.0), 10, glm::vec4(1.0, 0.0, 0.0, 0.0), 1.0, 0.0)));
	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(0, 0, -60, 1.0), 10, glm::vec4(1.0, 0.0, 0.0, 0.0), 1.0, 0.0)));
	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(30, 0, -60, 1.0), 10, glm::vec4(1.0, 0.0, 0.0, 0.0), 1.0, 0.0)));
	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(60, 0, -60, 1.0), 10, glm::vec4(1.0, 0.0, 0.0, 0.0), 1.0, 0.0)));

	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(0, 20, -20, 1.0), glm::vec3(1.0, 1.0, 1.0), 1.0, 100)));
	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(0, 20, -40, 1.0), glm::vec3(1.0, 1.0, 1.0), 1.0, 100)));
	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(0, 20, -60, 1.0), glm::vec3(1.0, 1.0, 1.0), 1.0, 100)));
	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(0, 20, -80, 1.0), glm::vec3(1.0, 1.0, 1.0), 1.0, 100)));


}

void renderSceneFour(std::vector<std::unique_ptr<RenderingObject>> &objectList, std::vector<std::unique_ptr<Light>> & lightList)
{
	//																			--Position--					--Color--			--Normal--	--Reflectivity--
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.0, 0.0, -100.0, 1.0), glm::vec3(1.0, 0.0, 1.0), glm::vec3(0.0, 0.0, 1.0), 1.0))); //Back wall
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.0, -25.0, 0.0, 1.0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0, 1.0, 0.0), 0.0))); // Floor
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(-20.0, 0.0, 0.0, 1.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0), 1.0))); //Left wall
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(20.0, 0.0, 0.0, 1.0), glm::vec3(0.0, 0.5, 1.0), glm::vec3(-1.0, 0.0, 0.0), 1.0))); //Right wall
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.00, 40.0, 0.0, 1.0), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, -1.0, 0.0), 1.0))); //Roof
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.00, 0.0, 10.0, 1.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, -1.0), 0.0)));//Wall behind camera

	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(0, -20, -10, 1.0), 10, glm::vec4(1.0, 0.0, 0.0, 0.0), 0.0, 0.0)));
	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(0, -20, -30, 1.0), 10, glm::vec4(1.0, 1.0, 0.0, 0.0), 0.0, 0.0)));
	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(0, -20, -50, 1.0), 10, glm::vec4(1.0, 0.0, 1.0, 0.0), 0.0, 0.0)));
	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(0, -20, -70, 1.0), 10, glm::vec4(0.0, 1.0, 0.0, 0.0), 0.0, 0.0)));
	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(0, -20, -90, 1.0), 10, glm::vec4(0.0, 1.0, 1.0, 0.0), 0.0, 0.0)));

	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(0, 20, -20, 1.0), glm::vec3(1.0, 1.0, 1.0), 1.0, 100)));
	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(0, 20, -40, 1.0), glm::vec3(1.0, 1.0, 1.0), 1.0, 100)));
	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(0, 20, -60, 1.0), glm::vec3(1.0, 1.0, 1.0), 1.0, 100)));
	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(0, 20, -80, 1.0), glm::vec3(1.0, 1.0, 1.0), 1.0, 100)));
}

void renderSceneFive(std::vector<std::unique_ptr<RenderingObject>> &objectList, std::vector<std::unique_ptr<Light>> & lightList)
{
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.0, 0.0, -100.0, 1.0), glm::vec3(1.0, 0.0, 1.0), glm::vec3(0.0, 0.0, 1.0), 0.0))); //Back wall
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.0, -25.0, 0.0, 1.0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0, 1.0, 0.0), 0.0))); // Floor
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(-30.0, 0.0, 0.0, 1.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0), 0.0))); //Left wall
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(30.0, 0.0, 0.0, 1.0), glm::vec3(0.0, 0.5, 1.0), glm::vec3(-1.0, 0.0, 0.0), 0.0))); //Right wall
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.00, 40.0, 0.0, 1.0), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, -1.0, 0.0), 0.0))); //Roof
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.00, 0.0, 10.0, 1.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, -1.0), 0.0)));//Wall behind camera

	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(	0,  5, -30, 1.0), 10, glm::vec4(1.0, 0.0, 0.0, 1.0), 1.0, 1.4)));
	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(-15, -10, -30, 1.0), 10, glm::vec4(1.0, 0.0, 0.0, 1.0), 1.0, 1.4)));
	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4( 15, -10, -30, 1.0), 10, glm::vec4(1.0, 0.0, 0.0, 1.0), 1.0, 1.4)));
	//objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(0, -5, -15, 1.0), 5, glm::vec4(1.0, 0.0, 0.0, 1.0), 1.0, 1.4)));

	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(0, -20, -30, 1.0), 10, glm::vec4(1.0, 1.0, 0.0, 0.0), 0.0, 0.0)));
	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(0, -20, -50, 1.0), 10, glm::vec4(1.0, 0.0, 1.0, 0.0), 0.0, 0.0)));
	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(0, -20, -70, 1.0), 10, glm::vec4(0.0, 1.0, 0.0, 0.0), 0.0, 0.0)));
	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(0, -20, -90, 1.0), 10, glm::vec4(0.0, 1.0, 1.0, 0.0), 0.0, 0.0)));

	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(0, 20, -20, 1.0), glm::vec3(1.0, 1.0, 1.0), 1.0, 100)));
	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(0, 20, -40, 1.0), glm::vec3(1.0, 1.0, 1.0), 1.0, 100)));
	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(0, 20, -60, 1.0), glm::vec3(1.0, 1.0, 1.0), 1.0, 100)));
	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(0, 20, -80, 1.0), glm::vec3(1.0, 1.0, 1.0), 1.0, 100)));
}

void renderSceneSix(std::vector<std::unique_ptr<RenderingObject>> &objectList, std::vector<std::unique_ptr<Light>> & lightList)
{
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.0, 0.0, -100.0, 1.0), glm::vec3(1.0, 0.0, 1.0), glm::vec3(0.0, 0.0, 1.0)))); //Back wall
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(0.0, -40.0, 0.0, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec3(0.0, 1.0, 0.0), 1.0, 1.4))); // Floor
	objectList.push_back(std::unique_ptr<RenderingObject>(new Plane(glm::vec4(-40.0, 0.0, 0.0, 1.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0)))); //Left wall

	objectList.push_back(std::unique_ptr<RenderingObject>(new AlgorithmicSphere(glm::vec4(0, -40, -80, 1.0), 20, glm::vec3(0.0, 1.0, 1.0))));

	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(0, 20, -55, 1.0), glm::vec3(1.0, 0.0, 0.0), 1.0, 100)));
	lightList.push_back(std::unique_ptr<Light>(new PointLight(glm::vec4(0, 20, -45, 1.0), glm::vec3(0.0, 1.0, 0.0), 1.0, 100)));
	lightList.push_back(std::unique_ptr<Light>(new DirectionalLight(glm::vec4(-0.20, -1, -0.25, 1.0), glm::vec3(1.0, 1.0, 1.0), 0.75)));
}