require( "./assets/scripts/gamestates/gamestate_base" )

MainMenu = {}

function MainMenu:load()
end

function MainMenu:unload()
end

function MainMenu:update( dt )
	if Input.keyReleased( "F" ) then
		Gamestate:push( Gameplay )
	end
end

function MainMenu:render()
	Graphics.queueQuad( vec2( 128, 32 ), vec2( 32, 32 ), {0,0,0,0}, 1.0 )
end