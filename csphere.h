#ifndef CSPHERE_H
#define CSPHERE_H
#include "iobject3d.h"
#include "cmaterial.hpp"

class CSphere : public IObject3D {
	private:
		double m_radius;
		CPoint3D m_center;
		CMaterial m_material;
	public:
		CSphere() : m_radius(1.0), m_center(CPoint3D()), m_material(CMaterial()) {}
		CSphere(double r, const CPoint3D& c, const CMaterial& mat) : m_radius(r), m_center(c), m_material(mat) {}
		CSphere(const CSphere& s) : m_radius(s.get_radius()), m_center(s.get_center()), m_material(s.get_material()) {}
		~CSphere() {}

		double get_radius() const { return m_radius; }
		CPoint3D get_center() const { return m_center; }
		CMaterial get_material() const { return m_material; }
		void set_radius(double r) { m_radius = r; }
		void set_center(const CPoint3D& c) { m_center = c; }
		void set_material(const CMaterial& mat) { m_material = mat; }

		virtual bool intersect(const CVector3D& ray_vector, CPoint3D& intersection);
		virtual CColor get_intersection_color(const CPoint3D& intersection);
		virtual CVector3D get_normal_vector(const CPoint3D& intersection);
		virtual CPoint3D get_min_boundary_point() const;
		virtual CPoint3D get_max_boundary_point() const;

		void operator=(const CSphere& sphere) {
			m_radius = sphere.get_radius();
			m_center = sphere.get_center();
			m_material = sphere.get_material();
		}
};
#endif //CSPHERE_H
