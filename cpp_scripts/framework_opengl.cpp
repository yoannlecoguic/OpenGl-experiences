#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

#include "../headers/framework_opengl.h"

extern "C" {
	#include "../headers/read_file.h"
}

//Display compilation errors from the OpenGL shader compiler
void print_log(GLuint object)
{
	GLint log_length = 0;
	if (glIsShader(object))
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else if (glIsProgram(object))
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else
	{
		fprintf(stderr, "printlog: Not a shader or a program\n");
		return;
	}

	char* log = (char*)malloc(log_length);

	if (glIsShader(object))
		glGetShaderInfoLog(object, log_length, NULL, log);
	else if (glIsProgram(object))
	{
		glGetProgramInfoLog(object, log_length, NULL, log);

		fprintf(stderr, "%s", log);
		free(log);
	}
}

//Compile the shader from file 'filename', with error handling
GLuint create_shader(const char* filename, GLenum type)
{
	const GLchar* source = file_read(filename);
	if (source == NULL)
	{
		fprintf(stderr, "Error opening %s: ", filename); perror("");
		return 0;
	}
	GLuint res = glCreateShader(type);
	const GLchar* sources[2] =
	{
		#ifdef GL_ES_VERSION_2_0
		"#version 100\n"
		"#define GLES2\n",
		#else
		"#version 120\n",
		#endif
		source
	};
	glShaderSource(res, 2, sources, NULL);
	free((void*)source);

	glCompileShader(res);
	GLint compile_ok = GL_FALSE;
	glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
	if (compile_ok == GL_FALSE) {
		fprintf(stderr, "%s:", filename);
		print_log(res);
		glDeleteShader(res);
		return 0;
	}

	return res;
}
