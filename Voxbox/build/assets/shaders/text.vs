#version 330

layout(location=0) in vec2 vertPosition;
layout(location=1) in vec4 vertUV;
layout(location=2) in vec2 vertSize;

out vec4 geomUV;
out vec2 geomSize;

void main()
{
	geomUV = vertUV;
	geomSize = vertSize;
	gl_Position = vec4( vertPosition, 0.0, 1.0 );
}