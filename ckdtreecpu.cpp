#include "ckdtreecpu.h"

CVoxel::CVoxel(const std::vector<IObject3D*>& objects) {
	if(objects.size() == 0) {
		m_bottom = CPoint3D(-1.0, -1.0, -1.0);
		m_top = CPoint3D(1.0, 1.0, 1.0);
	}

	for(IObject3D* obj : objects) {
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

inline int CVoxel::contained_elements(const std::vector<IObject3D*>& objects) const {
	int result = 0;
	for(IObject3D* obj : objects) 
		if(contains(obj)) ++result;
	return result;
}

CKDTreeCPU::CKDTreeCPU(std::vector<IObject3D*>& objects) {
	m_bounding_box = CVoxel(objects);
	m_root->build(objects, m_bounding_box, 0);
}

double CKDNode::MinimizeSAH(const std::vector<IObject3D*>& obj, EPlane plane, double bestSAH, const CVoxel& voxel, 
		double Ssplit, double Snot_split, CPoint3D& plane_coord, EPlane& res_plane) const {
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
		curSAH = (Ssplit + l * Snot_split) * left_vox.contained_elements(obj) 
			+ (Ssplit + r * Snot_split) * right_vox.contained_elements(obj)
			+ SPLIT_COST;

		if(curSAH < bestSAH) {
			resSAH = curSAH;
			res_plane = plane;
			plane_coord = cur_plane_coord;
		}
	}

	return resSAH;
}

inline void CKDNode::find_plane(const std::vector<IObject3D*>& objects, const CVoxel& voxel,
		int depth, EPlane& plane, CPoint3D& plane_coord) {
	if((depth >= MAX_DEPTH) || (objects.size() <= OBJECTS_IN_LEAF)) {
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

	double bestSAH = objects.size();
	plane = EPlane::NONE;
	bestSAH = MinimizeSAH(objects, EPlane::XY, bestSAH, voxel, Sxy, Sxz + Syz, plane_coord, plane);
	bestSAH = MinimizeSAH(objects, EPlane::XZ, bestSAH, voxel, Sxz, Sxy + Syz, plane_coord, plane);
	bestSAH = MinimizeSAH(objects, EPlane::YZ, bestSAH, voxel, Syz, Sxz + Sxy, plane_coord, plane);
}

inline CKDNode* CKDNode::build(std::vector<IObject3D*>& objects, const CVoxel& voxel, int depth) {
	EPlane plane;
	CPoint3D plane_coord;
	find_plane(objects, voxel, depth, plane, plane_coord);
	
	if(plane == EPlane::NONE)
		return MakeLeaf(objects);

	CVoxel left_vox;
	CVoxel right_vox;
	voxel.split(plane, plane_coord, left_vox, right_vox);
	CKDNode* left_node = build(objects, left_vox, depth + 1);
	CKDNode* right_node = build(objects, right_vox, depth + 1);
	CKDNode* node = new CKDNode(plane, plane_coord, left_node, right_node); 

	return node;
}

inline CKDNode* CKDNode::MakeLeaf(const std::vector<IObject3D*>& objects) {
	CKDNode* node = new CKDNode();
	if(objects.size()) m_objects = objects;
	return node;
}
