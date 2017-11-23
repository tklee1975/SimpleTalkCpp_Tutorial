#include "precompiledHeader.h"
#include "MyRay.h"

bool MyRay3f::raycast(HitResult& outResult, const MyTriangle& tri, float maxDistance) {
	MyPlane plane(tri);

	auto s = direction.dot(plane.normal);
	
	if (my_math_eq(s,0))
		return false; // parallel to the plane

	if (s < 0)
		return false; // back face

	auto distance = (plane.distance - origin.dot(plane.normal)) / s;
	if (distance < 0 || distance > maxDistance)
		return false;

	outResult.distance = distance;
	outResult.point  = origin + (direction * distance);
	outResult.normal = plane.normal;

	return true;
}

bool MyRay3f::raycast(HitResult& outResult, const MyPlane& plane, float maxDistance) {
	auto s = direction.dot(plane.normal);
	
	if (my_math_eq(s,0))
		return false; // parallel to the plane

	if (s < 0)
		return false; // back face

	auto distance = (plane.distance - origin.dot(plane.normal)) / s;
	if (distance < 0 || distance > maxDistance)
		return false;

	outResult.distance = distance;
	outResult.point  = origin + (direction * distance);
	outResult.normal = plane.normal;

	return true;
}

