#include "RayTracer.h"

RayTracer::RayTracer(FrameBuffer & cBuffer, color defaultColor)
	:colorBuffer(cBuffer), defaultColor(defaultColor), recursionDepth(2) {

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

void RayTracer::calculatePerspectiveViewingParameters(unsigned int windows,const float verticalFieldOfViewDegrees) {
	// Set the set the topLimit, bottomLimit, rightLimit, 
	// leftLimit, distToPlane, nx, and ny data members
	// TODO
	nx = (float)colorBuffer.getWindowWidth()/2;
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
} // end setPerspectiveOriginAndDirection

void RayTracer::raytraceScene(const std::vector<std::shared_ptr<Surface>> & surfaces, const std::vector<std::shared_ptr<LightSource>> & lights,unsigned int aliasing, unsigned int window) 
{
	this->surfacesInScene = surfaces;
	this->lightsInScene = lights;
	color C(0.0f, 0.0f, 0.0f, 0.0f);
	for (int x = 0; x < nx; x++) 
	{

		for (int y = 0; y < colorBuffer.getWindowHeight(); y++) 
		{
			for (unsigned int i = 0; i < aliasing; i++)
			{
				for (unsigned int j = 0; j < aliasing; j++)
				{
					setPerspectiveRayOriginAndDirection(x+(i/aliasing), y+(j/aliasing));
					C += traceIndividualRay(rayOrigin, rayDirection,lights,1);
				}
			}
			C = color(C.r / (aliasing*aliasing), C.g / (aliasing*aliasing), C.b / (aliasing*aliasing), C.a);
			if (y == 0 || y >= colorBuffer.getWindowHeight() - 1 || x == 0 || x >= nx - 1)
			{
				C = color(0.5f, 0.5f, 0.5f, 1.0f);
			}

			colorBuffer.setPixel(x +(nx*(window-1)) , y, C);
			C = color(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}
	// Iterate through each and every pixel in the rendering window
	// TODO


} // end raytraceScene

color RayTracer::traceIndividualRay(const glm::vec3 &e, const glm::vec3 &d,const std::vector<std::shared_ptr<LightSource>> & lights,int recursionLevel) {
	// Find surface intersection that is closest to 'e' in the direction 'd.'
	// TODO
	HitRecord save;
	float distance = FLT_MAX;
	color result = defaultColor;
	for (unsigned int i = 0; i < surfacesInScene.size(); i++) 
	{
		HitRecord hit = surfacesInScene[i]->findClosestIntersection(e, d);
		
		if (hit.t != FLT_MAX && hit.t < distance) 
		{
			
				save = hit;
				distance = hit.t;
				result = hit.material;
		}
	}

	if (distance != FLT_MAX)
	{
		for (unsigned int i = 0; i < surfacesInScene.size(); i++)
		{
			glm::vec3 start;
			if (dynamic_cast<PositionalLight *>(lights[0].get()))
			{
				PositionalLight *ptr = dynamic_cast<PositionalLight *>(lights[0].get());
				glm::vec3 direction(glm::normalize(ptr->lightPosition - save.interceptPoint));
				if (glm::dot(save.surfaceNormal, direction) < 0)
				{
					start = glm::vec3(save.interceptPoint - (EPSILON* save.surfaceNormal));
				}
				else
				{
					start = glm::vec3(save.interceptPoint + (EPSILON* save.surfaceNormal));
				}
				HitRecord other = surfacesInScene[i]->findClosestIntersection(start, direction);
				if (other.t < glm::distance(ptr->lightPosition, start) && other.t > 0)
				{
					return color(0.0f, 0.0f, 0.0f, 1.0f);
				}
			}
		}
	}
	

	return result;


} // end traceRay