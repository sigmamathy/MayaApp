#version 330 core

out vec4 FragColor;

in vec2 v_texture_coordinate;

uniform sampler2D u_texture_diffuse[4];
uniform sampler2D u_texture_specular[4];

void main() {
	FragColor = texture(u_texture_diffuse[0], v_texture_coordinate);
}