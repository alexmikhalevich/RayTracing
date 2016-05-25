#ifndef CTRIANGLE_H
#define CTRIANGLE_H
#include "iobject3d.h"
#include "geometry.hpp"
#include "cmaterial.hpp"

class CTriangle : public IObject3D {
	private:
		std::vector<CPoint3D> m_vertices;
		CMaterial m_material;
		CVector3D m_normal;
	public:
		CTriangle() : m_material(CMaterial()) {
			m_vertices.push_back(CPoint3D());
			m_vertices.push_back(CPoint3D());
			m_vertices.push_back(CPoint3D());
		}
		CTriangle(const std::vector<CPoint3D>& vert, const CMaterial& mat) :  m_vertices(vert), m_material(mat) {
			m_normal = CVector3D(vert[0], vert[1]) * CVector3D(vert[1], vert[2]);
		}
		CTriangle(const CTriangle& t) : m_vertices(t.get_vertices()), m_material(t.get_material()) {
			m_normal = CVector3D(m_vertices[0], m_vertices[1]) * CVector3D(m_vertices[1], m_vertices[2]);
		}
		~CTriangle() {}

		std::vector<CPoint3D> get_vertices() const { return m_vertices; }
		CMaterial get_material() const { return m_material; }
		CVector3D get_normal() const { return m_normal; }
		void set_vertices(const std::vector<CPoint3D>& vert) { m_vertices = vert; }
		void set_material(const CMaterial& mat) { m_material = mat; }
		virtual bool intersect(const CVector3D& ray_vector, CPoint3D& intersection);
		virtual CColor get_intersection_color(const CPoint3D& intersection);
		virtual CVector3D get_normal_vector(const CPoint3D& intersection);
		virtual CPoint3D get_min_boundary_point() const;
		virtual CPoint3D get_max_boundary_point() const;

		void operator=(const CTriangle& tr) {
			m_vertices = tr.get_vertices();
			m_material = tr.get_material();
		}
};

#endif //CTRIANGLE_H
