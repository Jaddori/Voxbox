#include "Shader.h"

Shader::Shader()
	: program( 0 ), valid( false )
{
}

Shader::~Shader()
{
}

bool Shader::load( const char* vertex, const char* geometry, const char* fragment )
{
	LOG_INFO( "Loading:" );

	valid = true;

	unload();

	GLuint vertexShader = loadShader( vertex, GL_VERTEX_SHADER );
	GLuint geometryShader = loadShader( geometry, GL_GEOMETRY_SHADER );
	GLuint fragmentShader = loadShader( fragment, GL_FRAGMENT_SHADER );

	if( vertex && vertexShader == 0 )
		valid = false;
	else if( geometry && geometryShader == 0 )
		valid = false;
	else if( fragment && fragmentShader == 0 )
		valid = false;
	else
	{
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
			char logbuffer[1024] = {};
			int len = 1024;
			glGetProgramInfoLog( program, 1024, &len, logbuffer );
			LOG_WARNING( "%s", logbuffer );

			valid = false;
		}
	}

	return valid;
}

void Shader::unload()
{
	if( program > 0 )
		glDeleteProgram( program );
	program = 0;
}

GLuint Shader::loadShader( const char* path, GLenum type )
{
	GLuint result = 0;

	if( path )
	{
		LOG_INFO( "\t%s", path );

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
				char logbuffer[1024] = {};
				int len = 1024;
				glGetShaderInfoLog( result, 1024, &len, logbuffer );

				LOG_WARNING( "%s", logbuffer );
			}
		}
		else
			LOG_WARNING( "File not found: %s", path );
	}

	return result;
}

GLuint Shader::getProgram() const
{
	return program;
}

bool Shader::getValid() const
{
	return valid;
}