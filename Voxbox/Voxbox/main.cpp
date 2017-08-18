#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Input.h"
#include "World.h"
#include "Font.h"
#include "Graphics.h"
#include "DebugShapes.h"
#include "CoreData.h"
#include "LuaBinds.h"
#include "Assets.h"
#include "ThreadPool.h"

#define THREAD_UPDATE_WAIT 1000

struct ThreadData
{
	CoreData* coreData;
	LuaBinds* luaBinds;
	bool running;
	HANDLE updateDone;
	HANDLE renderDone;
};

DWORD WINAPI update( LPVOID args )
{
	ThreadData* data = (ThreadData*)args;

	Camera& perspectiveCamera	=	*data->coreData->perspectiveCamera;
	Input& input				=	*data->coreData->input;
	World& world				=	*data->coreData->world;
	Graphics& graphics			=	*data->coreData->graphics;
	DebugShapes& debugShapes	=	*data->coreData->debugShapes;
	LuaBinds& luaBinds			=	*data->luaBinds;

	while( data->running )
	{
		DWORD result = WaitForSingleObject( data->renderDone, THREAD_UPDATE_WAIT );
		if( result == WAIT_OBJECT_0 )
		{
			if( input.keyReleased( SDL_SCANCODE_ESCAPE ) )
				data->running = false;

			const Frustum& frustum = perspectiveCamera.getFrustum();
			world.queueChunks( data->coreData, frustum );

			luaBinds.update();
			luaBinds.render();

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
		LOG_INFO( "Window created." );

		SDL_GLContext context = SDL_GL_CreateContext( window );
		if( context )
		{
			LOG_INFO( "OpenGL context created." );

			glewExperimental = GL_TRUE;
			if( glewInit() == GLEW_OK )
			{
				LOG_INFO( "GLEW initialized." );
			}
			else
			{
				LOG_ERROR( "Failed to initialize GLEW." );
				return -1;
			}

			glEnable( GL_DEPTH_TEST );
			glEnable( GL_CULL_FACE );
			SDL_GL_SetSwapInterval( 1 );
			//srand( time( 0 ) );
			srand( 1337 );

			ThreadPool threadPool;
			threadPool.load();

			Assets assets;

			Graphics graphics;
			graphics.load( &assets );
			graphics.getPerspectiveCamera().setPosition( glm::vec3( 0.0f, 0.0f, -10.0f ) );

			World world;
			world.load( &threadPool );

			DebugShapes debugShapes;
			debugShapes.load();
			debugShapes.upload();

			Input input;

			CoreData coreData;
			coreData.perspectiveCamera = &graphics.getPerspectiveCamera();
			coreData.input = &input;
			coreData.world = &world;
			coreData.graphics = &graphics;
			coreData.debugShapes = &debugShapes;
			coreData.assets = &assets;

			LuaBinds luaBinds;
			luaBinds.bind( &coreData );
			luaBinds.load();

			ThreadData threadData;
			threadData.coreData = &coreData;
			threadData.luaBinds = &luaBinds;
			threadData.running = true;
			threadData.updateDone = CreateSemaphore( NULL, 0, 1, NULL );
			threadData.renderDone = CreateSemaphore( NULL, 1, 1, NULL );
			HANDLE updateThread = CreateThread( NULL, 0, update, &threadData, 0, NULL );

			long fpsTimer = SDL_GetTicks();
			int fps = 0;

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
					assets.upload();
					graphics.finalize();
					debugShapes.finalize();

					world.upload();

					threadPool.schedule();

					ReleaseSemaphore( threadData.renderDone, 1, NULL );
				}

				// render
				glClearColor( 0.15f, 0.15f, 0.15f, 0.0f );
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

				graphics.render();

				debugShapes.render( graphics.getPerspectiveCamera().getProjectionMatrix(), graphics.getPerspectiveCamera().getViewMatrix() );

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

			threadPool.unload();

			LOG_INFO( "Waiting for update thread to finish." );
			WaitForSingleObject( updateThread, INFINITE );

			LOG_INFO( "Deleting OpenGL context." );
			SDL_GL_DeleteContext( context );
		}

		LOG_INFO( "Destroying window." );
		SDL_DestroyWindow( window );
	}

	LOG_STOP();

	return 0;
}