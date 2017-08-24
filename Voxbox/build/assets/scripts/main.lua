require( "./assets/scripts/console" )
require( "./assets/scripts/info" )
require( "./assets/scripts/camera" )
require( "./assets/scripts/gamestates/gamestate_gameplay" )
require( "./assets/scripts/gamestates/gamestate_main_menu" )
require( "./assets/scripts/gamestates/gamestate_pause" )

function mainLoad()
	console:load()
	info:load()
	camera:load()
	
	-- TEMP
	MainMenu:load()
	Gameplay:load()
	Pause:load()

	Gamestate:push( MainMenu )
end

function mainUnload()
	console:unload()
	info:unload()
	
	-- TEMP
	MainMenu:unload()
	Gameplay:unload()
	Pause:unload()
end

function mainUpdate( dt )
	console:update( dt )
	info:update( dt )

	Gamestate:update( dt )
end

function mainRender()
	console:render()
	info:render()

	Gamestate:render()
end