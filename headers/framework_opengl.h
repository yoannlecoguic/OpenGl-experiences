#ifndef FRAMEWORK_OPENGL_H
#define FRAMEWORK_OPENGL_H

//Display compilation errors from the OpenGL shader compiler
void print_log(GLuint object);
//Compile the shader from file 'filename', with error handling
GLuint create_shader(const char* filename, GLenum type);

#endif
