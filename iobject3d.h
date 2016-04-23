#ifndef IOBJECT_H
#define IOBJECT_H
#include "geometry.hpp"

class IObject3D {
	public:
		virtual bool intersect(const CVector3D& ray_vector, CPoint3D& intersection) = 0;
		virtual CColor get_intersection_color(const CPoint3D& intersection) = 0;
		virtual CVector3D get_normal_vector(const CPoint3D& intersection) = 0;
		virtual CPoint3D get_min_boundary_point() const = 0;
		virtual CPoint3D get_max_boundary_point() const = 0;
		virtual ~IObject3D() {}
};
#endif //IOBJECT_H
