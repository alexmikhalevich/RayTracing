#include "iobject3d.h"

class CSphere : public IObject3D {
	private:
		double m_radius;
		CPoint3D m_center;
		CColor m_color;
	public:
		CSphere() : m_radius(1.0), m_center(CPoint3D()), m_color(CColor()) {}
		CSphere(double r, const CPoint3D& c, const CColor& col) : m_radius(r), m_center(c), m_color(col) {}
		CSphere(const CSphere& s) : m_radius(s.get_radius()), m_center(s.get_center()), m_color(s.get_color()) {}
		~CSphere() {}

		double get_radius() const { return m_radius; }
		CPoint3D get_center() const { return m_center; }
		CColor get_color() const { return m_color; }
		void set_radius(double r) { m_radius = r; }
		void set_center(const CPoint3D& c) { m_center = c; }
		void set_color(const CColor& c) { m_color = c; }

		virtual bool intersect(const CVector3D& ray_vector, CPoint3D& intersection);
		virtual CColor get_intersection_color(const CPoint3D& intersection);
		virtual CVector3D get_normal_vector(const CPoint3D& intersection);
		virtual CPoint3D get_min_boundary_point() const;
		virtual CPoint3D get_max_boundary_point() const;

		void operator=(const CSphere& sphere) {
			m_radius = sphere.get_radius();
			m_center = sphere.get_center();
			m_color = sphere.get_color();
		}
};
