#ifndef CMATERIAL_H
#define CMATERIAL_H
#include "geometry.hpp"

class CMaterial {
	private:
		CColor m_color;
		double m_alpha;
		double m_reflect;
		double m_refract;
	public:
		CMaterial() : m_color(CColor()), m_alpha(1.0), m_reflect(0), m_refract(0) {}
		CMaterial(const CColor& col, double a, double refl, double refr) {
			m_color = col;
			m_alpha = std::min(std::max(0.0, a), 1.0);
			m_reflect = std::min(std::max(refl, 0.0), 1.0);
			m_refract = std::max(0.0, refr);
		}
		
		CColor get_color() const {
			return m_color;
		}

		double get_alpha() const {
			return m_alpha;
		}

		double get_reflect() const {
			return m_reflect;
		}

		double get_refract() const {
			return m_refract;
		}

		void set_color(const CColor& col) {
			m_color = col;
		}

		void set_alpha(double a) {
			m_alpha = a;
		}

		void set_reflection(double r) {
			m_reflect = r;
		}

		void set_refraction(double r) {
			m_refract = r;
		}
};
#endif //CMATERIAL_H
