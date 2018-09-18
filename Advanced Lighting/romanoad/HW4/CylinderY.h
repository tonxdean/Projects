#pragma once
#include <glm/glm.hpp>
#include "Surface.h"

class CylinderY : public Surface {
public:
	CylinderY(const glm::vec3 &position, float rad, float H,
						const Material &material);
	~CylinderY() {}
	virtual HitRecord findClosestIntersection(const glm::vec3 &origin, const glm::vec3 &dir);
protected:
	glm::vec3 center;
	float radius, height;
};