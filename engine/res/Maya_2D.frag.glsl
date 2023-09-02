#version 330 core

out vec4 FragColor;

in vec2 v_texture_coordinate;

uniform bool u_draw_texture;
uniform sampler2D u_texture;
uniform vec4 u_color;

void main()
{
	FragColor = !u_draw_texture ? u_color : texture(u_texture, v_texture_coordinate);
}