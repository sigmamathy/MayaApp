#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texture_coordinate;

out vec2 v_texture_coordinate;

uniform mat4 u_projection, u_view, u_model;

void main() {
	v_texture_coordinate = in_texture_coordinate;
	gl_Position = u_projection * u_view * u_model * vec4(in_position, 1.0f);
}