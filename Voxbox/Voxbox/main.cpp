#include "Camera.h"
#include "Shader.h"
#include "Texture.h"

int main( int argc, char* argv[] )
{
	SDL_Window* window = SDL_CreateWindow( "Voxbox", WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
	if( window )
	{
		SDL_GLContext context = SDL_GL_CreateContext( window );
		if( context )
		{
			bool running = true;
			while( running )
			{
				// events
				SDL_Event e;
				while( SDL_PollEvent( &e ) )
				{
					if( e.type == SDL_QUIT )
						running = false;
					else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE )
						running = false;
				}

				// update

				// render
				glClearColor( 1.0f, 0.0f, 0.0f, 0.0f );
				glClear( GL_COLOR_BUFFER_BIT );

				glBegin( GL_TRIANGLES );
					glVertex3f( 0.0f, -1.0f, 0.0f );
					glVertex3f( 1.0f, 1.0f, 0.0f );
					glVertex3f( -1.0f, 1.0f, 0.0f );
				glEnd();

				SDL_GL_SwapWindow( window );
			}

			SDL_GL_DeleteContext( context );
		}
		SDL_DestroyWindow( window );
	}

	return 0;
}