#ifndef CRENDERER_H
#define CRENDERER_H
#include <SDL2/SDL.h>
#include "ikdtree.h"
#include "ccamera.hpp"

const double PROJ_PLANE_DIST = 10.0;

class CRenderer {
	private:
		CCamera m_camera;
		IKDTree* m_tree;
		int m_width;
		int m_height;
		SDL_Renderer* renderer;
		SDL_Window* window;
		CMatrix3D m_rot_matrix;
		CVector3D GetRay(const CPoint3D& screen_point);
		void FindRotMatrix();
	public:
		CRenderer() = delete;
		CRenderer(const CCamera& c, IKDTree* t, int w = -1, int h = -1) : m_camera(c), m_tree(t), m_width(w), m_height(h) {}
		~CRenderer();
		void render();
};
#endif //CRENDERER_H
