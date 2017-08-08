#version 330

const int BLOCK_TILEMAP_SIZE = 10;
const float BLOCK_INVERSE_SIZE = 1.0 / BLOCK_TILEMAP_SIZE;

in vec2 fragUV;
flat in int fragBlockType;

out vec4 finalColor;

uniform sampler2D diffuseMap;

void main()
{
	int yoffset = fragBlockType / BLOCK_TILEMAP_SIZE;
	int xoffset = ( fragBlockType - yoffset*BLOCK_TILEMAP_SIZE );
	vec2 uv = fragUV + vec2( xoffset, yoffset ) * BLOCK_INVERSE_SIZE;
	
	finalColor = texture( diffuseMap, uv );
}