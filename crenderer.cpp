#include "crenderer.h"

CRenderer::~CRenderer() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

CVector3D CRenderer::GetRay(const CPoint3D& screen_point) const {
	CPoint3D plane_point;
	plane_point.set_x(m_camera.get_position().get_x() 
			+ m_camera.get_view().get_coordinates().get_x() * PROJ_PLANE_DIST / m_camera.get_view().length());
	plane_point.set_y((screen_point.get_x() - m_width / 2) / 300);	//TODO: replace this constant
	plane_point.set_z((screen_point.get_y() - m_height / 2) / 300);

	CVector3D ray_vector(m_camera.get_position(), plane_point);
	return ray_vector; 
}

inline double CRenderer::GetIntencity(const CVector3D& normal, const CLighter& lighter, const CPoint3D& intersection) const {
	CVector3D ray(m_camera.get_position(), intersection);
	double result = lighter.get_intensity(intersection);
	double distance = lighter.get_position().get_distance(intersection);
	result /= (distance * distance);
	double cos_angle = (normal.get_coordinates().get_x() * ray.get_coordinates().get_x()
		+ normal.get_coordinates().get_y() * ray.get_coordinates().get_y()
		+ normal.get_coordinates().get_z() * ray.get_coordinates().get_z()) / ray.length();
	result *= std::abs(cos_angle);
	result *= INTENSITY_COEFF;
	return result;
}

inline CColor CRenderer::RayTracing(double backlight, const std::vector<CLighter>& lighters, const CVector3D& ray, const CColor& background) const {
	CColor res_color, intersection_color;
	IObject3D* obj = NULL;
	CPoint3D intersection, tmp;
	if(lighters.empty()) return background;

	if(m_tree->find_intersection(ray, obj, intersection)) {
		intersection_color = obj->get_intersection_color(intersection);
		for(CLighter lighter : lighters) {
			if(m_tree->find_intersection(CVector3D(intersection, lighter.get_position()), obj, tmp)) {
				intersection_color *= backlight / 100;
				res_color += intersection_color;
			}
			else { 
				intersection_color *= GetIntencity(obj->get_normal_vector(intersection), lighter, intersection);
				res_color += intersection_color;
			}
		}
		return res_color;
	}
	else return background;
}

void CRenderer::render(double backlight, const std::vector<CLighter>& lighters, bool testing) {
	std::chrono::steady_clock::time_point t1;
	SDL_Event event;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(m_width, m_height, 0, &window, &renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	if(testing) t1 = std::chrono::steady_clock::now();
	for(int x = 0; x < m_width; ++x) {
		for(int y = 0; y < m_height; ++y) {
			CVector3D vect = GetRay(CPoint3D(x, y, 0));
			CColor c = RayTracing(backlight, lighters, vect, CColor(0, 0, 0));
			SDL_SetRenderDrawColor(renderer, c.get_red(), c.get_green(), c.get_blue(), 0); 
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
	SDL_RenderPresent(renderer);

	if(testing) {
		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		std::cout << "Rendering cycle working time: " << time_span.count() << "s" << std::endl;
	}

	while(1) {
		if(SDL_PollEvent(&event) && event.type == SDL_QUIT) break;
	}	
}
