#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Input.h"
#include "Chunk.h"
#include "Font.h"
#include "Graphics.h"
#include "DebugShapes.h"
#include "CoreData.h"
#include "LuaBinds.h"
#include "Assets.h"

#define THREAD_UPDATE_WAIT 1000
#define CHUNK_WIDTH 10
#define CHUNK_DEPTH 10
#define NUM_CHUNKS (CHUNK_WIDTH * CHUNK_DEPTH)

bool rayCheck( const DebugLine& ray, const glm::vec3& minPosition, const glm::vec3& maxPosition )
{
	float epsilon = glm::epsilon<float>();
	float tmin = 0.0f;
	float tmax = std::numeric_limits<float>().max();
	glm::vec3 rayDirection = glm::normalize( ray.end - ray.start );
	glm::vec3 rayPosition = ray.start;
	glm::vec3 aabbMin = minPosition;
	glm::vec3 aabbMax = maxPosition;

	unsigned int threeSlabs = 3;

	for (unsigned int i = 0; i < threeSlabs; i++)
	{
		if (glm::abs(rayDirection[i]) < epsilon) // Ray is parallell to slab
		{
			if (rayPosition[i] < aabbMin[i] || rayPosition[i] > aabbMax[i]) // No hit if origin not inside slab
				return false;
		}
		else
		{
			// compute intersection t value of ray with near and far plane of slab
			float ood = 1.0f / rayDirection[i];
			float t1 = (aabbMin[i] - rayPosition[i]) * ood;
			float t2 = (aabbMax[i] - rayPosition[i]) * ood;

			if (t1 > t2)
			{
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}

			if (t1 > tmin)
				tmin = t1;

			if (t2 < tmax)
				tmax = t2;

			if (tmin > tmax)
				return false;
		}

	}

	/*float hitdistance = tmin;
	if (tmin < 0)
		hitdistance = tmax;
	glm::vec3 intersectionPoint = rayPosition + (rayDirection * hitdistance);
	ray->hit(intersectionPoint, hitdistance);*/

	return true;
}

DWORD WINAPI generateChunks( LPVOID args )
{
	CoreData* data = (CoreData*)args;

	LOG_INFO( "Starting chunk generation thread." );

	for( int i=0; i<100; i++ )
	{
		data->chunks[i].calculateFaces();
		
		Sleep( 100 );
	}

	LOG_INFO( "Chunk generation thread finished." );

	return 0;
}

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
	Chunk* chunks				=	data->coreData->chunks;
	Graphics& graphics			=	*data->coreData->graphics;
	DebugShapes& debugShapes	=	*data->coreData->debugShapes;
	LuaBinds& luaBinds			=	*data->luaBinds;

	DebugLine projectLine;
	projectLine.color = glm::vec4( 1.0f, 1.0f, 0.0f, 1.0f );

	while( data->running )
	{
		DWORD result = WaitForSingleObject( data->renderDone, THREAD_UPDATE_WAIT );
		if( result == WAIT_OBJECT_0 )
		{
			if( input.keyReleased( SDL_SCANCODE_ESCAPE ) )
				data->running = false;

			if( input.buttonDown( SDL_BUTTON_LEFT ) )
			{
				Point mouseDelta = input.getMouseDelta();
				perspectiveCamera.updateDirection( mouseDelta.x, mouseDelta.y );
			}
			if( input.buttonReleased( SDL_BUTTON_RIGHT ) )
			{
				Point mousePosition = input.getMousePosition();
				perspectiveCamera.unproject( mousePosition, 0.0f, projectLine.start );
				perspectiveCamera.unproject( mousePosition, 1.0f, projectLine.end );

				bool collision = false;
				for( int x=0; x<CHUNK_WIDTH && !collision; x++ )
				{
					for( int z=0; z<CHUNK_DEPTH && !collision; z++ )
					{
						if( chunks[x*CHUNK_WIDTH+z].getUploaded() )
						{
							glm::vec3 minPosition = glm::vec3( x, 0.0f, z ) * (float)CHUNK_SIZE;
							glm::vec3 maxPosition = glm::vec3( x+1, 1, z+1 ) * (float)CHUNK_SIZE;

							if( rayCheck( projectLine, minPosition, maxPosition ) )
							{
								glm::vec3 offset = chunks[x*CHUNK_WIDTH+z].getOffset();
								chunks[x*CHUNK_WIDTH+z].setOffset( offset + glm::vec3( 0.0f, 1.0f, 0.0f ) );

								chunks[x*CHUNK_WIDTH+z].calculateFaces();
								chunks[x*CHUNK_WIDTH+z].setUploaded( false );

								collision = true;
							}
						}
					}
				}
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
				perspectiveCamera.updatePosition( cameraMovement );

			const Frustum& frustum = perspectiveCamera.getFrustum();

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
							graphics.queueChunk( &chunks[x*CHUNK_WIDTH+z] );

							DebugAABB aabb = { minPosition, maxPosition, glm::vec4( 1.0f, 0.0f, 1.0f, 1.0f ) };
							debugShapes.addAABB( aabb );
						}
					}
				}
			}

			luaBinds.update();
			luaBinds.render();

			debugShapes.addLine( projectLine );

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

			Assets assets;

			Graphics graphics;
			graphics.load( &assets );
			graphics.getPerspectiveCamera().setPosition( glm::vec3( 0.0f, 0.0f, -10.0f ) );

			Chunk* chunks = new Chunk[NUM_CHUNKS];

			for( int x=0; x<CHUNK_WIDTH; x++ )
			{
				for( int z=0; z<CHUNK_DEPTH; z++ )
				{
					chunks[x*CHUNK_WIDTH+z].setOffset( glm::vec3( x, 0.0f, z ) );
					chunks[x*CHUNK_WIDTH+z].noise( x*CHUNK_SIZE, z*CHUNK_SIZE );
				}
			}

			DebugShapes debugShapes;
			debugShapes.load();
			debugShapes.upload();

			Input input;

			CoreData coreData;
			coreData.perspectiveCamera = &graphics.getPerspectiveCamera();
			coreData.input = &input;
			coreData.chunks = chunks;
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

			HANDLE generationThread = CreateThread( NULL, 0, generateChunks, &coreData, 0, NULL );

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

					for( int i=0; i<NUM_CHUNKS; i++ )
					{
						if( chunks[i].getValid() && !chunks[i].getUploaded() )
						{
							chunks[i].upload();
						}
					}

					ReleaseSemaphore( threadData.renderDone, 1, NULL );
				}

				// render
				glClearColor( 0.15f, 0.15f, 0.15f, 0.0f );
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

				debugShapes.render( graphics.getPerspectiveCamera().getProjectionMatrix(), graphics.getPerspectiveCamera().getViewMatrix() );

				//console.render( &graphics );
				graphics.render();

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