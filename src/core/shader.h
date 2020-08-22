#ifndef SHADER_HPP
#define SHADER_HPP

//#include <GL/glew.h>

#if defined __arm__ || defined __aarch64__
#include <GLES2/gl2.h>
#else
#include <GL/gl.h>
#endif

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif
