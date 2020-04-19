#include "opengl.h"

void GLClearError() {
	// while body is irrelevant... we just want to call until we get GL_NO_ERROR
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "GL Error: " << error <<
			" : " << function << " : " << file << " : " << line << std::endl;
		return false;
	}
	return true;
}