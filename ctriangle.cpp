#include "ctriangle.h"
#include <iostream>
bool CTriangle::intersect(const CVector3D& ray_vector, CPoint3D& intersection) {
	CVector3D n = get_normal();
	double scalar_product = CVector3D::dot_product(n, ray_vector);
	if(fabs(scalar_product) < EPS) return false;
	double d = -(m_vertices[0].get_x() * n.get_coordinates().get_x() 
			+ m_vertices[0].get_y() * n.get_coordinates().get_y()
			+ m_vertices[0].get_z() * n.get_coordinates().get_z());
	double coeff = -(n.get_coordinates().get_x() * ray_vector.get_coordinates().get_x()
		+ n.get_coordinates().get_y() * ray_vector.get_coordinates().get_y()
		+ n.get_coordinates().get_z() * ray_vector.get_coordinates().get_z() + d) / scalar_product; 
	if(coeff < EPS) return false;

	double x = ray_vector.get_begin().get_x() + ray_vector.get_coordinates().get_x() * coeff;
	double y = ray_vector.get_begin().get_y() + ray_vector.get_coordinates().get_y() * coeff;
	double z = ray_vector.get_begin().get_z() + ray_vector.get_coordinates().get_z() * coeff;
	CPoint3D intr(x, y, z);
	if(CVector3D::same_clock_dir(CVector3D(m_vertices[0], m_vertices[1]), CVector3D(m_vertices[0], intr), n)
			&& CVector3D::same_clock_dir(CVector3D(m_vertices[1], m_vertices[2]), CVector3D(m_vertices[1], intr), n)
			&& CVector3D::same_clock_dir(CVector3D(m_vertices[2], m_vertices[0]), CVector3D(m_vertices[2], intr), n)) {
		intersection = intr;
		return true;
	}
	else return false;
}

CColor CTriangle::get_intersection_color(const CPoint3D& intersection) {
	return m_material.get_color(); //TODO: add support for textures
}

CVector3D CTriangle::get_normal_vector(const CPoint3D& intersection) {
	return get_normal(); 
}

CPoint3D CTriangle::get_max_boundary_point() const {
	return CPoint3D(std::max(m_vertices[0].get_x(), std::max(m_vertices[1].get_x(), m_vertices[2].get_x())),
			std::max(m_vertices[0].get_y(), std::max(m_vertices[1].get_y(), m_vertices[2].get_y())),
			std::max(m_vertices[0].get_z(), std::max(m_vertices[1].get_z(), m_vertices[2].get_z())));
}

CPoint3D CTriangle::get_min_boundary_point() const {
	return CPoint3D(std::min(m_vertices[0].get_x(), std::min(m_vertices[1].get_x(), m_vertices[2].get_x())),
			std::min(m_vertices[0].get_y(), std::min(m_vertices[1].get_y(), m_vertices[2].get_y())),
			std::min(m_vertices[0].get_z(), std::min(m_vertices[1].get_z(), m_vertices[2].get_z())));
}
