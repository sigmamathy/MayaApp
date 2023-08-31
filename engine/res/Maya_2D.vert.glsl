#version 330 core

layout (location = 0) in vec2 in_position;

uniform mat4 u_projection, u_view, u_model;

void main() {
	gl_Position = u_projection * u_view * u_model * vec4(in_position, 0.0f, 1.0f);
}