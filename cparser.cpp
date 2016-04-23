#include "cparser.h"
 
void CCustomParser::parse(const std::string& filename) {
	std::ifstream in(filename);
	std::string s;
	while(std::getline(in, s)) {
		std::istringstream s_stream(s);
		std::string cur_s;
		s_stream >> cur_s;
		if(cur_s == "sphere") {
			double c_x, c_y, c_z, radius;
			int r, g, b;
			s_stream >> c_x >> c_y >> c_z >> radius;
			s_stream >> r >> g >> b;
			CSphere* sphere = new CSphere(radius, CPoint3D(c_x, c_y, c_z), CColor(r, g, b));
			m_objects.push_back(sphere);	
		}
		else if(cur_s == "camera") {
			double p_x, p_y, p_z, v_x, v_y, v_z;
			s_stream >> p_x >> p_y >> p_z >> v_x >> v_y >> v_z;
			m_camera = CCamera(CPoint3D(p_x, p_y, p_z), CPoint3D(v_x, v_y, v_z));
		}
		else {
			std::cerr << "[EE]: Parsing error." << std::endl;
			exit(-1);	
		}
	}
}

std::vector<IObject3D*>& CCustomParser::get_objects() {
	return m_objects;
}

CCamera CCustomParser::get_camera() const {
	return m_camera;
}

