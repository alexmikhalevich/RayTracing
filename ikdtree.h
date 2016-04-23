#ifndef IKDTREE
#define IKDTREE
#include "geometry.hpp"
#include "iobject3d.h"

class IKDTree {
	public:
		virtual bool find_intersection(const CVector3D& vector, IObject3D* nearest_object, CPoint3D& nearest_intersect) = 0;
		virtual ~IKDTree() {}
};
#endif //IKDTREE
