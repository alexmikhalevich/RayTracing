#ifndef IPARSER_H
#define IPARSER_H
#include "iobject3d.h"
#include "ccamera.hpp"
#include "clighter.hpp"

class IParser {
	public:
		virtual void parse(const std::string& filename) = 0;
		virtual std::vector<IObject3D*>& get_objects() = 0;
		virtual CCamera get_camera() const = 0; 
		virtual std::vector<CLighter>& get_lighters() = 0;
		virtual ~IParser() {}
};
#endif //IPARSER_H
