#version 450 core
			
layout(location = 0) out vec4 color;
in vec2 v_TexCoord;

uniform vec3 u_Color;
//uniform sampler2D u_Texture;

void main()
{
	vec4 colorMod = vec4(u_Color, 1.0);
	color = vec4(v_TexCoord, 0.0, 1.0);
}