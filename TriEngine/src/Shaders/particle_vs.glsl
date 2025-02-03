#version 450 core

struct Particle {
	vec3 Position;
	vec2 Velocity;
	vec2 Acceleration;
	vec4 Color;
	float Lifetime;

	int TextureIndex;
};

layout(std430, binding = 0) readonly restrict buffer Particles {
    Particle particles[];
};

layout(location = 0) in vec2 a_Position;

layout(location = 0) uniform mat4 u_ViewProj;

layout(location = 0) out vec2 v_TexCoord;
layout(location = 1) out flat int v_TexIndex;
layout(location = 2) out vec4 v_Color;

void main() {
    int index = gl_InstanceID;

    Particle particle = particles[index];

	v_TexCoord = a_Position + 0.5;
	v_Color = particle.Color;
	v_TexIndex = particle.TextureIndex;

	vec3 globalPosition = vec3(a_Position.xy, 1.0) + particle.Position;

	gl_Position = u_ViewProj * vec4(globalPosition, 1.0);
}