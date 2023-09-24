#version 330 core

out vec4 FragColor;

in vec2 v_texture_coordinate;

uniform bool u_draw_texture;
uniform bool u_draw_text;
uniform bool u_draw_glow;

uniform sampler2D u_texture;
uniform sampler2D u_glow_texture;
uniform vec4 u_color;

void main()
{
	float glow = u_draw_glow ? texture(u_glow_texture, v_texture_coordinate).r : 1.0f;

	if (u_draw_text)
	{
		FragColor = texture(u_texture, v_texture_coordinate) * u_color;
		FragColor.a *= glow;
		return;
	}
	FragColor = u_draw_texture ? texture(u_texture, v_texture_coordinate) : u_color;
	FragColor.a *= glow;
}