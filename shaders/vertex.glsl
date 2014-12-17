#version 120

attribute vec2 coord2d;
attribute vec4 color4d;
varying   vec4 f_color4d;
void main(void) {
	gl_Position = vec4(coord2d, 0.0, 1.0);
	f_color4d   = color4d;
}
