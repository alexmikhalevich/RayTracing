#include <iostream>

class IException {
	public:
		virtual void what() const = 0;
};

class ExEOF : public IException {
	public:
		virtual void what() const { std::cerr << "[EE]: Unexpected end of file" << std::endl; }
};

class ExInvalidLighter : public IException {
	public:
		virtual void what() const { std::cerr << "[EE]: Invalid lighter format" << std::endl; }
};

class ExInvalidSphere : public IException {
	public:
		virtual void what() const { std::cerr << "[EE]: Invalid sphere format" << std::endl; }
};

class ExInvalidTriangle : public IException {
	public:
		virtual void what() const { std::cerr << "[EE]: Invalid triangle format" << std::endl; }
};

class ExInvalidQuadrangle : public IException {
	public:
		virtual void what() const { std::cerr << "[EE]: Invalid quadrangle format" << std::endl; }
};
