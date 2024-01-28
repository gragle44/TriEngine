#version 450 core
			
layout(location = 0) out vec4 color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Tint;
uniform float u_TilingFactor;

void main()
{
	color = texture(u_Texture, v_TexCoord * u_TilingFactor) * u_Tint;
}