#version 120

uniform float fade;
varying vec3 fcolor;
void main(void) {
	gl_FragColor = vec4(fcolor.x, fcolor.y, fcolor.z, fade);
}
