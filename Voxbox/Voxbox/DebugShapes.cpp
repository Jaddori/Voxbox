#include "DebugShapes.h"

DebugShapes::DebugShapes()
{
	LOG( VERBOSITY_INFORMATION, "DebugShapes.cpp - Constructing." );
}

DebugShapes::~DebugShapes()
{
	LOG( VERBOSITY_INFORMATION, "DebugShapes.cpp - Destructing." );
}

bool DebugShapes::load()
{
	LOG( VERBOSITY_INFORMATION, "DebugShapes.cpp - Loading shaders." );

	bool result = false;

	if( !lineShader.load( "./assets/shaders/debug_line.vs",
							"./assets/shaders/debug_line.gs",
							"./assets/shaders/debug_line.fs" ) )
	{
		 LOG( VERBOSITY_ERROR, "DebugShapes.cpp - Failed to load debug line shader." );
		 result = false;
	}

	if( !sphereShader.load( "./assets/shaders/debug_sphere.vs",
							"./assets/shaders/debug_sphere.gs",
							"./assets/shaders/debug_sphere.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "DebugShapes.cpp - Failed to load debug sphere shader." );
		result = false;
	}

	if( !aabbShader.load( "./assets/shaders/debug_aabb.vs",
							"./assets/shaders/debug_aabb.gs",
							"./assets/shaders/debug_aabb.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "DebugShapes.cpp - Failed to load debug aabb shader." );
		result = false;
	}

	if( !obbShader.load( "./assets/shaders/debug_obb.vs",
							"./assets/shaders/debug_obb.gs",
							"./assets/shaders/debug_obb.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "DebugShapes.cpp - Failed to load debug obb shader." );
		result = false;
	}

	return result;
}

void DebugShapes::upload()
{
	LOG( VERBOSITY_INFORMATION, "DebugShapes.cpp - Uploading shaders." );

	//if( lineShader.getValid() )
	{
		lineProjectionMatrixLocation = lineShader.getLocation( "projectionMatrix" );
		lineViewMatrixLocation = lineShader.getLocation( "viewMatrix" );
		
		glGenVertexArrays( 1, &lineVAO );
		glBindVertexArray( lineVAO );

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );

		glGenBuffers( 1, &lineVBO );
		glBindBuffer( GL_ARRAY_BUFFER, lineVBO );

		const int STRIDE = sizeof(DebugLine);
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, STRIDE, 0 );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(glm::vec3) ) );
		glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(glm::vec3)*2) );

		glBindVertexArray( 0 );
	}

	//if( sphereShader.getValid() )
	{
		sphereProjectionMatrixLocation = sphereShader.getLocation( "projectionMatrix" );
		sphereViewMatrixLocation = sphereShader.getLocation( "viewMatrix" );

		glGenVertexArrays( 1, &sphereVAO );
		glBindVertexArray( sphereVAO );

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );

		glGenBuffers( 1, &sphereVBO );
		glBindBuffer( GL_ARRAY_BUFFER, sphereVBO );

		const int STRIDE = sizeof(DebugSphere);
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, STRIDE, 0 );
		glVertexAttribPointer( 1, 1, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(glm::vec3) ) );
		glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(glm::vec3)+ sizeof(float) ) );

		glBindVertexArray( 0 );
	}

	//if( aabbShader.getValid() )
	{
		aabbProjectionMatrixLocation = aabbShader.getLocation( "projectionMatrix" );
		aabbViewMatrixLocation = aabbShader.getLocation( "viewMatrix" );

		glGenVertexArrays( 1, &aabbVAO );
		glBindVertexArray( aabbVAO );

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );

		glGenBuffers( 1, &aabbVBO );
		glBindBuffer( GL_ARRAY_BUFFER, aabbVBO );

		const int STRIDE = sizeof(DebugSphere);
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, STRIDE, 0 );
		glVertexAttribPointer( 1, 1, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(glm::vec3) ) );
		glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(glm::vec3)* 2 ) );

		glBindVertexArray( 0 );
	}
}

void DebugShapes::unload()
{
}

void DebugShapes::render( Camera* camera )
{
}

void DebugShapes::finalize()
{
}