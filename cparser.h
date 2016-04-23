#ifndef CPARSER_H
#define CPARSER_H
#include <fstream>
#include <sstream>
#include "iparser.h"
#include "csphere.h"
#include "ccamera.hpp"

class CCustomParser : public IParser {
	private:
		std::vector<IObject3D*> m_objects;
		CCamera m_camera;
	public:
		CCustomParser() : m_camera(CCamera(CPoint3D(0, 0, 0), CPoint3D(1, 0, 0))) {}
		virtual void parse(const std::string& filename);
		virtual std::vector<IObject3D*>& get_objects();
		virtual CCamera get_camera() const;
};
#endif //CPARSER_H
