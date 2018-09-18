#include "CylinderY.h"
#include "Quadric.h"
#include "Disk.h"

CylinderY::CylinderY(const glm::vec3 &position, float rad, float H,
							const color &material)
	: Surface(material), center(position), radius(rad), height(H), top(glm::vec3(position.x,position.y + H / 2,position.z), glm::vec3(0, 1, 0), rad, material), bot(glm::vec3(position.x,position.y - H / 2,position.z), glm::vec3(0, -1, 0), rad, material)
{
}

HitRecord CylinderY::findClosestIntersection(const glm::vec3 &rayOrigin,
										const glm::vec3 &rayDirection) 
{
	HitRecord theHit;
	QuadricParameters qParams = { 1 / (radius*radius),0,1 / (radius*radius),0,0,0,0,0,0,-1 };
	QuadricSurface cyl(qParams, center, material);
	HitRecord hits[2];
	int numInter = cyl.findIntersections(rayOrigin, rayDirection, hits);
	if (numInter == 0)
	{
		theHit.t = FLT_MAX;
	}
	else if (numInter == 1)
	{
		if (hits[0].interceptPoint.y > center.y + height / 2 || hits[0].interceptPoint.y < center.y - height / 2)
		{
			theHit.t = FLT_MAX;
		}
		else
		{
			theHit = hits[0];
		}
	}
	else
	{
		HitRecord close;
		HitRecord Far;
		if (hits[0].t < hits[1].t)
		{
			close = hits[0];
			Far = hits[1];
		}
		else
		{
			close = hits[1];
			Far = hits[0];
		}

		if (close.interceptPoint.y > center.y + height / 2 || close.interceptPoint.y < center.y - height / 2)
		{
			if (Far.interceptPoint.y > center.y + height / 2 || Far.interceptPoint.y < center.y - height / 2)
			{
				theHit.t = FLT_MAX;
			}
			else
			{
				theHit = Far;
			}
		}
		else
		{
			theHit = close;
		}
	}
	
	HitRecord theHit2 = (this->top).findClosestIntersection(rayOrigin, rayDirection);
	HitRecord theHit3 = (this->bot).findClosestIntersection(rayOrigin, rayDirection);

	if (theHit.t < theHit2.t && theHit.t < theHit3.t)
	{
		return theHit;
	}
	else if (theHit2.t < theHit.t && theHit2.t < theHit3.t)
	{
		return theHit2;
	}
	else
	{
		return theHit3;
	}
	

}

