#version 330

layout(points) in;
layout(triangle_strip, max_vertices=4) out;

in vec4 geomUV[];
in vec2 geomSize[];
in float geomOpacity[];

out vec2 fragUV;
out float fragOpacity;

uniform mat4 projectionMatrix;

void main()
{
	vec2 minUV = vec2( geomUV[0].x, geomUV[0].y );
	vec2 maxUV = vec2( geomUV[0].z, geomUV[0].w );
	vec4 pos;
	
	// top left
	pos = gl_in[0].gl_Position;
	gl_Position = projectionMatrix * pos;
	fragUV = minUV;
	fragOpacity = geomOpacity[0];
	EmitVertex();
	
	// bottom left
	pos = gl_in[0].gl_Position;
	pos.y += geomSize[0].y;
	gl_Position = projectionMatrix * pos;
	fragUV = vec2( minUV.x, maxUV.y );
	fragOpacity = geomOpacity[0];
	EmitVertex();
	
	// top right
	pos = gl_in[0].gl_Position;
	pos.x += geomSize[0].x;
	gl_Position = projectionMatrix * pos;
	fragUV = vec2( maxUV.x, minUV.y );
	fragOpacity = geomOpacity[0];
	EmitVertex();
	
	// bottom right
	pos = gl_in[0].gl_Position;
	pos.xy += geomSize[0];
	gl_Position = projectionMatrix * pos;
	fragUV = maxUV;
	fragOpacity = geomOpacity[0];
	EmitVertex();
	
	EndPrimitive();
}