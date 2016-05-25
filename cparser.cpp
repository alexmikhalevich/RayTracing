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
			CMaterial mat;
			mat.set_color(CColor(r, g, b));
			CSphere* sphere = new CSphere(radius, CPoint3D(c_x, c_y, c_z), mat);
			m_objects.push_back(sphere);	
		}
		else if(cur_s == "camera") {
			double p_x, p_y, p_z, v_x, v_y, v_z;
			s_stream >> p_x >> p_y >> p_z >> v_x >> v_y >> v_z;
			m_camera = CCamera(CPoint3D(p_x, p_y, p_z), CPoint3D(v_x, v_y, v_z));
		}
		else if(cur_s == "lighter") {
			double i, p_x, p_y, p_z;
			s_stream >> i >> p_x >> p_y >> p_z;
			m_lighters.push_back(CLighter(CPoint3D(p_x, p_y, p_z), i));
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

std::vector<CLighter>& CCustomParser::get_lighters() {
	return m_lighters;
}

void CRTParser::ParseMaterials(FILE* file) {
	char field[4096];
	if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
	
	while(strcmp(field, "endmaterials") != 0) {
		if(strcmp(field, "entry") == 0) {
			if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
			while(strcmp(field, "endentry") != 0) {
				std::string name;
				CMaterial mat;
				if(strcmp(field, "name") == 0) {
					if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
					name = std::string(field);
				}
				else if(strcmp(field, "color") == 0) {
					char r[4], g[4], b[4];
					if(fscanf(file, "%s %s %s", r, g, b) == EOF) throw new ExEOF();
					mat.set_color(CColor(atoi(r), atoi(g), atoi(b)));
				}
				else if(strcmp(field, "alpha") == 0) {
					if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
					mat.set_alpha(atof(field));
				}
				else if(strcmp(field, "reflect") == 0) {
					if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
					mat.set_reflection(atof(field));
				}
				else if(strcmp(field, "refract") == 0) {
					if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
					mat.set_refraction(atof(field));
				}
				if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
				m_materials.insert(std::pair<std::string, CMaterial>(name, mat));
			}
		}
		if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
	}
}

void CRTParser::ParseViewport(FILE* file) {	//TODO: set camera here
	char field[4096];
	if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
	CPoint3D origin, topleft, bottomleft, topright;
	while(strcmp(field, "endviewport") != 0) {
		if(strcmp(field, "origin") == 0) {
			char x[100], y[100], z[100];
			if(fscanf(file, "%s %s %s", x, y, z) == EOF) throw new ExEOF();
			origin = CPoint3D(atof(x), atof(y), atof(z));
		}
		else if(strcmp(field, "topleft") == 0) {
			char x[100], y[100], z[100];
			if(fscanf(file, "%s %s %s", x, y, z) == EOF) throw new ExEOF();
			topleft = CPoint3D(atof(x), atof(y), atof(z));
		}
		else if(strcmp(field, "bottomleft") == 0) {
			char x[100], y[100], z[100];
			if(fscanf(file, "%s %s %s", x, y, z) == EOF) throw new ExEOF();
			bottomleft = CPoint3D(atof(x), atof(y), atof(z));
		}
		else if(strcmp(field, "topright") == 0) {
			char x[100], y[100], z[100];
			if(fscanf(file, "%s %s %s", x, y, z) == EOF) throw new ExEOF();
			topright = CPoint3D(atof(x), atof(y), atof(z));
		}
		if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
	}
}

void CRTParser::ParseLighters(FILE* file) {
	char field[4096];
	if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
	double ref_power, ref_dst;
	bool check[2] = { false, false };
	while(strcmp(field, "endlights") != 0) {
		if(strcmp(field, "reference") == 0) {	//TODO: normalize lighters here
			if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
			while(strcmp(field, "endreference")) {
				if(strcmp(field, "power") == 0) {
					if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
					ref_power = atof(field);
				}
				else if(strcmp(field, "distance") == 0) {
					if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
					ref_dst = atof(field);
				}
				if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
			}
		}
		else if(strcmp(field, "point") == 0) {
			if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
			while(strcmp(field, "endpoint") != 0) {
				CLighter lighter(CPoint3D(), 1.0);
				if(strcmp(field, "coords") == 0) {
					char x[100], y[100], z[100];
					if(fscanf(file, "%s %s %s", x, y, z) == EOF) throw new ExEOF();
					lighter.set_position(CPoint3D(atof(x), atof(y), atof(z)));
					if(check[0]) throw new ExInvalidLighter();
					check[0] = true;
				}
				else if(strcmp(field, "power") == 0) {
					if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
					lighter.set_intensity(atof(field));
					if(check[1]) throw new ExInvalidLighter();
					check[1] = true;
				}
				if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
			}
			if(!check[0] || !check[1]) throw new ExInvalidLighter();
			check[0] = false;
			check[1] = false;
		}
		if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
	}
}

void CRTParser::ParseGeometry(FILE* file) {
	char field[4096];
	bool check_sphere[3] = { false, false, false };
	bool check_tri[4] = { false, false, false, false };
	bool check_quad[5] = { false, false, false, false, false };
	int c_t = 0, c_q = 0;
	if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
	while(strcmp(field, "endgeometry") != 0) {
		if(strcmp(field, "sphere") == 0) {
			CSphere* sphere = new CSphere();
			if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
			while(strcmp(field, "endsphere")) {
				if(strcmp(field, "coords") == 0) {
					char x[100], y[100], z[100];
					if(fscanf(file, "%s %s %s", x, y, z) == EOF) throw new ExEOF();
					sphere->set_center(CPoint3D(atof(x), atof(y), atof(z)));
					if(check_sphere[0]) throw new ExInvalidSphere();
					check_sphere[0] = true;
				}
				else if(strcmp(field, "radius") == 0) {
					char r[100];
					if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
					sphere->set_radius(atof(r));
					if(check_sphere[1]) throw new ExInvalidSphere();
					check_sphere[1] = true;
				}
				else if(strcmp(field, "material") == 0) {
					if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
					sphere->set_material(m_materials.at(field));
					if(check_sphere[2]) throw new ExInvalidSphere();
					check_sphere[2] = true;
				}
				if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
			}
			if(!check_sphere[0] || !check_sphere[1] || !check_sphere[2]) throw new ExInvalidSphere();
			check_sphere[0] = false;
			check_sphere[1] = false;
			check_sphere[2] = false;
			m_objects.push_back(sphere);
		}
		else if(strcmp(field, "triangle") == 0) {
			CTriangle* triangle = new CTriangle();
			std::vector<CPoint3D> vert;
			if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
			while(strcmp(field, "endtriangle") != 0) {
				if(strcmp(field, "vertex") == 0) {
					char x[100], y[100], z[100];
					if(fscanf(file, "%s %s %s", x, y, z) == EOF) throw new ExEOF();
					vert.push_back(CPoint3D(atof(x), atof(y), atof(z)));
					if(check_tri[c_t]) throw new ExInvalidTriangle();
					check_tri[c_t] = true;
					++c_t;
					if(c_t > 3) throw new ExInvalidTriangle();
				}
				else if(strcmp(field, "material") == 0) {
					if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
					triangle->set_material(m_materials.at(field));
					if(check_tri[3]) throw new ExInvalidTriangle();
					check_tri[3] = true;
				}
				if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
			}
			if(!check_tri[0] || !check_tri[1] || !check_tri[2] || !check_tri[3]) throw new ExInvalidTriangle();
			check_tri[0] = false;
			check_tri[1] = false;
			check_tri[2] = false;
			check_tri[3] = false;
			c_t = 0;
			triangle->set_vertices(vert);
			m_objects.push_back(triangle);
		}
		else if(strcmp(field, "quadrangle") == 0) {
			CQuadrangle* quadrangle = new CQuadrangle();
			std::vector<CPoint3D> vert;
			if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
			while(strcmp(field, "endquadrangle") != 0) {
				if(strcmp(field, "vertex") == 0) {
					char x[100], y[100], z[100];
					if(fscanf(file, "%s %s %s", x, y, z) == EOF) throw new ExEOF();
					vert.push_back(CPoint3D(atof(x), atof(y), atof(z)));
					if(check_quad[c_q]) throw new ExInvalidQuadrangle();
					check_quad[c_q] = true;
					++c_q;
					if(c_q > 4) throw new ExInvalidQuadrangle();
				}
				else if(strcmp(field, "material") == 0) {
					if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
					if(check_quad[4]) throw new ExInvalidQuadrangle();
					quadrangle->set_material(m_materials.at(field));
					check_quad[4] = true;
				}
				if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
			}
			if(!check_quad[0] || !check_quad[1] || !check_quad[2] || !check_quad[3] || !check_quad[4]) throw new ExInvalidQuadrangle();
			check_quad[0] = false;
			check_quad[1] = false;
			check_quad[2] = false;
			check_quad[3] = false;
			check_quad[4] = false;
			c_q = 0;
			quadrangle->set_vertices(vert);
			m_objects.push_back(quadrangle);
		}
		if(fscanf(file, "%s", field) == EOF) throw new ExEOF();
	}
}
void CRTParser::parse(const std::string& filename) {
	FILE* source = fopen(filename.c_str(), "r");
	char* buffer = new char(10);
	std::map<std::string, CMaterial> materials;

	while(fscanf(source, "%s", buffer) != EOF) {
		if(strcmp(buffer, "viewport") == 0) 
			ParseViewport(source);
		else if(strcmp(buffer, "materials") == 0) 
			ParseMaterials(source);
		else if(strcmp(buffer, "lights") == 0) 
			ParseLighters(source);
		else if(strcmp(buffer, "geometry") == 0)
			ParseGeometry(source);
	}
	delete buffer;	
}

std::vector<IObject3D*>& CRTParser::get_objects() {
	return m_objects;
}

CCamera CRTParser::get_camera() const {
	return m_camera;
}

std::vector<CLighter>& CRTParser::get_lighters() {
	return m_lighters;
}
