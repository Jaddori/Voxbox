#include "Shader.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}

bool Shader::load( const char* vertex, const char* geometry, const char* fragment )
{
	bool result = true;

	unload();

	GLuint vertexShader = loadShader( vertex, GL_VERTEX_SHADER );
	GLuint geometryShader = loadShader( geometry, GL_GEOMETRY_SHADER );
	GLuint fragmentShader = loadShader( fragment, GL_FRAGMENT_SHADER );

	program = glCreateProgram();
	if( vertexShader )
		glAttachShader( program, vertexShader );
	if( geometryShader )
		glAttachShader( program, geometryShader );
	if( fragmentShader )
		glAttachShader( program, fragmentShader );
	glLinkProgram( program );

	GLint linkSuccess;
	glGetProgramiv( program, GL_LINK_STATUS, &linkSuccess );
	if( linkSuccess != GL_TRUE )
	{
		char buffer[1024] = {};
		int len = 1024;
		glGetProgramInfoLog( program, 1024, &len, buffer );
		printf( "%s\n", buffer );
	}

	return result;
}

void Shader::unload()
{
	if( program > 0 )
		glDeleteProgram( program );
	program = 0;
}

void Shader::bind()
{
	glUseProgram( program );
}

GLuint Shader::getLocation( const char* name )
{
	return glGetUniformLocation( program, name );
}

void Shader::setInt( GLuint location, int value )
{
	glUniform1i( location, value );
}

void Shader::setFloat( GLuint location, float value )
{
	glUniform1f( location, value );
}

void Shader::setVec2( GLuint location, const glm::vec2& value )
{
	glUniform2f( location, value.x, value.y );
}

void Shader::setVec3( GLuint location, const glm::vec3& value )
{
	glUniform3f( location, value.x, value.y, value.z );
}

void Shader::setVec4( GLuint location, const glm::vec4& value )
{
	glUniform4f( location, value.x, value.y, value.z, value.w );
}

void Shader::setMat4( GLuint location, const glm::mat4& value )
{
	glUniformMatrix4fv( location, 1, GL_FALSE, glm::value_ptr( value ) );
}

void Shader::setInt( GLuint location, const int* values, int n )
{
	glUniform1iv( location, n, values );
}

void Shader::setFloat( GLuint location, const float* values, int n )
{
	glUniform1fv( location, n, values );
}

void Shader::setMat4( GLuint location, const glm::mat4* values, int n )
{
	glUniformMatrix4fv( location, n, GL_FALSE, glm::value_ptr( *values ) );
}

GLuint Shader::loadShader( const char* path, GLenum type )
{
	GLuint result = 0;

	if( path )
	{
		FILE* file = fopen( path, "rb" );
		if( file )
		{
			// retrieve the length of the file
			fseek( file, 0, SEEK_END );
			int len = ftell( file );
			fseek( file, 0, SEEK_SET );

			// read the file
			char* buffer = new char[len+1];
			fread( buffer, 1, len, file );
			buffer[len] = 0;

			fclose( file );

			// compile the shader source
			result = glCreateShader( type );
			glShaderSource( result, 1, &buffer, 0 );
			glCompileShader( result );

			GLint compilationSuccess;
			glGetShaderiv( result, GL_COMPILE_STATUS, &compilationSuccess );
			if( compilationSuccess != GL_TRUE )
			{
				char buffer[1024] = {};
				int len = 1024;
				glGetShaderInfoLog( result, 1024, &len, buffer );
				printf( "%s\n", buffer );
			}
		}
	}

	return result;
}