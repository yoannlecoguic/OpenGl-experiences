#include <stdio.h>
#include <stdlib.h>

//Opengl
#include <GL/glew.h>
#include <GL/freeglut.h>

//C scripts
/*extern "C" {
	#include "headers/read_file.h"
}*/

//Cpp library
#include "headers/framework_opengl.h"

GLuint program;
GLuint vbo_triangle, vbo_triangle_colors;
GLint attribute_coord2d, attribute_color4d;

int init_resources(void)
{
	GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;
	GLuint vertex_shader, fragment_shader;

	if ((vertex_shader = create_shader("shaders/vertex.glsl", GL_VERTEX_SHADER))   == 0) return 0;
	if ((fragment_shader = create_shader("shaders/fragment.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		fprintf(stderr, "glLinkProgram:");
		print_log(program);
		return 0;
	}

	GLfloat triangle_vertices[] = {
		0.0,  0.8,
		-0.8, -0.8,
		0.8, -0.8,
	};

	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	const char* attribute_name = "coord2d";
	attribute_coord2d = glGetAttribLocation(program, attribute_name);
	if (attribute_coord2d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	GLfloat triangle_colors[] = {
		0.0, 0.8, 0.0, 1.0,
		0.0, 0.8, 0.0, 1.0,
		0.8, 0.8, 0.0, 1.0,
	};

	glGenBuffers(1, &vbo_triangle_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_colors), triangle_colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	attribute_name = "color4d";
	attribute_color4d = glGetAttribLocation(program, attribute_name);
	if (attribute_color4d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	return 1;
}

void onDisplay()
{
	/* Clear the background as white */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(program);

	glEnableVertexAttribArray(attribute_coord2d);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glVertexAttribPointer(
		attribute_coord2d, // attribute
		2,                 // number of elements per vertex, here (x,y)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of the first element
	);

	/* Push each element in buffer_vertices to the vertex shader */
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glEnableVertexAttribArray(attribute_color4d);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle_colors);
	glVertexAttribPointer(
		attribute_color4d, // attribute
		4,                 // number of elements per vertex, here (x,y)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of the first element
	);

	/* Push each element in buffer_vertices to the vertex shader */
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//Cleaning
	glDisableVertexAttribArray(attribute_coord2d);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Display the result */
	glutSwapBuffers();
}

void free_resources()
{
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_triangle);
}

int main(int argc, char* argv[])
{
	/* Glut-related initialising functions */
	glutInit(&argc, argv);
	glutInitContextVersion(2,0);
	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("My First Triangle");

	/* Extension wrangler initialising */
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return EXIT_FAILURE;
	}

	/* When all init functions run without errors,
	the program can initialise the resources */
	if (init_resources())
	{
		/* We can display it if everything goes OK */
		glutDisplayFunc(onDisplay);
		glutMainLoop();
	}

	/* If the program exits in the usual way,
	free resources and exit with a success */
	free_resources();
	return EXIT_SUCCESS;
}
