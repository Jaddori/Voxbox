#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Input.h"

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

			Camera camera;
			camera.setPosition( glm::vec3( 0.0f, 0.0f, -10.0f ) );
			camera.updatePerspective( WINDOW_WIDTH, WINDOW_HEIGHT );

			Shader shader;
			if( !shader.load( "./assets/shaders/basic.vs", 
						nullptr,
						"./assets/shaders/basic.fs" ) )
				printf( "Failed to load shader.\n" );

			GLint viewMatrixLocation = shader.getLocation( "viewMatrix" );
			GLint projectionMatrixLocation = shader.getLocation( "projectionMatrix" );
			GLint diffuseMapLocation = shader.getLocation( "diffuseMap" );

			Texture texture;
			if( !texture.load( "./assets/textures/bricks1.dds" ) )
				printf( "Failed to load texture.\n" );
			texture.upload();

			GLuint vao;
			glGenVertexArrays( 1, &vao );
			glBindVertexArray( vao );

			glEnableVertexAttribArray( 0 );
			glEnableVertexAttribArray( 1 );

			GLuint vbo, ibo;
			glGenBuffers( 1, &vbo );
			glGenBuffers( 1, &ibo );

			float vdata[] = { 0.0f, 1.0f, 0.5f, 0.0f,
								1.0f, -1.0f, 1.0f, 1.0f,
								-1.0f, -1.0f, 0.0f, 1.0f };
			GLuint idata[] = { 0, 1, 2 };

			glBindBuffer( GL_ARRAY_BUFFER, vbo );
			glBufferData( GL_ARRAY_BUFFER, sizeof(float)*12, vdata, GL_STATIC_DRAW );

			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
			glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*3, idata, GL_STATIC_DRAW );

			glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, 0 );
			glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)(sizeof(float)*2) );

			Input input;

			bool running = true;
			while( running )
			{
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

				// update

				// render
				glClearColor( 0.0f, 0.0f, 1.0f, 0.0f );
				glClear( GL_COLOR_BUFFER_BIT );

				shader.bind();
				texture.bind();

				shader.setMat4( viewMatrixLocation, camera.getViewMatrix() );
				shader.setMat4( projectionMatrixLocation, camera.getProjectionMatrix() );
				shader.setInt( diffuseMapLocation, 0 );

				glBindVertexArray( vao );
				glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0 );

				SDL_GL_SwapWindow( window );
			}

			SDL_GL_DeleteContext( context );
		}
		SDL_DestroyWindow( window );
	}

	return 0;
}