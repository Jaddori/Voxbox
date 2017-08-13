#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Input.h"
#include "Chunk.h"
#include "Font.h"
#include "Graphics.h"
#include "Console.h"
#include "DebugShapes.h"
#include "CoreData.h"

#define THREAD_UPDATE_WAIT 1000

DWORD WINAPI generateChunks( LPVOID args )
{
	CoreData* data = (CoreData*)args;

	LOG( VERBOSITY_INFORMATION, "main.cpp - Starting chunk generation thread." );

	for( int i=0; i<100; i++ )
	{
		data->chunks[i].calculateFaces();
		
		Sleep( 100 );
	}

	LOG( VERBOSITY_INFORMATION, "main.cpp - Chunk generation thread finished." );

	return 0;
}

struct ThreadData
{
	CoreData* coreData;
	bool running;
	HANDLE updateDone;
	HANDLE renderDone;
};

DWORD WINAPI update( LPVOID args )
{
	ThreadData* data = (ThreadData*)args;

	while( data->running )
	{
		DWORD result = WaitForSingleObject( data->renderDone, THREAD_UPDATE_WAIT );
		if( result == WAIT_OBJECT_0 )
		{
			if( data->coreData->input->keyReleased( SDL_SCANCODE_ESCAPE ) )
				data->running = false;

			if( data->coreData->input->buttonDown( SDL_BUTTON_LEFT ) )
			{
				Point mouseDelta = data->coreData->input->getMouseDelta();
				data->coreData->perspectiveCamera->updateDirection( mouseDelta.x, mouseDelta.y );
			}

			glm::vec3 cameraMovement;
			if( data->coreData->input->keyDown( SDL_SCANCODE_W ) )
				cameraMovement.z += 1.0f;
			if( data->coreData->input->keyDown( SDL_SCANCODE_S ) )
				cameraMovement.z -= 1.0f;
			if( data->coreData->input->keyDown( SDL_SCANCODE_D ) )
				cameraMovement.x += 1.0f;
			if( data->coreData->input->keyDown( SDL_SCANCODE_A ) )
				cameraMovement.x -= 1.0f;
			if( glm::length( cameraMovement ) > 0 )
				data->coreData->perspectiveCamera->updatePosition( cameraMovement );

			if( data->coreData->input->keyReleased( SDL_SCANCODE_GRAVE ) )
				data->coreData->console->toggle();

			if( data->coreData->input->keyReleased( SDL_SCANCODE_SPACE ) )
				LOG( VERBOSITY_DEBUG, "main.cpp - User pressed the spacebar." );

			DebugSphere sphere = { glm::vec3( 0.0f, 0.0f, 0.0f ), 2.0f, glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ) };
			data->coreData->debugShapes->addSphere( sphere );

			ReleaseSemaphore( data->updateDone, 1, NULL );
		}
	}

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
			//srand( time( 0 ) );
			srand( 1337 );

			Texture texture;
			if( !texture.load( "./assets/textures/blocks.dds" ) )
				printf( "Failed to load texture.\n" );
			else
				texture.upload();

			Font font;
			if( font.load( "./assets/fonts/verdana12.bin", "./assets/fonts/verdana12.dds" ) )
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
					chunks[x*CHUNK_WIDTH+z].noise( x*CHUNK_SIZE, z*CHUNK_SIZE );
				}
			}

			/*for( int i=0; i<NUM_CHUNKS; i++ )
			{
				chunks[i].calculateFaces();
				chunks[i].upload();
			}*/

			DebugShapes debugShapes;
			debugShapes.load();
			debugShapes.upload();

			Console console;
			console.load();
			//console.setVisible( true );

			Input input;

			Frustum cameraFrustum;

			CoreData coreData;
			coreData.perspectiveCamera = &graphics.getChunkCamera();
			coreData.input = &input;
			coreData.console = &console;
			coreData.chunks = chunks;
			coreData.debugShapes = &debugShapes;

			ThreadData threadData;
			threadData.coreData = &coreData;
			threadData.running = true;
			threadData.updateDone = CreateSemaphore( NULL, 0, 1, NULL );
			threadData.renderDone = CreateSemaphore( NULL, 1, 1, NULL );
			HANDLE updateThread = CreateThread( NULL, 0, update, &threadData, 0, NULL );

			HANDLE generationThread = CreateThread( NULL, 0, generateChunks, &coreData, 0, NULL );

			long fpsTimer = SDL_GetTicks();
			int fps = 0;

			int currentChunk = 0;

			while( threadData.running )
			{
				if( WaitForSingleObject( threadData.updateDone, INFINITE ) == WAIT_OBJECT_0 )
				{
					input.reset();

					// events
					SDL_Event e;
					while( SDL_PollEvent( &e ) )
					{
						if( e.type == SDL_QUIT )
							threadData.running = false;

						input.update( &e );
					}

					// finalize objects
					graphics.getChunkCamera().finalize();
					graphics.getTextCamera().finalize();
					console.finalize();
					cameraFrustum.addDebugLines( debugShapes );
					debugShapes.finalize();

					if( currentChunk < NUM_CHUNKS )
					{
						if( chunks[currentChunk].getValid() )
						{
							chunks[currentChunk].upload();
							currentChunk++;
						}
					}

					ReleaseSemaphore( threadData.renderDone, 1, NULL );
				}

				// render
				glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

				graphics.begin();
				texture.bind();

				const Frustum& frustum = graphics.getChunkCamera().getFrustum();

				for( int x=0; x<CHUNK_WIDTH; x++ )
				{
					for( int z=0; z<CHUNK_DEPTH; z++ )
					{
						if( chunks[x*CHUNK_WIDTH+z].getUploaded() )
						{
							glm::vec3 minPosition = glm::vec3( x, 0.0f, z ) * (float)CHUNK_SIZE;
							glm::vec3 maxPosition = glm::vec3( x+1, 1, z+1 ) * (float)CHUNK_SIZE;

							if( frustum.aabbCollision( minPosition, maxPosition ) )
							{
								graphics.renderChunk( &chunks[x*CHUNK_WIDTH+z] );
							}
						}
					}
				}

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

			LOG( VERBOSITY_INFORMATION, "main.cpp - Waiting for update thread to finish." );
			WaitForSingleObject( updateThread, INFINITE );

			LOG( VERBOSITY_INFORMATION, "main.cpp - Deleting OpenGL context." );
			SDL_GL_DeleteContext( context );
		}

		LOG( VERBOSITY_INFORMATION, "main.cpp - Destroying window." );
		SDL_DestroyWindow( window );
	}

	LOG_STOP();

	return 0;
}