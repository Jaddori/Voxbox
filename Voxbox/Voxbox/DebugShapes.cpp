#include "DebugShapes.h"

DebugShapes::DebugShapes()
{
	LOG_INFO( "Constructing." );
}

DebugShapes::~DebugShapes()
{
	LOG_INFO( "Destructing." );
}

bool DebugShapes::load()
{
	LOG_INFO( "Loading shaders." );

	bool result = false;

	if( !lineShader.load( "./assets/shaders/debug_line.vs",
							"./assets/shaders/debug_line.gs",
							"./assets/shaders/debug_shape.fs" ) )
	{
		LOG_ERROR( "Failed to load line shader." );
		result = false;
	}

	if( !sphereShader.load( "./assets/shaders/debug_sphere.vs",
							"./assets/shaders/debug_sphere.gs",
							"./assets/shaders/debug_shape.fs" ) )
	{
		LOG_ERROR( "Failed to load sphere shader." );
		result = false;
	}

	if( !aabbShader.load( "./assets/shaders/debug_aabb.vs",
							"./assets/shaders/debug_aabb.gs",
							"./assets/shaders/debug_shape.fs" ) )
	{
		LOG_ERROR( "Failed to load AABB shader." );
		result = false;
	}

	if( !obbShader.load( "./assets/shaders/debug_obb.vs",
							"./assets/shaders/debug_obb.gs",
							"./assets/shaders/debug_shape.fs" ) )
	{
		LOG_ERROR( "Failed to load OBB shader." );
		result = false;
	}

	return result;
}

void DebugShapes::upload()
{
	LOG_INFO( "Uploading shaders." );

	if( lineShader.getValid() )
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
		glBufferData( GL_ARRAY_BUFFER, sizeof(DebugLine)*DEBUG_SHAPES_MAX_LINES, nullptr, GL_STREAM_DRAW );

		const int STRIDE = sizeof(DebugLine);
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, STRIDE, 0 );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(glm::vec3) ) );
		glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(glm::vec3)*2) );

		glBindVertexArray( 0 );
	}

	if( sphereShader.getValid() )
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
		glBufferData( GL_ARRAY_BUFFER, sizeof(DebugSphere)*DEBUG_SHAPES_MAX_SPHERES, nullptr, GL_STREAM_DRAW );

		const int STRIDE = sizeof(DebugSphere);
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, STRIDE, 0 );
		glVertexAttribPointer( 1, 1, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(glm::vec3) ) );
		glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(glm::vec3)+ sizeof(float) ) );

		glBindVertexArray( 0 );
	}

	if( aabbShader.getValid() )
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
		glBufferData( GL_ARRAY_BUFFER, sizeof(DebugAABB)*DEBUG_SHAPES_MAX_AABB, nullptr, GL_STREAM_DRAW );

		const int STRIDE = sizeof(DebugAABB);
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, STRIDE, 0 );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(glm::vec3) ) );
		glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(glm::vec3)* 2 ) );

		glBindVertexArray( 0 );
	}

	if( obbShader.getValid() )
	{
		obbProjectionMatrixLocation = obbShader.getLocation( "projectionMatrix" );
		obbViewMatrixLocation = obbShader.getLocation( "viewMatrix" );

		glGenVertexArrays( 1, &obbVAO );
		glBindVertexArray( obbVAO );

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );
		glEnableVertexAttribArray( 3 );
		glEnableVertexAttribArray( 4 );
		glEnableVertexAttribArray( 5 );

		glGenBuffers( 1, &obbVBO );
		glBindBuffer( GL_ARRAY_BUFFER, obbVBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(DebugOBB)*DEBUG_SHAPES_MAX_OBB, nullptr, GL_STREAM_DRAW );

		const int STRIDE = sizeof(DebugOBB);
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, STRIDE, 0 );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(glm::vec3) ) );
		glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(glm::vec3)* 2 ) );
		glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(glm::vec3)* 3 ) );
		glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(glm::vec3)* 4 ) );
		glVertexAttribPointer( 5, 4, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(glm::vec3)* 5 ) );

		glBindVertexArray( 0 );
	}
}

void DebugShapes::unload()
{
}

void DebugShapes::render( const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix )
{
	const int LINE_COUNT = lines.getRead().getSize();
	const int SPHERE_COUNT = spheres.getRead().getSize();
	const int AABB_COUNT = AABBs.getRead().getSize();
	const int OBB_COUNT = OBBs.getRead().getSize();

	if( LINE_COUNT > 0 )
	{
		lineShader.bind();
		lineShader.setMat4( lineProjectionMatrixLocation, projectionMatrix );
		lineShader.setMat4( lineViewMatrixLocation, viewMatrix );

		glBindVertexArray( lineVAO );
		glBindBuffer( GL_ARRAY_BUFFER, lineVBO );

		int offset = 0;
		while( offset < LINE_COUNT )
		{
			int count = LINE_COUNT - offset;
			if( count > DEBUG_SHAPES_MAX_LINES )
				count = DEBUG_SHAPES_MAX_LINES;

			glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(DebugLine)*count, lines.getRead().getData() + offset );
			glDrawArrays( GL_POINTS, 0, count );

			offset += count;
		}
	}

	/*if( NUM_SPHERES > 0 )
	{
		glBindVertexArray( sphereVAO );
		glBindBuffer( GL_ARRAY_BUFFER, sphereVBO );
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(DebugSphere)*NUM_SPHERES, spheres.getRead().getData() );

		sphereShader.bind();
		sphereShader.setMat4( sphereProjectionMatrixLocation, projectionMatrix );
		sphereShader.setMat4( sphereViewMatrixLocation, viewMatrix );

		glDrawArrays( GL_POINTS, 0, NUM_SPHERES );
	}*/

	if( SPHERE_COUNT > 0 )
	{
		sphereShader.bind();
		sphereShader.setMat4( sphereProjectionMatrixLocation, projectionMatrix );
		sphereShader.setMat4( sphereViewMatrixLocation, viewMatrix );

		glBindVertexArray( sphereVAO );
		glBindBuffer( GL_ARRAY_BUFFER, sphereVBO );

		int offset = 0;
		while( offset < SPHERE_COUNT )
		{
			int count = SPHERE_COUNT - offset;
			if( count > DEBUG_SHAPES_MAX_SPHERES )
				count = DEBUG_SHAPES_MAX_SPHERES;

			glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(DebugSphere)*count, spheres.getRead().getData() + offset );
			glDrawArrays( GL_POINTS, 0, count );

			offset += count;
		}
	}

	/*if( NUM_AABB > 0 )
	{
		glBindVertexArray( aabbVAO );
		glBindBuffer( GL_ARRAY_BUFFER, aabbVBO );
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(DebugAABB)*NUM_AABB, AABBs.getRead().getData() );

		aabbShader.bind();
		aabbShader.setMat4( aabbProjectionMatrixLocation, projectionMatrix );
		aabbShader.setMat4( aabbViewMatrixLocation, viewMatrix );

		glDrawArrays( GL_POINTS, 0, NUM_AABB );
	}*/

	if( AABB_COUNT > 0 )
	{
		aabbShader.bind();
		aabbShader.setMat4( aabbProjectionMatrixLocation, projectionMatrix );
		aabbShader.setMat4( aabbViewMatrixLocation, viewMatrix );

		glBindVertexArray( aabbVAO );
		glBindBuffer( GL_ARRAY_BUFFER, aabbVBO );

		int offset = 0;
		while( offset < AABB_COUNT )
		{
			int count = AABB_COUNT - offset;
			if( count > DEBUG_SHAPES_MAX_AABB )
				count = DEBUG_SHAPES_MAX_AABB;

			glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(DebugAABB)*count, AABBs.getRead().getData() + offset );
			glDrawArrays( GL_POINTS, 0, count );

			offset += count;
		}
	}

	/*if( NUM_OBB > 0 )
	{
		glBindVertexArray( obbVAO );
		glBindBuffer( GL_ARRAY_BUFFER, obbVBO );
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(DebugOBB)*NUM_OBB, OBBs.getRead().getData() );

		obbShader.bind();
		obbShader.setMat4( obbProjectionMatrixLocation, projectionMatrix );
		obbShader.setMat4( obbViewMatrixLocation, viewMatrix );

		glDrawArrays( GL_POINTS, 0, NUM_OBB );
	}*/

	if( OBB_COUNT > 0 )
	{
		obbShader.bind();
		obbShader.setMat4( obbProjectionMatrixLocation, projectionMatrix );
		obbShader.setMat4( obbViewMatrixLocation, viewMatrix );

		glBindVertexArray( obbVAO );
		glBindBuffer( GL_ARRAY_BUFFER, obbVBO );

		int offset = 0;
		while( offset < OBB_COUNT )
		{
			int count = OBB_COUNT - offset;
			if( count > DEBUG_SHAPES_MAX_OBB )
				count = DEBUG_SHAPES_MAX_OBB;

			glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(DebugOBB)*count, OBBs.getRead().getData() + offset );
			glDrawArrays( GL_POINTS, 0, count );

			offset += count;
		}
	}

	// reset
	glBindVertexArray( 0 );
}

void DebugShapes::finalize()
{
	lines.swap();
	spheres.swap();
	AABBs.swap();
	OBBs.swap();

	lines.getWrite().clear();
	spheres.getWrite().clear();
	AABBs.getWrite().clear();
	OBBs.getWrite().clear();
}

void DebugShapes::addLine( const DebugLine& line )
{
	lines.getWrite().add( line );
}

void DebugShapes::addSphere( const DebugSphere& sphere )
{
	spheres.getWrite().add( sphere );
}

void DebugShapes::addAABB( const DebugAABB& aabb )
{
	AABBs.getWrite().add( aabb );
}

void DebugShapes::addOBB( const DebugOBB& obb )
{
	OBBs.getWrite().add( obb );
}