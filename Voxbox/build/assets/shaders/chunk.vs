#version 330

const int CHUNK_SIZE = 16;

layout(location=0) in vec3 vertPosition;
layout(location=1) in vec2 vertUV;

out vec2 fragUV;
flat out int fragBlockType;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 offset;

layout(std140) uniform blockPositions
{
	vec4 positions[CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE];
};

void main()
{
	fragUV = vertUV;
	fragBlockType = int( positions[gl_InstanceID].w );
	gl_Position = projectionMatrix * viewMatrix * vec4( vertPosition + positions[gl_InstanceID].xyz + offset*CHUNK_SIZE, 1.0 );
}