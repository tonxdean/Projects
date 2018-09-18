#include <iostream>
#include <numeric>
#include "BasicIncludesAndDefines.h"

color getRandomColor() {
	float red = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float green = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float blue = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	return color(red, green, blue, 1.0f);

} // end getRandomColor

float max(float one, float two)
{
	if (one > two)
	{
		return one;
	}
	return two;
}


void print(std::ostream &os, const glm::vec2 & v0) {
	os << "[ " << v0.x << " " << v0.y << " ]" << std::endl;
}

void print(std::ostream &os, const glm::vec3 & v0) {
	os << "[ " << v0.x << " " << v0.y << " " << v0.z << " " << " ]" << std::endl;
}

void print(std::ostream &os, const glm::vec4 & v0) {
	os << "[ " << v0.x << " " << v0.y << " " << v0.z << " " << v0.w << " ]" << std::endl;
}

void print(std::ostream &os, const glm::mat2 & m) {
	os << "\n";

	for (int row = 0; row < 2; row++) {
		os << "|\t";
		for (int col = 0; col < 2; col++) {
			os << m[col][row] << "\t";
		}
		os << "|\n";
	}
	os << "\n";
}

void print(std::ostream &os, const glm::mat3 & m) {
	os << "\n";
	for (int row = 0; row < 3; row++) {
		{
			os << "|\t";
			for (int col = 0; col < 3; col++) {
				os << m[col][row] << "\t";
			}
		}
		os << "|\n";
	}
	os << "\n";
}

void print(std::ostream &os, const glm::mat4 & m) {
	os << "\n";

	for (int row = 0; row < 4; row++) {
		os << "|\t";
		for (int col = 0; col < 4; col++) {
			os << m[col][row] << "\t";
		}
		os << "|\n";
	}
	os << "\n";
}

// Returns the roots (0, 1, or 2 roots) that satisfy the equation:
// Ax^2 + Bx^1 + C = 0
// Roots are passed out as reference parameters.
// The return value of the function is the number of real roots (0, 1, or 2).
int solveQuadratic(float A, float B, float C, float roots[2]) {
	int numRoots;
	if (A == 0) {
		if (B == 0)
			return 0;
		else {
			roots[0] = -C / B;
			return 1;
		}
	}
	float discriminant = B*B - 4 * A * C;
	if (discriminant < 0) {
		numRoots = 0;
		roots[0] = std::numeric_limits<float>::quiet_NaN();
		roots[1] = std::numeric_limits<float>::quiet_NaN();
	} else if (discriminant == 0) {
		numRoots = 1;
		roots[0] = -B / (2 * A);
		roots[1] = std::numeric_limits<float>::quiet_NaN();
	} else {
		numRoots = 2;
		roots[0] = ((-B) - std::sqrt(discriminant)) / (2 * A);
		roots[1] = ((-B) + std::sqrt(discriminant)) / (2 * A);
	}
	return numRoots;
}

