#include <iostream>
#include <cstdlib>
#include "iobject3d.h"

const short MAX_DEPTH = 20;
const short OBJECTS_IN_LEAF = 1;
const short SPLIT_COST = 5;
const short MAX_SPLITS = 5;

class CVoxel {
	private:
		CPoint3D m_bottom;	//two those points define the bounding box
		CPoint3D m_top;
	public:
		CVoxel() : m_bottom(CPoint3D()), m_top(CPoint3D(1.0, 1.0, 1.0)) {}
		CVoxel(const CPoint3D a, const CPoint3D& b) : m_bottom(a), m_top(b) {}
		CVoxel(double x1, double y1, double z1, double x2, double y2, double z2) 
			: m_bottom(CPoint3D(x1, y1, z1)), m_top(CPoint3D(x2, y2, z2)) {}
		CVoxel(const std::vector<IObject3D*>& objects); 
		CPoint3D get_bottom() const { return m_bottom; }
		CPoint3D get_top() const { return m_top; }
		inline void split(EPlane plane, const CPoint3D& plane_coord, CVoxel& left_vox, CVoxel& right_vox) const;
		inline bool contains(IObject3D* object) const;
		inline int contained_elements(const std::vector<IObject3D*>& objects) const;

		void operator=(const CVoxel& v) {
			m_bottom = v.get_bottom();
			m_top = v.get_top();
		}
};

class CKDNode {
	private:
		EPlane m_plane;
		CPoint3D m_coordinate;
		CKDNode* m_left;
		CKDNode* m_right;
		std::vector<IObject3D*> m_objects;
		inline CKDNode* MakeLeaf(const std::vector<IObject3D*>& objects);
		double MinimizeSAH(const std::vector<IObject3D*>& obj, EPlane plane, double bestSAH, const CVoxel& voxel, 
				double Ssplit,	double Snot_split, CPoint3D& plane_coord, EPlane& res_plane) const;
	public:
		CKDNode() : m_plane(EPlane::NONE), m_coordinate(CPoint3D()), m_left(NULL), m_right(NULL) {}
		CKDNode(EPlane p, const CPoint3D& pnt, CKDNode* l, CKDNode* r) : m_plane(p), m_coordinate(pnt), m_left(l), m_right(r) {}
		inline void find_plane(const std::vector<IObject3D*>& objects, const CVoxel& voxel,
				int depth, EPlane& plane, CPoint3D& plane_coord);
		inline CKDNode* build(std::vector<IObject3D*>& objects, const CVoxel& voxel, int depth);
};

class CKDTreeCPU {
	private:
		CVoxel m_bounding_box;
		CKDNode* m_root;
	public:
		CKDTreeCPU() = delete;
		CKDTreeCPU(std::vector<IObject3D*>& obj);
};