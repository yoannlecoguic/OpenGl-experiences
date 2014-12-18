#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Opengl
#include <GL/glew.h>
#include <GL/freeglut.h>

//C scripts
/*extern "C" {
	#include "headers/read_file.h"
}*/

//Cpp library
#include "headers/framework_opengl.h"
#include "headers/objects.h"

GLuint program;
GLuint vbo_triangle, vbo_triangle_colors;
GLuint vbo_cube, vbo_cube_colors;
GLint attribute_coord3d, attribute_vcolor, uniform_fade, uniform_matrix, uniform_mvp;
GLuint ibo_cube_elements;

int screen_width=800, screen_height=600;

/*
GLfloat triangle_vertices[] = {
	0.0,  0.8, 0.8,
	-0.8, -0.8, 0.8,
	0.8, -0.8,  0.8,
};

GLfloat triangle_colors[] = {
	1.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 0.0, 0.0,
};
*/

GLfloat cube_vertices[] = {
	// front
	-0.7, -0.7,  0.7,
	0.7, -0.7,  0.7,
	0.7,  0.7,  0.7,
	-0.7,  0.7,  0.7,
	// back
	-0.7, -0.7, -0.7,
	0.7, -0.7, -0.7,
	0.7,  0.7, -0.7,
	-0.7,  0.7, -0.7,
};

GLfloat cube_colors[] = {
	// front colors
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
	// back colors
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
};

//specify all faces in a similar way, here counter-clockwise important
GLushort cube_elements[] = {
	// front
	0, 1, 2,
	2, 3, 0,
	// top
	3, 2, 6,
	6, 7, 3,
	// back
	7, 6, 5,
	5, 4, 7,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// left
	4, 0, 3,
	3, 7, 4,
	// right
	1, 5, 6,
	6, 2, 1,
};

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

	//Bind attribute uniform fade
	const char* fade_name;
	fade_name = "fade";
	uniform_fade = glGetUniformLocation(program, fade_name);
	if (uniform_fade == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", fade_name);
		return 0;
	}

	//Bind attribute uniform matrix
	const char* matrix_name;
	matrix_name = "matrix";
	uniform_matrix = glGetUniformLocation(program, matrix_name);
	if (uniform_matrix == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", matrix_name);
		return 0;
	}

	//Bind attribute uniform matrix
	const char* mvp_name;
	mvp_name = "mvp";
	uniform_mvp = glGetUniformLocation(program, mvp_name);
	if (uniform_mvp == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", mvp_name);
		return 0;
	}

	//Bind attribute coord3d
	glGenBuffers(1, &vbo_cube);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	const char* attribute_name = "coord3d";
	attribute_coord3d = glGetAttribLocation(program, attribute_name);
	if (attribute_coord3d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	//Bind attribute vcolor
	glGenBuffers(1, &vbo_cube_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	attribute_name = "vcolor";
	attribute_vcolor = glGetAttribLocation(program, attribute_name);
	if (attribute_vcolor == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	//Bind element
	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return 1;
}

void onDisplay()
{
	/* Clear the background as white */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Enable alpha
	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(program);

	glEnableVertexAttribArray(attribute_coord3d);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
	glVertexAttribPointer(
		attribute_coord3d, // attribute
		3,                 // number of elements per vertex, here (x,y)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of the first element
	);

	glEnableVertexAttribArray(attribute_vcolor);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
	glVertexAttribPointer(
		attribute_vcolor, // attribute
		3,                 // number of elements per vertex, here (x,y)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of the first element
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	/* Push each element in buffer_vertices to the vertex shader */
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	//Cleaning
	glDisableVertexAttribArray(attribute_coord3d);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Display the result */
	glutSwapBuffers();
}

void free_resources()
{
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_triangle);
}

void onReshape(int width, int height) {
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, screen_width, screen_height);
}

void idle()
{
	float cur_fade = 1;
	float move = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * -1;  // 45° per second
	float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 1;  // 45° per second
	glm::vec3 axis_z(0, 1, 1);
	//First rotate then translate, because the rotation is done on the center on the vertex origin
	//Before the translation so
	glm::mat4 m_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, move))
	* glm::rotate(glm::mat4(1.0f), angle, axis_z);

	//model
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
	//view
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
	//projection
	glm::mat4 projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 100.0f);
	//mvp
	glm::mat4 mvp = projection * view * model;

	glUseProgram(program);
	glUniform1f(uniform_fade, cur_fade);
	glUniformMatrix4fv(uniform_matrix, 1, GL_FALSE, glm::value_ptr(m_transform));
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	/* Glut-related initialising functions */
	glutInit(&argc, argv);
	glutInitContextVersion(2,0);
	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
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
		glutIdleFunc(idle);
		glutReshapeFunc(onReshape);
		glutMainLoop();
	}

	/* If the program exits in the usual way,
	free resources and exit with a success */
	free_resources();
	return EXIT_SUCCESS;
}
