#include "ckdtreecpu.h"

/*
 * CVoxel - this class implements a 3D-element (some kind of parallelepiped). All scene space is
 * devided into voxels (using KD-tree) to increase the speed of ray tracing technique.
 * m_bottom - point, which determines the bottom left vertice of voxel;
 * m_top - point, which determines the top right vertice of voxel.
 */


/*
 * The constructor of voxel object.
 * begin - the iterator to the beginning of the array, which contains scene objects;
 * end - the iterator to the ending of the array, which contains scene objects.
 */
CVoxel::CVoxel(ObjIterator begin, ObjIterator end) {
	if(begin == end) {
		m_bottom = CPoint3D(-1.0, -1.0, -1.0);
		m_top = CPoint3D(1.0, 1.0, 1.0);
	}

	IObject3D* obj = *(begin);
	CPoint3D tmin_p = obj->get_min_boundary_point();
	CPoint3D tmax_p = obj->get_max_boundary_point();
	
	m_bottom.set_x(tmin_p.get_x()); 
	m_bottom.set_y(tmin_p.get_y()); 
	m_bottom.set_z(tmin_p.get_z()); 
	m_top.set_x(tmax_p.get_x()); 
	m_top.set_y(tmax_p.get_y()); 
	m_top.set_z(tmax_p.get_z()); 

	for(ObjIterator iter = begin; iter != end; ++iter) {
		obj = *iter;
		CPoint3D tmin_p = obj->get_min_boundary_point();
		CPoint3D tmax_p = obj->get_max_boundary_point();

		m_bottom.set_x(std::min(m_bottom.get_x(), tmin_p.get_x())); 
		m_bottom.set_y(std::min(m_bottom.get_y(), tmin_p.get_y())); 
		m_bottom.set_z(std::min(m_bottom.get_z(), tmin_p.get_z())); 
		m_top.set_x(std::max(m_top.get_x(), tmax_p.get_x())); 
		m_top.set_y(std::max(m_top.get_y(), tmax_p.get_y())); 
		m_top.set_z(std::max(m_top.get_z(), tmax_p.get_z())); 
	}

	m_bottom -= 1.0;
	m_top += 1.0;
}

/* 
 * Splits the voxel into two separate voxels.
 * plane - determines the section plane (XY, YZ, XZ or NONE);
 * plane_coord - the coordinate of the section plane;
 *
 * Results:
 * left_vox - the left voxel, which was separated;
 * right_vox - the right voxel, which was separated.
 */
inline void CVoxel::split(EPlane plane, const CPoint3D& plane_coord, CVoxel& left_vox, CVoxel& right_vox) const {
	CPoint3D left_top, left_bottom, right_top, right_bottom;
	left_top = m_top;
	right_top = m_top;
	left_bottom = m_bottom;
	right_bottom = m_bottom;

	switch(plane) {
		case EPlane::XY:
			left_top.set_z(plane_coord.get_z());
			right_bottom.set_z(plane_coord.get_z());
			break;
		case EPlane::XZ:
			left_top.set_y(plane_coord.get_y());
			right_bottom.set_y(plane_coord.get_y());
		case EPlane::YZ:
			left_top.set_x(plane_coord.get_x());
			right_bottom.set_x(plane_coord.get_x());
		case EPlane::NONE:
			std::cerr << "[EE]: Unable to split voxel: no line provided." << std::endl;
			exit(1);
			break;
	}

	left_vox = CVoxel(left_bottom, left_top);
	right_vox = CVoxel(right_bottom, right_top);
}

/*
 * Determines if voxel contains object.
 * object - the object, which should be checked;
 *
 * Returns value:
 * the result of checking: true or false.
 */
inline bool CVoxel::contains(IObject3D* object) const {
	CPoint3D min_p = object->get_min_boundary_point();
	CPoint3D max_p = object->get_max_boundary_point();

	return !((max_p.get_x() < m_bottom.get_x()) 
			|| (max_p.get_y() < m_bottom.get_y()) 
			|| (max_p.get_z() < m_bottom.get_z()) 
			|| (min_p.get_x() > m_top.get_x()) 
			|| (min_p.get_y() > m_top.get_y()) 
			|| (min_p.get_z() > m_top.get_z())); 
			
}

/*
 * Counts elements, which are contained in voxel.
 * begin - the iterator to the beginning of the array, which contains scene objects;
 * end - the iterator to the ending of the array, which contains scene objects.
 *
 * Returns value:
 * the number of objects, which the current voxel contains.
 */
inline int CVoxel::contained_elements(ObjIterator begin, ObjIterator end) const {
	int result = 0;
	for(ObjIterator iter = begin; iter != end; ++iter)
		if(contains(*iter)) ++result;
	return result;
}

/* Checks if the current voxel contains point.
 * point - the point, which should be checked.
 *
 * Returns value:
 * the result of checking: true or false.
 */
inline bool CVoxel::contains_point(const CPoint3D& point) const {
	return ((m_bottom.get_x() < point.get_x() && m_top.get_x() > point.get_x()
				&& m_bottom.get_y() < point.get_y() && m_top.get_y() > point.get_y()
				&& m_bottom.get_z() < point.get_z() && m_top.get_z() > point.get_z()));
}

/* Checks if some vector intersects with voxel face.
 * vector - the vector, which should be checked.
 *
 * Returns value:
 * the result of checking: true of false.
 */
inline bool CVoxel::intersects_with_vector(const CVector3D& vector) const {
	if(contains_point(vector.get_begin())) return true;

	CPoint3D intersection;
	CPoint3D plane_coord;

	plane_coord.set_z(m_bottom.get_z());
	if(vector.intersects_with_plane(EPlane::XY, plane_coord, intersection) 
			&& (intersection.get_x() > m_bottom.get_x() && intersection.get_x() < m_top.get_x())
			&& (intersection.get_y() > m_bottom.get_y() && intersection.get_y() < m_top.get_y()))
		return true;
	plane_coord.set_z(m_top.get_z());
	if(vector.intersects_with_plane(EPlane::XY, plane_coord, intersection) 
			&& (intersection.get_x() > m_bottom.get_x() && intersection.get_x() < m_top.get_x())
			&& (intersection.get_y() > m_bottom.get_y() && intersection.get_y() < m_top.get_y()))
		return true;
	plane_coord.set_y(m_bottom.get_y());
	if(vector.intersects_with_plane(EPlane::XZ, plane_coord, intersection) 
			&& (intersection.get_x() > m_bottom.get_x() && intersection.get_x() < m_top.get_x())
			&& (intersection.get_z() > m_bottom.get_z() && intersection.get_z() < m_top.get_z()))
		return true;
	plane_coord.set_y(m_top.get_y());
	if(vector.intersects_with_plane(EPlane::XZ, plane_coord, intersection) 
			&& (intersection.get_x() > m_bottom.get_x() && intersection.get_x() < m_top.get_x())
			&& (intersection.get_z() > m_bottom.get_z() && intersection.get_z() < m_top.get_z()))
	plane_coord.set_z(m_bottom.get_x());
	if(vector.intersects_with_plane(EPlane::YZ, plane_coord, intersection) 
			&& (intersection.get_z() > m_bottom.get_z() && intersection.get_z() < m_top.get_z())
			&& (intersection.get_y() > m_bottom.get_y() && intersection.get_y() < m_top.get_y()))
		return true;
	plane_coord.set_z(m_top.get_x());
	if(vector.intersects_with_plane(EPlane::YZ, plane_coord, intersection) 
			&& (intersection.get_z() > m_bottom.get_z() && intersection.get_z() < m_top.get_z())
			&& (intersection.get_y() > m_bottom.get_y() && intersection.get_y() < m_top.get_y()))
		return true;
	return false;
}

/* 
 * Counts objects, which are contained by the current voxel, and puts all such objects in the
 * beginning of the array.
 * begin - the iterator to the beginning of the array, which contains scene objects;
 * end - the iterator to the ending of the array, which contains scene objects.
 *
 * Returns value:
 * the number of elements, contained by the current voxel.
 */
inline int CVoxel::sort_and_count_contained(ObjIterator begin, ObjIterator end) const {
	ObjIterator b = begin;
	ObjIterator e = end;

	while(b != e) {
		while((b != e) && (contains(*b))) ++b;
		while((b != e) && (!contains(*e))) --e;

		IObject3D* tmp = *b;
		*b = *e;
		*e = tmp;
		++b;
		--e;
	}

	return b - begin;
}

/*
 * CKDNode - the implementation of the KD-tree's node.
 * m_plane - sectional plane; devides the voxel into two;
 * m_coordinate - coordinates of the m_plane;
 * m_left - the pointer to the left child of the current node;
 * m_right - the pointer to the right child of the current node;
 * m_begin - the iterator to the beginning of the array, which contains scene objects in the current voxel;
 * m_end - the iterator to the beginning of the array, which contains scene objects in the current voxel.
 */

/*
 * Connected with SAH heuristic - finds the minimum of this heuristic for the particular voxel to 
 * determine the best way of splitting it.
 * plane - determines the section plane (XY, YZ, XZ or NONE);
 * bestSAH - the best heuristic value, which was already calculated;
 * voxel - the voxel, which will be splitted;
 * Ssplit - the area of the voxel's face, which is parallel to the section plane;  
 * Snot_split - the sum of areas of the two other faces.
 * 
 * Results:
 * plane_coord - the coordinate of the section plane;
 * res_plane - the resulting section plane.
 *
 * Returns value:
 * the best value of SAH heuristic.
 */
double CKDNode::MinimizeSAH(EPlane plane, double bestSAH, const CVoxel& voxel, double Ssplit, 
		double Snot_split, CPoint3D& plane_coord, EPlane& res_plane) const {
	const double hx = voxel.get_top().get_x() - voxel.get_bottom().get_x();
	const double hy = voxel.get_top().get_y() - voxel.get_bottom().get_y();
	const double hz = voxel.get_top().get_z() - voxel.get_bottom().get_z();

	CVoxel left_vox;
	CVoxel right_vox;
	double curSAH;
	double resSAH = bestSAH;

	for(int i = 1; i <= MAX_SPLITS; ++i) {
		double l = i / MAX_SPLITS;
		double r = 1 - l;
		
		CPoint3D cur_plane_coord(voxel.get_bottom().get_x() + l * hx,
				voxel.get_bottom().get_y() + l * hy,
				voxel.get_bottom().get_z() + l * hz);
		voxel.split(plane, cur_plane_coord, left_vox, right_vox);
		curSAH = (Ssplit + l * Snot_split) * left_vox.contained_elements(m_begin, m_end)
			+ (Ssplit + r * Snot_split) * right_vox.contained_elements(m_begin, m_end)
			+ SPLIT_COST;

		if(curSAH < bestSAH) {
			resSAH = curSAH;
			res_plane = plane;
			plane_coord = cur_plane_coord;
		}
	}

	return resSAH;
}

/*
 * Finds the best section plane for the particular voxel.
 * voxel - the voxel, which should be splitted;
 * depth - the current recursion depth.
 * 
 * Results:
 * plane - the result section plane;
 * plane_coord - the coordinates of the result section plane.
 */
inline void CKDNode::FindPlane(const CVoxel& voxel, int depth, EPlane& plane, CPoint3D& plane_coord) {
	if((depth >= MAX_DEPTH) || (m_end - m_begin <= OBJECTS_IN_LEAF)) {
		plane = EPlane::NONE;
		return;
	}

	const double hx = voxel.get_top().get_x() - voxel.get_bottom().get_x();
	const double hy = voxel.get_top().get_y() - voxel.get_bottom().get_y();
	const double hz = voxel.get_top().get_z() - voxel.get_bottom().get_z();
	double Sxy = hx * hy;
	double Sxz = hx * hz;
	double Syz = hy * hz;
	double Ssum = Sxy + Sxz + Syz;

	//normalization
	Sxy /= Ssum;
	Sxz /= Ssum;
	Syz /= Ssum;

	double bestSAH = m_end - m_begin;
	plane = EPlane::NONE;
	bestSAH = MinimizeSAH(EPlane::XY, bestSAH, voxel, Sxy, Sxz + Syz, plane_coord, plane);
	bestSAH = MinimizeSAH(EPlane::XZ, bestSAH, voxel, Sxz, Sxy + Syz, plane_coord, plane);
	bestSAH = MinimizeSAH(EPlane::YZ, bestSAH, voxel, Syz, Sxz + Sxy, plane_coord, plane);
}

/*
 * Recursively builds the KD-tree.
 * begin - the iterator to the beginning of the array, which contains scene objects;
 * end - the iterator to the ending if the array, which contains scene objects;
 * voxel - voxel, which will be splitted;
 * depth - current recursion depth.
 *
 * Returns value:
 * a pointer to the constructed node.
 */
inline CKDNode* CKDNode::build(ObjIterator begin, ObjIterator end, const CVoxel& voxel, int depth) {
	EPlane plane;
	CPoint3D plane_coord;
	FindPlane(voxel, depth, plane, plane_coord);
	
	if(plane == EPlane::NONE)
		return MakeLeaf(begin, end);

	CVoxel left_vox;
	CVoxel right_vox;
	voxel.split(plane, plane_coord, left_vox, right_vox);
	int left_elements = left_vox.sort_and_count_contained(begin, end);
	//int right_elements = right_vox.sort_and_count_contained(begin, end);
	CKDNode* left_node = build(begin, begin + left_elements + 1, left_vox, depth + 1);
	CKDNode* right_node = build(begin + left_elements + 1, end, right_vox, depth + 1);
	CKDNode* node = new CKDNode(plane, plane_coord, left_node, right_node); 

	return node;
}

/*
 * Constructs a new leaf node.
 * begin - the iterator to the beginning of the array, which contains scene objects;
 * end - the iterator to the ending of the array, which contains scene objects.
 *
 * Returns value:
 * a pointer to the constructed leaf.
 */
inline CKDNode* CKDNode::MakeLeaf(ObjIterator begin, ObjIterator end) {
	CKDNode* node = new CKDNode();
	if(begin != end) {
		node->set_begin(begin);
		node->set_end(end);
	}
	return node;
}

/*
 * Setters for the begin-end iterators of the current CNode object
 */
inline void CKDNode::set_begin(ObjIterator b) {
	m_begin = b;
}

inline void CKDNode::set_end(ObjIterator e) {
	m_end = e;
}

/*
 * Determines the intersection of voxel and the particular vector.
 * voxel - the voxel, which may be intersected;
 * vector - the vector, which may intersect a voxel;
 *
 * Results:
 * nearest_object - the nearest object, which will be intersected by the vector;
 * nearest_intersect - the point of the intersection with the nearest_object.
 *
 * Returns value:
 * if the intersection exists - returns true, otherwise false.
 */
bool CKDNode::find_intersection(const CVoxel& voxel, const CVector3D& vector,
		IObject3D* nearest_object, CPoint3D& nearest_intersect) {
	if(m_plane == EPlane::NONE) {
		if(m_end - m_begin) {
			CPoint3D intersection;
			double min_dist = -1.0, cur_dist;
			for(ObjIterator iter = m_begin; iter != m_end; ++iter) {
				IObject3D* obj = *iter;
				if(obj->intersect(vector, intersection) 
						&& voxel.contains_point(intersection)){
					CVector3D v(vector.get_begin(), intersection);
					cur_dist = v.length();
					if((min_dist - cur_dist < EPS && min_dist - cur_dist > 0) 
							|| (min_dist < 0)) {
						nearest_object = obj;
						nearest_intersect = intersection;
						min_dist = cur_dist;
					}
				}
			}

			if(min_dist >= 0) return true;
		}
		return false;
	}

	CVoxel front_vox;
	CVoxel back_vox;

	CKDNode* front_node;
	CKDNode* back_node;

	switch(m_plane) {
		case EPlane::XY:
			if(((m_coordinate.get_z() > voxel.get_bottom().get_z()) 
					&& (m_coordinate.get_z() > vector.get_begin().get_z()))
					|| ((m_coordinate.get_z() < voxel.get_bottom().get_z())
					&& (m_coordinate.get_z() < vector.get_begin().get_z()))) {
				front_node = m_left;
				back_node = m_right;
				voxel.split(m_plane, m_coordinate, front_vox, back_vox);
			}
			else {
				front_node = m_right;
				back_node = m_left;
				voxel.split(m_plane, m_coordinate, back_vox, front_vox);
			}
			break;

		case EPlane::XZ:
			if(((m_coordinate.get_y() > voxel.get_bottom().get_y()) 
					&& (m_coordinate.get_y() > vector.get_begin().get_y()))
					|| ((m_coordinate.get_y() < voxel.get_bottom().get_y())
					&& (m_coordinate.get_y() < vector.get_begin().get_y()))) {
				front_node = m_left;
				back_node = m_right;
				voxel.split(m_plane, m_coordinate, front_vox, back_vox);
			}
			else {
				front_node = m_right;
				back_node = m_left;
				voxel.split(m_plane, m_coordinate, back_vox, front_vox);
			}
			break;

		case EPlane::YZ:
			if(((m_coordinate.get_x() > voxel.get_bottom().get_x()) 
					&& (m_coordinate.get_x() > vector.get_begin().get_x()))
					|| ((m_coordinate.get_x() < voxel.get_bottom().get_x())
					&& (m_coordinate.get_x() < vector.get_begin().get_x()))) {
				front_node = m_left;
				back_node = m_right;
				voxel.split(m_plane, m_coordinate, front_vox, back_vox);
			}
			else {
				front_node = m_right;
				back_node = m_left;
				voxel.split(m_plane, m_coordinate, back_vox, front_vox);
			}
			break;

		case EPlane::NONE:
			std::cerr << "[EE]: No plane" << std::endl;
			exit(1);
			break;
	}

	if(front_vox.intersects_with_vector(vector)
		&& front_node->find_intersection(front_vox, vector, nearest_object, nearest_intersect))
		return true;

	return (back_vox.intersects_with_vector(vector) 
			&& back_node->find_intersection(back_vox, vector, nearest_object, nearest_intersect));
}

/*
 * CKDTreeCPU - the KD-tree implementation (for processing on CPU).
 * m_bounding_box - scene bounding box;
 * m_root - the root of the KD-tree.
 */

/* Constructor of the KD-tree.
 * objects - the array of scene objects
 */
CKDTreeCPU::CKDTreeCPU(std::vector<IObject3D*>& objects) {
	m_bounding_box = CVoxel(objects.begin(), objects.end());
	m_root->build(objects.begin(), objects.end(), m_bounding_box, 0);
}

/*
 * Determines the intersection of the particular vector and some scene object (if exists).
 * vector - the vector, which may intersect a voxel;
 *
 * Results:
 * nearest_object - the nearest object, which will be intersected by the vector;
 * nearest_intersect - the point of the intersection with the nearest_object.
 *
 * Returns value:
 * if the intersection exists - returns true, otherwise false.
 */
bool CKDTreeCPU::find_intersection(const CVector3D& vector, IObject3D* nearest_object, CPoint3D& nearest_intersect) {
	return (m_bounding_box.intersects_with_vector(vector)
			&& m_root->find_intersection(m_bounding_box, vector, nearest_object, nearest_intersect));
}
