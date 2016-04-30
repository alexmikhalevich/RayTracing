#ifndef CRENDERER_H
#define CRENDERER_H
#include <SDL2/SDL.h>
#include <chrono>
#include "ikdtree.h"
#include "ccamera.hpp"
#include "clighter.hpp"

const double PROJ_PLANE_DIST = 1.0;
const double INTENSITY_COEFF = 10.0;

class CRenderer {
	private:
		CCamera m_camera;
		IKDTree* m_tree;
		int m_width;
		int m_height;
		SDL_Renderer* renderer;
		SDL_Window* window;
		CVector3D GetRay(const CPoint3D& screen_point) const;
		inline CColor RayTracing(double backlight, const std::vector<CLighter>& lighters, const CVector3D& ray, const CColor& background) const;
		inline double GetIntencity(const CVector3D& normal, const CLighter& l, const CPoint3D& intersection) const;
	public:
		CRenderer() = delete;
		CRenderer(const CCamera& c, IKDTree* t, int w = -1, int h = -1) : m_camera(c), m_tree(t), m_width(w), m_height(h) {}
		~CRenderer();
		void render(double backlight, const std::vector<CLighter>& lighters, bool testing, bool fullscreen);
};
#endif //CRENDERER_H
