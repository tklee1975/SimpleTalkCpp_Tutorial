#include "precompiledHeader.h"
#include "MyRayTracer.h"

void MyRayTracer::init(const MyVec2f& screenSize, const MyMatrix4f& projMatrix, const MyMatrix4f& modelview) {
	m_screenSize = screenSize;
	m_projMatrix = projMatrix;

	m_invProjMatrix = projMatrix.inverse();
	m_invModelview  = modelview.inverse();

	m_modelview = modelview;
}

MyRay3f MyRayTracer::getRay(float x, float y) {
	MyRay3f ray;
	ray.origin.set(0, 0, 0);

	auto s = MyVec2f(x, y) / m_screenSize * 2 - 1;
	s.y = -s.y;

	auto a = m_invProjMatrix.mulPoint(MyVec4f(s.x, s.y, 0, 1)).toVec3();
	auto b = m_invProjMatrix.mulPoint(MyVec4f(s.x, s.y, 1, 1)).toVec3();

	ray.direction = (b - a).normalize();

	ray = m_invModelview.mul(ray);
	return ray;
}
