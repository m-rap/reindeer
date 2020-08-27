#ifndef SHADER_HPP
#define SHADER_HPP

//#if defined __arm__ || defined __aarch64__
#ifdef USE_GLES
#include <GLES2/gl2.h>
#elif defined __APPLE__
    #include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif
