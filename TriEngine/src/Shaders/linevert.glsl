#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjection;

out vec4 fragColor;

void main() {
    fragColor = a_Color;
    gl_Position = u_ViewProjection * vec4(a_Pos, 1.0);
}
