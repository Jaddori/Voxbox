#version 330

in vec2 fragUV;
in float fragOpacity;

out vec4 finalColor;

uniform sampler2D diffuseMap;

void main()
{
	finalColor = texture( diffuseMap, fragUV ) * vec4( 1.0, 1.0, 1.0, fragOpacity );
}