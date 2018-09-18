#include "RayTracer.h"

RayTracer::RayTracer(FrameBuffer & cBuffer, color defaultColor)
	:colorBuffer(cBuffer), defaultColor(defaultColor) {
}

RayTracer::~RayTracer(void) {
}

void RayTracer::setCameraFrame(const glm::vec3 & viewPosition, const glm::vec3 & viewingDirection, glm::vec3 up) {
	// Calculate and set the the w, u, and vdata members and 
	// set the eye data member to the viewing position.
	// TODO

	eye = viewPosition;
	// Squaring up a basis
	w = glm::normalize(-viewingDirection);
	u = glm::normalize(glm::cross(up, w));
	v = glm::normalize(glm::cross(w, u));
} // end setCameraFrame

void RayTracer::calculatePerspectiveViewingParameters(const float verticalFieldOfViewDegrees) {
	nx = (float)colorBuffer.getWindowWidth();
	ny = (float)colorBuffer.getWindowHeight();
	topLimit = 1;
	bottomLimit = -topLimit;
	rightLimit = topLimit * nx / ny;
	leftLimit = -rightLimit;
	float rads = glm::radians(verticalFieldOfViewDegrees / 2);
	distToPlane = 1.0f / std::tan(rads);

} // end calculatePerspectiveViewingParameters

void RayTracer::calculateOrthographicViewingParameters(const float viewPlaneHeight) {
	topLimit = fabs(viewPlaneHeight) / 2.0f;

	rightLimit = topLimit * ((float)colorBuffer.getWindowWidth() / colorBuffer.getWindowHeight()); // Set r based on aspect ratio and height of plane

	// Make view plane symetrical about the viewing direction
	leftLimit = -rightLimit;
	bottomLimit = -topLimit;

	// Calculate the distance between pixels in the horizontal and vertical directions
	nx = (float)colorBuffer.getWindowWidth();
	ny = (float)colorBuffer.getWindowHeight();

	distToPlane = 0.0; // Rays start on the view plane

} // end calculateOrthographicViewingParameters

glm::vec2 RayTracer::getProjectionPlaneCoordinates(const int x, const int y) {
	// Page 75
	// Calculate and return the u and v data members as the x an y coordinates
	// of the 
	glm::vec2 uvScalarValues;
	uvScalarValues.x = leftLimit + (rightLimit - leftLimit) * (x + 0.5f) / nx;
	uvScalarValues.y = bottomLimit + (topLimit - bottomLimit) * (y + 0.5f) / ny;
	// TODO
	return uvScalarValues;
}

void RayTracer::setOrthoRayOriginAndDirection(const int x, const int y) {
	glm::vec2 uv = getProjectionPlaneCoordinates(x, y);

	// Page 75
	rayDirection = glm::normalize(-w);
	rayOrigin = eye + uv.x * u + uv.y * v;

} // end setOrthoOriginAndDirection

void RayTracer::setPerspectiveRayOriginAndDirection(const int x, const int y) {
	glm::vec2 uv = getProjectionPlaneCoordinates(x, y);
	rayDirection = glm::normalize(-distToPlane*w + uv.x*u + uv.y*v);
	rayOrigin = eye;
}

void RayTracer::raytraceScene(const std::vector<std::shared_ptr<Surface>> & surfaces,
					const std::vector<std::shared_ptr<PositionalLight>> & lights) {
	this->surfacesInScene = surfaces;
	this->lightsInScene = lights;
	for (int x = 0; x < colorBuffer.getWindowWidth(); x++) {
		for (int y = 0; y < colorBuffer.getWindowHeight(); y++) {
			setPerspectiveRayOriginAndDirection(x, y);
			color C = traceIndividualRay(rayOrigin, rayDirection, lights);
			colorBuffer.setPixel(x, y, C);
		}
	}
} // end raytraceScene

color RayTracer::traceIndividualRay(const glm::vec3 &e, const glm::vec3 &d,
	const std::vector < std::shared_ptr<PositionalLight>> &lights) {
	float distance = FLT_MAX;
	HitRecord theHit;
	for (unsigned int i = 0; i < surfacesInScene.size(); i++) {
		HitRecord thisHit = surfacesInScene[i]->findClosestIntersection(e, d);
		if (thisHit.t != FLT_MAX && thisHit.t < theHit.t) {
			theHit = thisHit;
		}
	}
	if (theHit.t == FLT_MAX) {
		return defaultColor;
	}
	color result = color(0, 0, 0, 1);
	for (unsigned int i = 0; i < lightsInScene.size(); i++) 
	{
		glm::vec3 shadowFeelerOrigin = theHit.interceptPoint + 0.01f * theHit.surfaceNormal;
		glm::vec3 shadowFeelerDirection = glm::normalize(lightsInScene[i]->lightPosition - shadowFeelerOrigin);
		HitRecord r = findIntersection(shadowFeelerOrigin, shadowFeelerDirection, surfacesInScene);
		float dist = glm::distance(lightsInScene[i]->lightPosition, shadowFeelerOrigin);
		result += lightsInScene[i]->illuminate(theHit, d, r.t < dist);
	}
	if (result.r > 1)
	{
		result.r = 1;
	}
	if (result.g > 1)
	{
		result.g = 1;
	}
	if (result.b > 1)
	{
		result.b = 1;
	}
	if (result.a > 1)
	{
		result.a = 1;
	}
	return result;
} // end traceRay