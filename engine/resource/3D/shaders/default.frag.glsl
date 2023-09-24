#version 330 core

out vec4 FragColor;

in vec2 v_texture_coordinate;

uniform sampler2D u_texture_diffuse_0;
uniform sampler2D u_texture_specular_0;

void main() {
	FragColor = texture(u_texture_diffuse_0, v_texture_coordinate);
	// FragColor = vec4(1.0f);
}