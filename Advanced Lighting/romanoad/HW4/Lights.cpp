#include "Lights.h"

HitRecord findIntersection(const glm::vec3 &e, const glm::vec3 &d, const std::vector<std::shared_ptr<Surface>> & surfaces) {
	HitRecord closestHit;
	closestHit.t = FLT_MAX;

	// Check if the ray intersects any surfaces in the scene
	for (unsigned int s = 0; s < surfaces.size(); s++) {

		HitRecord hitRec = surfaces[s]->findClosestIntersection(e, d);

		// Check if it is closer that previous intersection.
		if (hitRec.t < closestHit.t) {
			closestHit = hitRec;
		}
	}

	return closestHit;
}

std::ostream &operator << (std::ostream &os, const PositionalLight &pl) {
	os << " position " << " ";
	print(os, pl.lightPosition);
	return os;
}


std::ostream &operator << (std::ostream &os, const SpotLight &sl) {
	PositionalLight pl = (sl);
	os << pl;
	os << "Direction: ";
	print(os, sl.direction);
	os << "FOV: " << sl.fov << std::endl;
	// Spotlight specific information can be printed using sl
	return os;
}