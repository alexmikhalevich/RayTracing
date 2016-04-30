#ifndef CSPHERE_H
#define CSPHERE_H
#include "iobject3d.h"

class CSphere : public IObject3D {
	private:
		double m_radius;
		CPoint3D m_center;
		CColor m_color;
		double m_reflecting_ability;
	public:
		CSphere() : m_radius(1.0), m_center(CPoint3D()), m_color(CColor()), m_reflecting_ability(0.0) {}
		CSphere(double r, const CPoint3D& c, const CColor& col, double ref = 0.0) : m_radius(r), m_center(c), m_color(col), m_reflecting_ability(ref) {
			if(m_reflecting_ability < 0.0) m_reflecting_ability = 0.0;
			if(m_reflecting_ability > 100.0) m_reflecting_ability = 100.0;
		}
		CSphere(const CSphere& s) : m_radius(s.get_radius()), m_center(s.get_center()), m_color(s.get_color()), m_reflecting_ability(s.get_reflecting()) {}
		~CSphere() {}

		double get_radius() const { return m_radius; }
		CPoint3D get_center() const { return m_center; }
		CColor get_color() const { return m_color; }
		double get_reflecting() const { return m_reflecting_ability; }
		void set_radius(double r) { m_radius = r; }
		void set_center(const CPoint3D& c) { m_center = c; }
		void set_color(const CColor& c) { m_color = c; }
		void set_reflecting(double ref) { m_reflecting_ability = ref; }

		virtual bool intersect(const CVector3D& ray_vector, CPoint3D& intersection);
		virtual CColor get_intersection_color(const CPoint3D& intersection);
		virtual CVector3D get_normal_vector(const CPoint3D& intersection);
		virtual CPoint3D get_min_boundary_point() const;
		virtual CPoint3D get_max_boundary_point() const;

		void operator=(const CSphere& sphere) {
			m_radius = sphere.get_radius();
			m_center = sphere.get_center();
			m_color = sphere.get_color();
			m_reflecting_ability = sphere.get_reflecting();
		}
};
#endif //CSPHERE_H
