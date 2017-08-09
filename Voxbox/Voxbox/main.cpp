#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Input.h"
#include "Chunk.h"
#include "Font.h"
#include "Graphics.h"

int main( int argc, char* argv[] )
{
	SDL_Window* window = SDL_CreateWindow( "Voxbox", WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
	if( window )
	{
		SDL_GLContext context = SDL_GL_CreateContext( window );
		if( context )
		{
			glewExperimental = GL_TRUE;
			if( glewInit() != GLEW_OK )
			{
				printf( "Failed to init GLEW.\n" );
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

			Input input;

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

				// update

				// render
				glClearColor( 0.0f, 0.0f, 1.0f, 0.0f );
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

				graphics.begin();
				texture.bind();

				for( int y = 0; y<2; y++ )
				{
					for( int x = 0; x<2; x++ )
					{
						for( int z = 0; z<2; z++ )
						{
							//chunkRenderer.render( &chunks[y*2*2+x*2+z] );
							graphics.renderChunk( &chunks[y*2*2+x*2+z] );
						}
					}
				}

				graphics.renderText( &font, "Testing...", glm::vec2( 32.0f, 32.0f ) );
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

			SDL_GL_DeleteContext( context );
		}
		SDL_DestroyWindow( window );
	}

	return 0;
}