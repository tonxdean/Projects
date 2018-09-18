#pragma once

#include <iostream>
#include "BasicIncludesAndDefines.h"
#include "HitRecord.h"
#include "Surface.h"

HitRecord findIntersection(const glm::vec3 &e, const glm::vec3 &d, const std::vector<std::shared_ptr<Surface>> & surfaces);
float max(float one, float two);

struct PositionalLight {
	bool isOn;
	bool atten=true;
	glm::vec3 factor = glm::vec3(1, 0, 0);
	glm::vec3 lightPosition;
	virtual color illuminate(const HitRecord &hit, const glm::vec3 &eyeVector, bool inShadow) const 
	{
		glm::vec3 l(glm::normalize(lightPosition - hit.interceptPoint));
		glm::vec3 n(glm::normalize(hit.surfaceNormal));
		glm::vec3 v(glm::normalize(eyeVector-hit.interceptPoint));
		glm::vec3 r( glm::normalize( -l - (2 * glm::dot(-l, n)*n) ) );
		if (isOn && !inShadow)
		{
			float shad;
			if (inShadow)
			{
				shad = 0;
			}
			else
			{
				shad = 1;
			}
			if (atten)
			{
				color diffuse = shad *(1 / (factor.x + factor.y*hit.t + factor.z * factor.z * hit.t * hit.t))* (max(0, glm::dot(n, l)))*hit.material.diffuse;
				color specular = shad * (1 / (factor.x + factor.y*hit.t + factor.z * factor.z * hit.t * hit.t)) * pow((max(0, glm::dot(r, v))), hit.material.shininess)*hit.material.specular;
				return hit.material.ambient + diffuse + specular;
			}
			else
			{
				color diffuse = shad* (max(0, glm::dot(n, l)))*hit.material.diffuse;
				color specular = shad * pow((max(0, glm::dot(r, v))), hit.material.shininess)*hit.material.specular;
				return hit.material.ambient + diffuse + specular;
			}
		}
		else
		{
			return color(0, 0, 0, 1);
		}
	}
	PositionalLight(const glm::vec3 &pos)
		: lightPosition(pos) {
	}
	friend std::ostream &operator << (std::ostream &os, const PositionalLight &pl);

	
};


struct SpotLight : public PositionalLight {
	SpotLight(const glm::vec3 &position,const glm::vec3 &direc)
		: PositionalLight(position) {
		direction = glm::normalize(direc);
	}
	float fov = 45;
	glm::vec3 direction;
	virtual color illuminate(const HitRecord &hit, const glm::vec3 &eyeVector, bool inShadow) const 
	{
		glm::vec3 l(glm::normalize(lightPosition - hit.interceptPoint));
		glm::vec3 n(glm::normalize(hit.surfaceNormal));
		glm::vec3 v(glm::normalize(eyeVector - hit.interceptPoint));
		glm::vec3 r(glm::normalize(-l - (2 * glm::dot(-l, n)*n)));
		if (isOn)
		{
			float shad;
			if (inShadow)
			{
				shad = 0;
			}
			else
			{
				shad = 1;
			}
			if (atten)
			{
				color diffuse =  shad * (1 / (factor.x + factor.y*hit.t + factor.z * factor.z * hit.t * hit.t))* (max(0, glm::dot(n, l)))*hit.material.diffuse;
				color specular = shad * (1 / (factor.x + factor.y*hit.t + factor.z * factor.z * hit.t * hit.t)) * pow((max(0, glm::dot(r, v))), hit.material.shininess)*hit.material.specular;
				return color (in(hit) * (hit.material.ambient + diffuse + specular));
			}
			else
			{
				color diffuse = shad * (max(0, glm::dot(n, l)))*hit.material.diffuse;
				color specular = shad * pow((max(0, glm::dot(r, v))), hit.material.shininess)*hit.material.specular;
				return color(in(hit) * (hit.material.ambient + diffuse + specular));
			}
		}
		else
		{
			return color(0, 0, 0, 1);
		}
	}
	const float in(const HitRecord &hit) const
	{
		glm::vec3 place = glm::normalize(hit.interceptPoint - lightPosition);
		if (abs(glm::dot(place, direction)) > cos(glm::radians(fov/2)))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	friend std::ostream &operator << (std::ostream &os, const SpotLight &pl);
};


