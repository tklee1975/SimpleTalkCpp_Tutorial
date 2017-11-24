#include "precompiledHeader.h"
#include "MyRay.h"
#include "MyMatrix.h"

MyRay3f MyRay3f::unprojectFromInverseMatrix(const MyMatrix4f& invProj, const MyMatrix4f& invModelview, const MyVec2f& pointOnScreen, const MyVec2f& screenSize) {

	auto s = pointOnScreen / screenSize * 2 - 1;
	s.y = -s.y;

	auto a = MyVec4f(s.x, s.y, 0, 1);
	auto b = MyVec4f(s.x, s.y, 1, 1);

	a = invProj.mulPoint(a);
	b = invProj.mulPoint(b);

	a = invModelview.mulPoint(a);
	b = invModelview.mulPoint(b);

	MyRay3f o;
	o.origin = a.toVec3();
	o.direction = (b.toVec3() - o.origin).normalize();
	return o;
}

bool MyRay3f::raycast(HitResult& outResult, const MySphere& sphere, float maxDistance) {
	auto v = sphere.center - origin; // vector from origin to sphere center
	auto t = v.dot(direction); // distance 't' to closest point to sphere center
	if (t < 0)
		return false;

	float d2 = v.dot(v) - (t * t); // squared distance between closest point to sphere center
	float r2 =  sphere.radius * sphere.radius;

	if (d2 > r2)
		return false;

	auto q = std::sqrt(r2 - d2);

	auto t0 = t + q;
	auto t1 = t - q;

	bool hasResult = false;

	float dis;
	if (t0 >= 0 && t0 < maxDistance) {
		dis = t0;
		hasResult = true;
	}
	
	if (t1 >= 0 && t1 < maxDistance) {
		if (!hasResult || t1 < dis)
		{
			dis = t1;
			hasResult = true;
		}
	}
	
	if (!hasResult) {
		return false;
	}

	auto pt = origin + direction * dis;

	outResult.point = pt;
	outResult.normal = pt - sphere.center;
	outResult.distance = dis;
	outResult.hasResult = true;
	return true;
}

bool MyRay3f::raycast(HitResult& outResult, const MyPlane& plane, float maxDistance) {
	auto s = direction.dot(plane.normal);
	
	if (my_math_eq(s,0))
		return false; // parallel to the plane

	if (s > 0)
		return false; // back face

	auto distance = (plane.distance - origin.dot(plane.normal)) / s;
	if (distance < 0 || distance > maxDistance)
		return false;

	outResult.distance = distance;
	outResult.point  = origin + (direction * distance);
	outResult.normal = plane.normal;
	outResult.hasResult = true;
	return true;
}

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
	outResult.hasResult = true;
	return true;
}
