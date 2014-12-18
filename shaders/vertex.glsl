#version 120

uniform   mat4  mvp;
uniform   mat4  matrix;
attribute vec3  coord3d;
attribute vec3  vcolor;
varying   vec3  fcolor;
void main(void) {
	gl_Position = mvp * (matrix * vec4(coord3d, 1.0));
	fcolor      = vcolor;
}
