#include "csphere.h"
#include <iostream>
bool CSphere::intersect(const CVector3D& ray_vector, CPoint3D& intersection) {
	const double v_x = ray_vector.get_coordinates().get_x();
	const double v_y = ray_vector.get_coordinates().get_y();
	const double v_z = ray_vector.get_coordinates().get_z();
	const double b_x = ray_vector.get_begin().get_x();
	const double b_y = ray_vector.get_begin().get_y();
	const double b_z = ray_vector.get_begin().get_z();
	const double c_x = m_center.get_x();
	const double c_y = m_center.get_y();
	const double c_z = m_center.get_z();

	const double a = v_x * v_x + v_y * v_y + v_z * v_z;
	const double b = 2 * (v_x * (b_x - c_x) + v_y * (b_y - c_y) + v_z * (b_z - c_z));
	const double c = c_x * c_x + c_y * c_y + c_z * c_z 
		+ b_x * b_x + b_y * b_y + b_z * b_z 
		- 2 * (b_x * c_x + b_y * c_y + b_z * c_z) - m_radius * m_radius;
	const double d = b * b - 4 * a * c;
	
	if(d < 0) return false;
	const double t_1 = (-b + std::sqrt(d)) / (2 * a);
	const double t_2 = (-b - std::sqrt(d)) / (2 * a);
	const double min_t = std::min(t_1, t_2);
	const double max_t = std::max(t_1, t_2);
	const double t = (min_t > EPS) ? min_t : max_t;
	if(t < EPS) return false;

	intersection = CPoint3D(b_x + t * v_x, b_y + t * v_y, b_z + t * v_z);
	return true;
}

CColor CSphere::get_intersection_color(const CPoint3D& intersection) {
	return m_color; //TODO: add support for textures
}

CVector3D CSphere::get_normal_vector(const CPoint3D& intersection) {
	CVector3D n(m_center, intersection);
	n.normalize();
	return n;
}
