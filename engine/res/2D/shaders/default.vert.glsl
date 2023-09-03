#version 330 core

layout (location = 0) in vec2 in_position;
layout (location = 1) in vec2 in_texture_coordinate;

out vec2 v_texture_coordinate;

uniform mat4 u_projection, u_view_pos, u_view_scale, u_model;

void main()
{
	v_texture_coordinate = in_texture_coordinate;
	mat4 view = u_view_scale * u_view_pos;
	gl_Position = u_projection * view * u_model * vec4(in_position, 0.0f, 1.0f);
}