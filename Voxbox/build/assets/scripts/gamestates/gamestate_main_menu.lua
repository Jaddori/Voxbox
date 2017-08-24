require( "./assets/scripts/gamestates/gamestate_base" )
require( "./assets/scripts/gui/gui" )

MainMenu = {}

function MainMenu:load()
	self.backgroundTexture = Assets.loadTexture( "./assets/textures/gui_base_background.dds" )
	self.backgroundPosition = vec2(0,0)
	self.backgroundSize = vec2(WINDOW_WIDTH, WINDOW_HEIGHT)

	self.title = GuiLabel:create( {0,64}, {WINDOW_WIDTH, 32}, "Main Menu" )
	self.title.font = Assets.loadFont( "./assets/fonts/verdana18.bin", "./assets/fonts/verdana18.dds" )
	self.title:setTextAlignment( GUI_TEXT_ALIGNMENT_CENTER, GUI_TEXT_ALIGNMENT_CENTER )
	
	local screenCenterX = WINDOW_WIDTH*0.5
	self.playButton = GuiButton:create( {screenCenterX-64, 256}, {128,32}, "Play" )
	self.quitButton = GuiButton:create( {screenCenterX-64, 256+32+8}, {128,32}, "Quit" )
	
	self.playButton.onClick = function( button )
		Gamestate:clear()
		Gamestate:push( Gameplay )
	end
	self.quitButton.onClick = function( button ) Core.quit() end
end

function MainMenu:unload()
end

function MainMenu:update( dt )
	self.playButton:update( dt )
	self.quitButton:update( dt )
end

function MainMenu:render()
	Graphics.queueQuad( self.backgroundPosition, self.backgroundSize, {0,0,1,1}, 1.0, self.backgroundTexture )
	
	self.title:render()
	self.playButton:render()
	self.quitButton:render()
end