#include <iostream>
#include "csphere.h"

int main() {
	CSphere sphere;
	CVector3D v(CPoint3D(), CPoint3D(5.0, 5.0, 5.0));
	CPoint3D p;
	if(sphere.intersect(v, p)) std::cout << "yes: (" << p.get_x() << ", " << p.get_y() << ", " << p.get_z() << ")" << std::endl;
	else std::cout << "no";
	return 0;
}
