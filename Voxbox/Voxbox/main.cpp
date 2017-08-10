#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Input.h"
#include "Chunk.h"
#include "Font.h"
#include "Graphics.h"
#include "Console.h"
#include "DebugShapes.h"

int main( int argc, char* argv[] )
{
	LOG_START( "./log.txt" );
	LOG_WARNINGS();

	SDL_Window* window = SDL_CreateWindow( "Voxbox", WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
	if( window )
	{
		LOG( VERBOSITY_INFORMATION, "main.cpp - Window created." );

		SDL_GLContext context = SDL_GL_CreateContext( window );
		if( context )
		{
			LOG( VERBOSITY_INFORMATION, "main.cpp - OpenGL context created." );

			glewExperimental = GL_TRUE;
			if( glewInit() == GLEW_OK )
			{
				LOG( VERBOSITY_INFORMATION, "main.cpp - GLEW initialized." );
			}
			else
			{
				LOG( VERBOSITY_ERROR, "main.cpp - Failed to intialized GLEW." );
				return -1;
			}

			glEnable( GL_DEPTH_TEST );
			glEnable( GL_CULL_FACE );
			SDL_GL_SetSwapInterval( 1 );
			srand( time( 0 ) );

			Texture texture;
			if( !texture.load( "./assets/textures/blocks.dds" ) )
				printf( "Failed to load texture.\n" );
			else
				texture.upload();

			Font font;
			if( font.load( "./assets/fonts/modern12.bin", "./assets/fonts/modern12.dds" ) )
			{
				font.upload();
			}
			else
				printf( "Failed to load font.\n" );

			Chunk chunks[8];
			for( int y=0; y<2; y++ )
			{
				for( int x=0; x<2; x++ )
				{
					for( int z=0; z<2; z++ )
					{
						chunks[y*2*2+x*2+z].calculatePositions();
						chunks[y*2*2+x*2+z].setOffset( glm::vec3( x, y, z ) );
					}
				}
			}

			Graphics graphics;
			graphics.load();
			graphics.getChunkCamera().setPosition( glm::vec3( 0.0f, 0.0f, -10.0f ) );

			DebugShapes debugShapes;
			debugShapes.load();
			debugShapes.upload();

			Console console;
			console.load();
			//console.setVisible( true );

			Input input;

			Frustum cameraFrustum;

			long fpsTimer = SDL_GetTicks();
			int fps = 0;

			bool running = true;
			while( running )
			{
				input.reset();

				// events
				SDL_Event e;
				while( SDL_PollEvent( &e ) )
				{
					if( e.type == SDL_QUIT )
						running = false;

					input.update( &e );
				}
				
				if( input.keyReleased( SDL_SCANCODE_ESCAPE ) )
					running = false;

				if( input.buttonDown( SDL_BUTTON_LEFT ) )
				{
					Point mouseDelta = input.getMouseDelta();
					graphics.getChunkCamera().updateDirection( mouseDelta.x, mouseDelta.y );
				}

				glm::vec3 cameraMovement;
				if( input.keyDown( SDL_SCANCODE_W ) )
					cameraMovement.z += 1.0f;
				if( input.keyDown( SDL_SCANCODE_S ) )
					cameraMovement.z -= 1.0f;
				if( input.keyDown( SDL_SCANCODE_D ) )
					cameraMovement.x += 1.0f;
				if( input.keyDown( SDL_SCANCODE_A ) )
					cameraMovement.x -= 1.0f;
				if( glm::length( cameraMovement ) > 0 )
					graphics.getChunkCamera().updatePosition( cameraMovement );

				if( input.keyReleased( SDL_SCANCODE_GRAVE ) )
					console.toggle();

				if( input.keyReleased( SDL_SCANCODE_SPACE ) )
					LOG( VERBOSITY_DEBUG, "main.cpp - User pressed the spacebar." );

				if( input.keyReleased( SDL_SCANCODE_F ) )
				{
					LOG( VERBOSITY_DEBUG, "main.cpp - Getting camera frustum." );
					cameraFrustum = graphics.getChunkCamera().getFrustum();
				}

				// update

				// render
				glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

				graphics.begin();
				texture.bind();

				const Frustum& frustum = graphics.getChunkCamera().getFrustum();

				for( int y = 0; y<1; y++ )
				{
					for( int x = 0; x<1; x++ )
					{
						for( int z = 0; z<1; z++ )
						{
							glm::vec3 chunkPosition( x*CHUNK_SIZE, y*CHUNK_SIZE, z*CHUNK_SIZE );
							if( graphics.getChunkCamera().getFrustum().intersectCube( chunkPosition, CHUNK_SIZE ) )
							{
								graphics.renderChunk( &chunks[y*2*2+x*2+z] );
								printf("DARWIGN\n");
							}
							else
								printf("NOIT WARNIGN\n");
							
							/*DebugAABB aabb =
							{
								glm::vec3( x, y, z )*(float)CHUNK_SIZE,
								glm::vec3( x+1.0f, y+1.0f, z+1.0f )*(float)CHUNK_SIZE,
								glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f )
							};

							debugShapes.addAABB( aabb );*/

							glm::vec4 green( 0.0f, 1.0f, 0.0f, 1.0f );
							glm::vec4 red( 1.0f, 0.0f, 0.0f, 1.0f );

							DebugLine line;
							line.start = glm::vec3( x, y, z ) * (float)CHUNK_SIZE;
							line.end = line.start + glm::vec3( 0.0f, 0.5f, 0.0f );
							line.color = ( frustum.intersectPoint( line.start ) > Frustum::OUTSIDE ? green : red );

							debugShapes.addLine( line );

							line.start.x += CHUNK_SIZE;
							line.end.x += CHUNK_SIZE;
							line.color = ( frustum.intersectPoint( line.start ) > Frustum::OUTSIDE ? green : red );
							debugShapes.addLine( line );

							line.start.z += CHUNK_SIZE;
							line.end.z += CHUNK_SIZE;
							line.color = ( frustum.intersectPoint( line.start ) > Frustum::OUTSIDE ? green : red );
							debugShapes.addLine( line );

							line.start.x -= CHUNK_SIZE;
							line.end.x -= CHUNK_SIZE;
							line.color = ( frustum.intersectPoint( line.start ) > Frustum::OUTSIDE ? green : red );
							debugShapes.addLine( line );

							line.start = glm::vec3( x, y+1, z ) * (float)CHUNK_SIZE;
							line.end = line.start + glm::vec3( 0.0f, 0.5f, 0.0f );
							line.color = ( frustum.intersectPoint( line.start ) > Frustum::OUTSIDE ? green : red );
							debugShapes.addLine( line );

							line.start.x += CHUNK_SIZE;
							line.end.x += CHUNK_SIZE;
							line.color = ( frustum.intersectPoint( line.start ) > Frustum::OUTSIDE ? green : red );
							debugShapes.addLine( line );

							line.start.z += CHUNK_SIZE;
							line.end.z += CHUNK_SIZE;
							line.color = ( frustum.intersectPoint( line.start ) > Frustum::OUTSIDE ? green : red );
							debugShapes.addLine( line );

							line.start.x -= CHUNK_SIZE;
							line.end.x -= CHUNK_SIZE;
							line.color = ( frustum.intersectPoint( line.start ) > Frustum::OUTSIDE ? green : red );
							debugShapes.addLine( line );
						}
					}
				}

				cameraFrustum.addDebugLines( debugShapes );

				debugShapes.finalize();
				debugShapes.render( graphics.getChunkCamera().getProjectionMatrix(), graphics.getChunkCamera().getViewMatrix() );

				console.render( &graphics );

				graphics.end();

				SDL_GL_SwapWindow( window );

				long ticks = SDL_GetTicks();
				if( ticks - fpsTimer > 1000 )
				{
					//printf( "Fps: %d\n", fps );

					fpsTimer = ticks;
					fps = 0;
				}
				else
					fps++;
			}

			LOG( VERBOSITY_INFORMATION, "main.cpp - Deleting OpenGL context." );
			SDL_GL_DeleteContext( context );
		}

		LOG( VERBOSITY_INFORMATION, "main.cpp - Destroying window." );
		SDL_DestroyWindow( window );
	}

	LOG_STOP();

	return 0;
}