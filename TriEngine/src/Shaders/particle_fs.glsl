#version 450 core

layout(location = 0) in vec2 v_TexCoord;
layout(location = 1) in flat int v_TexIndex;
layout(location = 2) in vec4 v_Color;

layout(location = 2) uniform sampler2D u_Samplers[32];

layout(location = 0) out vec4 fragColor;

void main() {
	vec4 texColor;
	switch(int(v_TexIndex))
	{
		case  0: texColor = texture(u_Samplers[ 0], v_TexCoord); break;
		case  1: texColor = texture(u_Samplers[ 1], v_TexCoord); break;
		case  2: texColor = texture(u_Samplers[ 2], v_TexCoord); break;
		case  3: texColor = texture(u_Samplers[ 3], v_TexCoord); break;
		case  4: texColor = texture(u_Samplers[ 4], v_TexCoord); break;
		case  5: texColor = texture(u_Samplers[ 5], v_TexCoord); break;
		case  6: texColor = texture(u_Samplers[ 6], v_TexCoord); break;
		case  7: texColor = texture(u_Samplers[ 7], v_TexCoord); break;
		case  8: texColor = texture(u_Samplers[ 8], v_TexCoord); break;
		case  9: texColor = texture(u_Samplers[ 9], v_TexCoord); break;
		case 10: texColor = texture(u_Samplers[10], v_TexCoord); break;
		case 11: texColor = texture(u_Samplers[11], v_TexCoord); break;
		case 12: texColor = texture(u_Samplers[12], v_TexCoord); break;
		case 13: texColor = texture(u_Samplers[13], v_TexCoord); break;
		case 14: texColor = texture(u_Samplers[14], v_TexCoord); break;
		case 15: texColor = texture(u_Samplers[15], v_TexCoord); break;
		case 16: texColor = texture(u_Samplers[16], v_TexCoord); break;
		case 17: texColor = texture(u_Samplers[17], v_TexCoord); break;
		case 18: texColor = texture(u_Samplers[18], v_TexCoord); break;
		case 19: texColor = texture(u_Samplers[19], v_TexCoord); break;
		case 20: texColor = texture(u_Samplers[20], v_TexCoord); break;
		case 21: texColor = texture(u_Samplers[21], v_TexCoord); break;
		case 22: texColor = texture(u_Samplers[22], v_TexCoord); break;
		case 23: texColor = texture(u_Samplers[23], v_TexCoord); break;
		case 24: texColor = texture(u_Samplers[24], v_TexCoord); break;
		case 25: texColor = texture(u_Samplers[25], v_TexCoord); break;
		case 26: texColor = texture(u_Samplers[26], v_TexCoord); break;
		case 27: texColor = texture(u_Samplers[27], v_TexCoord); break;
		case 28: texColor = texture(u_Samplers[28], v_TexCoord); break;
		case 29: texColor = texture(u_Samplers[29], v_TexCoord); break;
		case 30: texColor = texture(u_Samplers[30], v_TexCoord); break;
		case 31: texColor = texture(u_Samplers[31], v_TexCoord); break;
	}
    fragColor = texColor * v_Color;

	if (fragColor.a == 0.0)
		discard;

}