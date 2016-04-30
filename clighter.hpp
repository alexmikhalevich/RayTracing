#ifndef CLIGHTER_HPP
#define CLIGHTER_HPP
#include "geometry.hpp"

class CLighter {
	private:
		CPoint3D m_position;
		double m_intensity;
	public:
		CLighter(const CPoint3D& p, double i = 1) : m_position(p), m_intensity(i) {
			if(m_intensity < 0) m_intensity = 0;
			if(m_intensity > 100) m_intensity = 100;
		}

		double get_intensity(const CPoint3D& p) const { 
			return m_intensity; 
		}
		void set_intensity(double intensity) { m_intensity = intensity; }

		CPoint3D get_position() const { return m_position; }
		void set_position(const CPoint3D& position) { m_position = position; }
};
#endif //CLIGHTER_HPP
