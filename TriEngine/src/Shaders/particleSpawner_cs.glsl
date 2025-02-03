#version 450 core

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct ParticleEmmiter {
	vec4 MinColor, MaxColor;
	vec3 MinOffset, MaxOffset;
	vec2 MinVelocity, MaxVelocity;
	vec2 MinAcceleration, MaxAcceleration;
	float MinLifetime, MaxLifetime;

	vec3 Position;

	float SpawnInterval, SpawnTimer;

	int TextureIndex;
};

struct Particle {
	vec3 Position;
	vec2 Velocity;
	vec2 Acceleration;
	vec4 Color;
	float Lifetime;

	int TextureIndex;
};

layout(std430, binding = 0) writeonly restrict buffer SSBO_0 {
  Particle particles[];
};

layout(std430, binding = 1) coherent restrict buffer SSBO_1 {
  int count;
  int indices[];
}freelist;

layout(location = 0) uniform int u_ParticlesToSpawn;
layout(location = 1) uniform int u_Seed;
layout(location = 2) uniform ParticleEmmiter u_Emitter;

uint hash(uint seed) {
    seed = (seed ^ 61u) ^ (seed >> 16u);
    seed *= 9u;
    seed = seed ^ (seed >> 4u);
    seed *= 0x27d4eb2du;
    seed = seed ^ (seed >> 15u);
    return seed;
}

float random(uint seed, float minVal, float maxVal) {
    float normalized = float(hash(seed)) / float(0xFFFFFFFFu);
    return mix(minVal, maxVal, normalized);
}

vec2 random2(uint seed, vec2 minVal, vec2 maxVal) {
    return vec2(random(seed, minVal.x, maxVal.x), 
                random(seed + 1u, minVal.y, maxVal.y));
}

vec3 random3(uint seed, vec3 minVal, vec3 maxVal) {
    return vec3(random(seed, minVal.x, maxVal.x), 
                random(seed + 1u, minVal.y, maxVal.y), 
                random(seed + 2u, minVal.z, maxVal.z));
}

vec4 random4(uint seed, vec4 minVal, vec4 maxVal) {
    return vec4(random(seed, minVal.x, maxVal.x), 
                random(seed + 1u, minVal.y, maxVal.y), 
                random(seed + 2u, minVal.z, maxVal.z), 
                random(seed + 3u, minVal.w, maxVal.w));
}


Particle CreateParticleAlt(uint seed) {
	Particle particle;
	particle.Position = u_Emitter.Position + random3(seed, u_Emitter.MinOffset, u_Emitter.MaxOffset);

	particle.Velocity = random2(seed, u_Emitter.MinVelocity, u_Emitter.MaxVelocity);
	particle.Acceleration = random2(seed, u_Emitter.MinAcceleration, u_Emitter.MaxAcceleration);
	particle.Color = random4(seed, u_Emitter.MinColor, u_Emitter.MaxColor);
	particle.Lifetime = random(seed, u_Emitter.MinLifetime, u_Emitter.MaxLifetime);
	particle.TextureIndex = u_Emitter.TextureIndex;

	return particle;
}


void main() {
	uint index = gl_GlobalInvocationID.x;

	if (index >= u_ParticlesToSpawn)
		return;

	int freeListIndex = atomicAdd(freelist.count, -1) - 1;
	if (freeListIndex < 0) {
		atomicAdd(freelist.count, 1);
		return;
	}

	int particleIndex = freelist.indices[freeListIndex];

	particles[particleIndex] = CreateParticleAlt(u_Seed+index);
	//CreateParticle(particles[particleIndex]);
}