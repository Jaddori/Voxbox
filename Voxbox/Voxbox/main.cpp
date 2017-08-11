#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Input.h"
#include "Chunk.h"
#include "Font.h"
#include "Graphics.h"
#include "Console.h"
#include "DebugShapes.h"

struct GenerationData
{
	int first, last;
	Chunk* chunks;
};

DWORD WINAPI generateChunkFaces( LPVOID args )
{
	GenerationData* data = (GenerationData*)args;

	for( int i=data->first; i<data->last; i++ )
		data->chunks[i].calculateFaces();

	return 0;
}

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

			Graphics graphics;
			graphics.load();
			graphics.getChunkCamera().setPosition( glm::vec3( 0.0f, 0.0f, -10.0f ) );

			const int CHUNK_WIDTH = 10;
			const int CHUNK_DEPTH = 10;
			const int NUM_CHUNKS = CHUNK_WIDTH * CHUNK_DEPTH;
			Chunk* chunks = new Chunk[NUM_CHUNKS];

			for( int x=0; x<CHUNK_WIDTH; x++ )
			{
				for( int z=0; z<CHUNK_DEPTH; z++ )
				{
					chunks[x*CHUNK_WIDTH+z].setOffset( glm::vec3( x, 0.0f, z ) );
				}
			}

			const int NUM_THREADS = 4;
			HANDLE threads[NUM_THREADS];

			GenerationData threadData[NUM_THREADS];

			int first = 0;
			for( int i=0; i<NUM_THREADS; i++ )
			{
				int last = first + NUM_CHUNKS / NUM_THREADS;
				if( last > NUM_CHUNKS )
					last = NUM_CHUNKS;

				threadData[i].first = first;
				threadData[i].last = last;
				threadData[i].chunks = chunks;
				threads[i] = CreateThread( NULL, 0, generateChunkFaces, &threadData[i], 0, NULL );

				first = last;
			}

			WaitForMultipleObjects( NUM_THREADS, threads, TRUE, INFINITE );

			for( int i=0; i<NUM_CHUNKS; i++ )
			{
				chunks[i].upload();
			}

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

				long startChunkRenderTime = SDL_GetTicks();
				for( int x=0; x<CHUNK_WIDTH; x++ )
				{
					for( int z=0; z<CHUNK_DEPTH; z++ )
					{
						glm::vec3 minPosition = glm::vec3( x, 0.0f, z ) * (float)CHUNK_SIZE;
						glm::vec3 maxPosition = glm::vec3( x+1, 1, z+1 ) * (float)CHUNK_SIZE;

						if( frustum.aabbCollision( minPosition, maxPosition ) )
						{
							graphics.renderChunk( &chunks[x*CHUNK_WIDTH+z] );
						}
					}
				}
				long endChunkRenderTime = SDL_GetTicks();

				//printf( "%d ms\n", endChunkRenderTime-startChunkRenderTime );

				cameraFrustum.addDebugLines( debugShapes );

				debugShapes.finalize();
				debugShapes.render( graphics.getChunkCamera().getProjectionMatrix(), graphics.getChunkCamera().getViewMatrix() );

				console.render( &graphics );

				graphics.end();

				SDL_GL_SwapWindow( window );

				long ticks = SDL_GetTicks();
				if( ticks - fpsTimer > 1000 )
				{
					printf( "Fps: %d\n", fps );

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