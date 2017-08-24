require( "./assets/scripts/gamestates/gamestate_base" )

Pause = {}

function Pause:load()
	self.backgroundPosition = vec2(0,0)
	self.backgroundSize = vec2(WINDOW_WIDTH, WINDOW_HEIGHT)
	self.backgroundOpacity = 0.75

	self.title = GuiLabel:create( {0, 64}, {WINDOW_WIDTH,32}, "Paused" )
	self.title.font = Assets.loadFont( "./assets/fonts/verdana18.bin", "./assets/fonts/verdana18.dds" )
	self.title:setTextAlignment( GUI_TEXT_ALIGNMENT_CENTER, GUI_TEXT_ALIGNMENT_CENTER )
	
	local screenCenterX = WINDOW_WIDTH*0.5
	self.resumeButton = GuiButton:create( {screenCenterX-64, 256}, {128,32}, "Resume" )
	self.quitButton = GuiButton:create( {screenCenterX-64, 256+32+8}, {128,32}, "Quit" )
	
	self.resumeButton.onClick = function( button ) Gamestate:pop() end
	self.quitButton.onClick = function( button )
		Gamestate:clear()
		Gamestate:push( MainMenu )
	end
end

function Pause:unload()
	self.title:unload()
	self.resumeButton:unload()
	self.quitButton:unload()
end

function Pause:update( dt )
	self.resumeButton:update( dt )
	self.quitButton:update( dt )
end

function Pause:render()
	Gamestate.states[Gamestate.current-1]:render()
	
	Graphics.queueQuad( self.backgroundPosition, self.backgroundSize, {0,0,1,1}, self.backgroundOpacity )

	self.title:render()
	self.resumeButton:render()
	self.quitButton:render()
end