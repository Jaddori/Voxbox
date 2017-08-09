#version 330

layout(location=0) in vec2 vertPosition;

out vec2 fragUV;

uniform mat4 projectionMatrix;
uniform mat4 worldMatrix;
uniform vec2 UVOffset;
uniform vec2 UVLength;

void main()
{
	fragUV = UVOffset + vertPosition * UVLength;
	gl_Position = projectionMatrix * worldMatrix * vec4( vertPosition, 0.0, 1.0 );
}