#version 450 core

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct Particle {
	vec3 Position;
	vec2 Velocity;
	vec2 Acceleration;
	vec4 Color;
	float Lifetime;

	int TextureIndex;
};

layout(std430, binding = 0) buffer SSBO_0 {
  Particle particles[];
};

layout(std430, binding = 1) buffer SSBO_1 {
  coherent int count;
  int indices[];
}freelist;

layout(location = 0) uniform float u_DeltaTime;

void Update(inout Particle particle, int index) {
	if (particle.Lifetime > 0.0) {
		particle.Velocity += particle.Acceleration.xy * u_DeltaTime;
		particle.Position.xy += particle.Velocity.xy * u_DeltaTime;
		particle.Lifetime -= u_DeltaTime;

		if (particle.Lifetime <= 0.0) {
			particle.Color.a = 0.0;
			freelist.indices[atomicAdd(freelist.count, 1)] = index;
		}
	}
}

void main() {
	uint index = gl_GlobalInvocationID.x;

	if (index >= particles.length())
		return;

	Update(particles[index], int(index));
}