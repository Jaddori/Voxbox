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
#include "SystemInfo.h"
#include <errno.h>

#define THREAD_UPDATE_WAIT 1000

struct ThreadData
{
	CoreData* coreData;
	LuaBinds* luaBinds;
	SDL_semaphore* updateDone;
	SDL_semaphore* renderDone;
};

int update( void* args )
{
	ThreadData* data = (ThreadData*)args;

	Camera& perspectiveCamera	=	*data->coreData->perspectiveCamera;
	Input& input				=	*data->coreData->input;
	World& world				=	*data->coreData->world;
	Graphics& graphics			=	*data->coreData->graphics;
	DebugShapes& debugShapes	=	*data->coreData->debugShapes;
	LuaBinds& luaBinds			=	*data->luaBinds;

	long lastTick = SDL_GetTicks();

	while( *data->coreData->running )
	{
		int result = SDL_SemWaitTimeout( data->renderDone, THREAD_UPDATE_WAIT );
		if( result == 0 )
		{
			// stop execution
			if( input.keyReleased( SDL_SCANCODE_ESCAPE ) )
			{
				*data->coreData->running = false;
				*data->coreData->executing = false;
			}
			// restart game
			else if( input.keyReleased( SDL_SCANCODE_TAB ) )
			{
				*data->coreData->running = false;
			}

			const Frustum& frustum = perspectiveCamera.getFrustum();
			world.queueChunks( data->coreData, frustum );

			long curTick = SDL_GetTicks();
			float deltaTime = ( curTick - lastTick ) * 0.001f; // delta time in seconds
			lastTick = curTick;

			luaBinds.update( deltaTime );
			luaBinds.render();

			SDL_SemPost( data->updateDone );
		}
		else if( result == -1 )
		{
			LOG_ERROR( "Update thread encountered error when waiting on semaphore." );
		}
	}

	return 0;
}

int main( int argc, char* argv[] )
{
	LOG_START( "./log.txt" );
	LOG_WARNINGS();
	
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		LOG_ERROR( "Failed to initialize SDL." );
		return -1;
	}
	
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	
	SDL_Window* window = SDL_CreateWindow( "Voxbox", WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
	if( window )
	{
		LOG_INFO( "Window created." );

		SDL_GLContext context = SDL_GL_CreateContext( window );
		if( context )
		{
			LOG_INFO( "OpenGL context created." );

#ifdef _WIN32
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
#endif

			glEnable( GL_DEPTH_TEST );
			glEnable( GL_CULL_FACE );
			SDL_GL_SetSwapInterval( 1 );

			bool executing = true;
			while( executing )
			{
				bool running = true;

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

				SystemInfo systemInfo;
				systemInfo.poll();

				CoreData coreData;
				coreData.perspectiveCamera = &graphics.getPerspectiveCamera();
				coreData.input = &input;
				coreData.world = &world;
				coreData.graphics = &graphics;
				coreData.debugShapes = &debugShapes;
				coreData.assets = &assets;
				coreData.systemInfo = &systemInfo;
				coreData.running = &running;
				coreData.executing = &executing;
			
				LuaBinds luaBinds;
				luaBinds.bind( &coreData );
				luaBinds.load();
			
				ThreadData threadData;
				threadData.coreData = &coreData;
				threadData.luaBinds = &luaBinds;
				
				threadData.updateDone = SDL_CreateSemaphore( 0 );
				threadData.renderDone = SDL_CreateSemaphore( 1 );
				
				SDL_Thread* updateThread = SDL_CreateThread( update, nullptr, &threadData );
				
				long fpsTimer = SDL_GetTicks();
				int fps = 0;

				while( running )
				{
					int waitResult = SDL_SemWait( threadData.updateDone );
					if( waitResult == 0 )
					{
						input.reset();

						// events
						SDL_Event e;
						while( SDL_PollEvent( &e ) )
						{
							if( e.type == SDL_QUIT )
								executing = running = false;
							input.update( &e );
						}

						// finalize objects
						assets.upload();
						graphics.finalize();
						debugShapes.finalize();

						world.upload();

						threadPool.schedule();

						SDL_SemPost( threadData.renderDone );
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

				LOG_INFO( "Waiting for update thread to finish." );
				SDL_WaitThread( updateThread, nullptr );

				luaBinds.unload();
				threadPool.unload();
				assets.unload();
				graphics.unload();
				world.unload();
				debugShapes.unload();				
			}

			LOG_INFO( "Deleting OpenGL context." );
			SDL_GL_DeleteContext( context );
		}

		LOG_INFO( "Destroying window." );
		SDL_DestroyWindow( window );
	}

	LOG_STOP();

	return 0;
}
