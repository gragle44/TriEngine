#version 450 core
			
layout(location = 0) out vec4 color;
in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;

uniform sampler2D u_Samplers[32];

void main()
{
	color = texture(u_Samplers[int(v_TexIndex)], v_TexCoord) * v_Color;
	//color = vec4(v_TexCoord, 1.0, 1.0);
}