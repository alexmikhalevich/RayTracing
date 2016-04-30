#ifndef CSCENE_H
#define CSCENE_H
#include <string>
#include <vector>
#include "csphere.h"
#include "cparser.h"
#include "crenderer.h"
#include "ckdtreecpu.h"
#include "ccamera.hpp"
#include "iparser.h"
#include "clighter.hpp"

class CScene {
	private:
		std::vector<IObject3D*> m_objects;
		std::vector<CLighter> m_lighters;
		CCamera m_camera;
		int m_width;
		int m_height;
	public:
		CScene(int w, int h) : m_width(w), m_height(h) {}
		CScene(const CScene& s) = delete;
		void load_file(IParser* parser, const std::string& filename);
		void load_object(IObject3D* object);
		void render(double backlight, bool gpu_process, bool testing);
};
#endif //CSCENE_H
