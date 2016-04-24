#include "crenderer.h"

CRenderer::~CRenderer() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

CVector3D CRenderer::GetRay(const CPoint3D& screen_point) {
	CPoint3D plane_point;
	plane_point.set_x(PROJ_PLANE_DIST);
	plane_point.set_y(screen_point.get_x() - m_width / 2);
	plane_point.set_z(m_height / 2 - screen_point.get_y());

	CVector3D ray_vector(m_camera.get_position(), plane_point);
	return m_rot_matrix * ray_vector;
}

void CRenderer::FindRotMatrix() {
	CVector3D cam_basis_x = m_camera.get_view();
	cam_basis_x.normalize();
	CVector3D cam_basis_y(CPoint3D(0, 0, 0), CPoint3D(-cam_basis_x.get_coordinates().get_y() / cam_basis_x.get_coordinates().get_x(), 1, 0));
	cam_basis_y.normalize();
	CVector3D cam_basis_z(CPoint3D(0, 0, 0), CPoint3D(-cam_basis_x.get_coordinates().get_z() / cam_basis_x.get_coordinates().get_x(), 0, 1));
	cam_basis_z.normalize();
	double cx_x = cam_basis_x.get_coordinates().get_x();
	double cx_y = cam_basis_x.get_coordinates().get_y();
	double cx_z = cam_basis_x.get_coordinates().get_z();
	double cy_x = cam_basis_y.get_coordinates().get_x();
	double cy_y = cam_basis_y.get_coordinates().get_y();
	double cy_z = cam_basis_y.get_coordinates().get_z();
	double cz_x = cam_basis_z.get_coordinates().get_x();
	double cz_y = cam_basis_z.get_coordinates().get_y();
	double cz_z = cam_basis_z.get_coordinates().get_z();
	CMatrix3D rot_matrix(cx_x, cy_x, cz_x, cx_y, cy_y, cz_y, cx_z, cy_z, cz_z);
	rot_matrix.inverse();
	m_rot_matrix = rot_matrix;
}

void CRenderer::render(bool testing) {
	std::chrono::steady_clock::time_point t1;
	FindRotMatrix();
	SDL_Event event;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(m_width, m_height, 0, &window, &renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	if(testing) t1 = std::chrono::steady_clock::now();

	for(int x = 0; x < m_width; ++x) {
		for(int y = 0; y < m_height; ++y) {
			IObject3D* obj = NULL;
			CPoint3D intersection;
			CVector3D vect = GetRay(CPoint3D(x, y, 0));
			if(m_tree->find_intersection(vect, obj, intersection)) {
				CColor c = obj->get_intersection_color(intersection);
				SDL_SetRenderDrawColor(renderer, c.get_red(), c.get_green(), c.get_blue(), 0); 
			}
			else 
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); 
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
