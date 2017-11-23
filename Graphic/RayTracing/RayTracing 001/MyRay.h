#pragma once

#include "MyVec.h"
#include "MyGeometry.h"

class MyRay3f {
public:
	MyRay3f() = default;

	MyRay3f(const MyVec3f& origin_, const MyVec3f& direction_) {
		origin = origin_;
		direction = direction_;
	}

	class HitResult {
	public:
		MyVec3f point;
		MyVec3f normal;
		float   distance = std::numeric_limits<float>::max();
	};

	bool raycast(HitResult& outResult, const MyTriangle& tri,   float maxDistance);
	bool raycast(HitResult& outResult, const MyPlane&    plane, float maxDistance);

	MyVec3f origin;
	MyVec3f direction;
};