#pragma once

#include "BaseIncludes.h"

class Shader
{
public:
	Shader();
	~Shader();

	bool load( const char* vertex, const char* geometry, const char* fragment );
	void unload();

	inline void bind();
	inline GLuint getLocation( const char* name );

	inline void setInt( GLuint location, int value );
	inline void setFloat( GLuint location, float value );
	inline void setVec2( GLuint location, const glm::vec2& value );
	inline void setVec3( GLuint location, const glm::vec3& value );
	inline void setVec4( GLuint location, const glm::vec4& value );
	inline void setMat4( GLuint location, const glm::mat4& value );
	
	inline void setInt( GLuint location, const int* values, int n );
	inline void setFloat( GLuint location, const float* values, int n );
	inline void setMat4( GLuint location, const glm::mat4* values, int n );

private:
	GLuint loadShader( const char* path, GLenum type );

	GLuint program;
};