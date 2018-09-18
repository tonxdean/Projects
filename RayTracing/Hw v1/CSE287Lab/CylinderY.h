#pragma once
#include <glm/glm.hpp>
#include "Surface.h"
#include "Disk.h"

class CylinderY : public Surface {
public:
	CylinderY(const glm::vec3 &position, float rad, float H,
						const color &material);
	~CylinderY() {}
	virtual HitRecord findClosestIntersection(const glm::vec3 &origin, const glm::vec3 &dir);
protected:
	glm::vec3 center;
	float radius, height;
	Disk top;
	Disk bot;
};