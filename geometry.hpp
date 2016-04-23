#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdlib>

const double EPS = 0.0001;

enum EPlane { XY, YZ, XZ, NONE };

class CPoint3D {
	private:
		double m_x;
		double m_y;
		double m_z;
	public:
		CPoint3D() : m_x(0.0), m_y(0.0), m_z(0.0) {}
		CPoint3D(double x, double y, double z) : m_x(x), m_y(y), m_z(z) {}
		CPoint3D(const CPoint3D& point) {
			m_x = point.get_x();
			m_y = point.get_y();
			m_z = point.get_z();
		}
		
		double get_x() const { return m_x; } 
		double get_y() const { return m_y; }
		double get_z() const { return m_z; }
		void set_x(double x) { m_x = x; }
		void set_y(double y) { m_y = y; }
		void set_z(double z) { m_z = z; }

		double get_distance(const CPoint3D& p) const {
			double dx = (m_x - p.get_x()) * (m_x - p.get_x());
			double dy = (m_y - p.get_y()) * (m_y - p.get_y());
			double dz = (m_z - p.get_z()) * (m_z - p.get_z());
			return std::sqrt(dx + dy + dz);
		}
		void operator=(const CPoint3D& point) {
			m_x = point.get_x();
			m_y = point.get_y();
			m_z = point.get_z();
		}
		CPoint3D operator+(const CPoint3D& point) {
			return CPoint3D(m_x + point.get_x(),
					m_y + point.get_y(), 
					m_z + point.get_z());
		}
		CPoint3D operator-(const CPoint3D& point) {
			return CPoint3D(m_x - point.get_x(),
					m_y - point.get_y(), 
					m_z -  point.get_z());
		}
		void operator/=(double div) {
			m_x /= div;
			m_y /= div;
			m_z /= div;
		}
		void operator+=(double a) {
			m_x += a;
			m_y += a;
			m_z += a;
		}
		void operator-=(double a) {
			m_x -= a;
			m_y -= a;
			m_z -= a;
		}
};


class CVector3D {
	private:
		CPoint3D m_begin;
		CPoint3D m_end;
		CPoint3D m_coordinates;
	public:
		CVector3D() : m_begin(CPoint3D()), m_end(CPoint3D()), m_coordinates(CPoint3D()) {}
		CVector3D(const CPoint3D& b, const CPoint3D& e) : m_begin(b), m_end(e), m_coordinates(m_end - m_begin) {}
		CVector3D(const CVector3D& v) : m_begin(v.get_begin()), m_end(v.get_end()), m_coordinates(v.get_coordinates()) {}
		
		CPoint3D get_begin() const { return m_begin; }
		CPoint3D get_end() const { return m_end; }
		CPoint3D get_coordinates() const { return m_coordinates; }
		void set_begin(const CPoint3D& begin) { 
			m_begin = begin; 
			m_coordinates = m_end - m_begin;
		}
		void set_end(const CPoint3D& end) { 
			m_end = end; 
			m_coordinates = m_end - m_begin;
		}
		inline void normalize() {
			double length = m_begin.get_distance(m_end); 
			m_coordinates /= length;
			m_end = m_begin + m_coordinates;
		}
		inline double length() const {
			return std::sqrt(m_coordinates.get_x() * m_coordinates.get_x() 
				+ m_coordinates.get_y() * m_coordinates.get_y() 
				+ m_coordinates.get_z() * m_coordinates.get_z());
		}
		inline bool intersects_with_plane(EPlane plane, const CPoint3D& plane_coord, CPoint3D& intersection) const {
			double coeff;

			switch(plane) {
				case EPlane::XY:
					if(((plane_coord.get_z() < m_begin.get_z()) && (m_coordinates.get_z() > 0))
							|| ((plane_coord.get_z() > m_begin.get_z()) && (m_coordinates.get_z() < 0)))
						return false;
					coeff = (plane_coord.get_z() - m_begin.get_z()) / m_coordinates.get_z(); 
					intersection = CPoint3D(m_begin.get_x() + m_coordinates.get_x() * coeff,
							m_begin.get_y() + m_coordinates.get_y() * coeff,
							plane_coord.get_z());
					break;
				case EPlane::YZ:
					if(((plane_coord.get_x() < m_begin.get_x()) && (m_coordinates.get_x() > 0))
							|| ((plane_coord.get_x() > m_begin.get_x()) && (m_coordinates.get_x() < 0)))
						return false;
					coeff = (plane_coord.get_x() - m_begin.get_x()) / m_coordinates.get_x(); 
					intersection = CPoint3D(plane_coord.get_x(),
							m_begin.get_y() + m_coordinates.get_y() * coeff,
							m_begin.get_z() + m_coordinates.get_z() * coeff);
					break;
				case EPlane::XZ:
					if(((plane_coord.get_y() < m_begin.get_y()) && (m_coordinates.get_y() > 0))
							|| ((plane_coord.get_y() > m_begin.get_y()) && (m_coordinates.get_y() < 0)))
						return false;
					coeff = (plane_coord.get_y() - m_begin.get_y()) / m_coordinates.get_y(); 
					intersection = CPoint3D(m_begin.get_x() + m_coordinates.get_x() * coeff,
							plane_coord.get_y(),
							m_begin.get_z() + m_coordinates.get_z() * coeff);
					break;
				case EPlane::NONE:
					std::cerr << "[EE]: unable to determine intersection: no plane" << std::endl;
					exit(1);
					break;
			}
			return true;
		}
		void operator=(const CVector3D& v) {
			m_begin = v.get_begin();
			m_end = v.get_end();
			m_coordinates = v.get_coordinates();
		}
};

class CMatrix3D {
	private:
		std::vector<std::vector<double>> m_matrix;
	public:
		CMatrix3D() : m_matrix{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}} {}
		CMatrix3D(double ax, double bx, double cx, double ay, double by, double cy, double az, double bz, double cz) 
			: m_matrix{ {ax, bx, cx}, {ay, by, cy}, {az, bz, cz} } {}
		void transpose() {
			for(size_t i = 0; i < 3; ++i)
				for(size_t j = 0; j < 3; ++j)
					m_matrix[j][i] = m_matrix[i][j];
		}
		double det() const {
			return m_matrix[0][0] * (m_matrix[1][1] * m_matrix[2][2] - m_matrix[1][2] * m_matrix[2][1]) 
				- m_matrix[0][1] * (m_matrix[1][0] * m_matrix[2][2] - m_matrix[1][2] * m_matrix[2][0]) 
				+ m_matrix[0][2] * (m_matrix[1][0] * m_matrix[2][1] - m_matrix[1][1] * m_matrix[2][0]);
		}
		void inverse() {
			double d = det();
			transpose();
			for(size_t i = 0; i < 3; ++i)
				for(size_t j = 0; j < 3; ++j)
					m_matrix[i][j] /= d;
		}
		double get(size_t i, size_t j) const {
			return m_matrix[i][j];
		}
		void operator /=(double c) {
			for(size_t i = 0; i < 3; ++i)
				for(size_t j = 0; j < 3; ++j)
					m_matrix[i][j] /= c;
		}
		CVector3D operator*(const CVector3D& vector) {
			CVector3D res;
			res.set_begin(vector.get_begin());
			res.set_end(CPoint3D(m_matrix[0][0] * vector.get_coordinates().get_x()
					+ m_matrix[0][1] * vector.get_coordinates().get_y() 
					+ m_matrix[0][2] * vector.get_coordinates().get_z(),
					m_matrix[1][0] * vector.get_coordinates().get_x()
					+ m_matrix[1][1] * vector.get_coordinates().get_y() 
					+ m_matrix[1][2] * vector.get_coordinates().get_z(),
					m_matrix[2][0] * vector.get_coordinates().get_x()
					+ m_matrix[2][1] * vector.get_coordinates().get_y() 
					+ m_matrix[2][2] * vector.get_coordinates().get_z())
					+ vector.get_begin());
			return res;
		}
};

class CColor {
	private:
		unsigned short m_red;
		unsigned short m_green;
		unsigned short m_blue;
	public:
		CColor() : m_red(0), m_green(0), m_blue(0) {}
		CColor(unsigned short red, unsigned short green, unsigned short blue) : m_red(red), m_green(green), m_blue(blue) {}
		CColor(const CColor& c) : m_red(c.get_red()), m_green(c.get_green()), m_blue(c.get_blue()) {}

		unsigned short get_red() const { return m_red; }
		unsigned short get_green() const { return m_green; }
		unsigned short get_blue() const { return m_blue; }

		void operator=(const CColor& color) {
			m_red = color.get_red();
			m_green = color.get_green();
			m_blue = color.get_blue();
		}
};
#endif //GEOMETRY_HPP
