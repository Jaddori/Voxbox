#version 330

layout(location=0) in vec3 vertPosition;
layout(location=1) in vec3 vertBlockPosition;
layout(location=2) in vec4 vertColor;

out vec4 fragColor;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * vec4( vertPosition + vertBlockPosition, 1.0 );
	fragColor = vertColor;
}