#include "cscene.h"

void CScene::load_file(IParser* parser, const std::string& filename) {
	parser->parse(filename);
	m_objects = parser->get_objects();
	m_camera = parser->get_camera();
	m_lighters = parser->get_lighters();
}

void CScene::render(double backlight, bool gpu_process, bool testing, bool fullscreen) {
	if(!gpu_process) {
		CKDTreeCPU kdtree(m_objects);
		CRenderer renderer(m_camera, &kdtree, m_width, m_height);
		renderer.render(backlight, m_lighters, testing, fullscreen);
	}
}

void CScene::load_object(IObject3D* object) {
	m_objects.push_back(object);
}
