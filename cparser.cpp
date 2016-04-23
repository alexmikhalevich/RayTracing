#include "cparser.h"
 
void CCustomParser::parse(const std::string& filename) {
	std::ifstream in(filename);
	std::string s;
	while(!in.eof()) {
		in >> s;
		if(s == "sphere") {
			double c_x, c_y, c_z, radius;
			int r, g, b;
			in >> c_x >> c_y >> c_z >> radius;
			in >> r >> g >> b;
			CSphere* sphere = new CSphere(radius, CPoint3D(c_x, c_y, c_z), CColor(r, g, b));
			m_objects.push_back(sphere);	
		}
		else if(s == "camera") {
			double p_x, p_y, p_z, v_x, v_y, v_z;
			in >> p_x >> p_y >> p_z >> v_x >> v_y >> v_z;
			m_camera = CCamera(CPoint3D(p_x, p_y, p_z), CPoint3D(v_x, v_y, v_z));
		}
		else {
			std::cerr << "[EE]: Parsing error." << std::endl;
			return;
		
		}
	}
}

std::vector<IObject3D*>& CCustomParser::get_objects() {
	return m_objects;
}

CCamera CCustomParser::get_camera() const {
	return m_camera;
}

