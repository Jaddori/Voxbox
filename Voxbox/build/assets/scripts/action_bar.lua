require( "./assets/scripts/gui/gui" )

ACTION_SELECT = 0
ACTION_DIG = 1
ACTION_BUILD = 2

ACTION_BAR_HEIGHT = 256

actionBar = {}

function actionBar:load()
	self.guiElements = {}
	self.action = ACTION_SELECT
	self.position = vec2( 0, WINDOW_HEIGHT - ACTION_BAR_HEIGHT )
	self.size = vec2( WINDOW_WIDTH, ACTION_BAR_HEIGHT )
	self.background = Assets.loadTexture( "./assets/textures/gui_base_background.dds" )
	
	local pos = { 64, WINDOW_HEIGHT - ACTION_BAR_HEIGHT + 8 }
	
	-- add select button
	self.guiElements.selectButton = GuiButton:create( pos, {32,32} )
	self.guiElements.selectButton.onClick = function( button )
		actionBar.action = ACTION_SELECT
		Log.log( VERBOSITY_DEBUG, "Select action selected." )
	end
	
	-- add dig button
	pos[1] = pos[1] + 32 + 8
	self.guiElements.digButton = GuiButton:create( pos, {32,32} )
	self.guiElements.digButton.onClick = function( button ) actionBar.action = ACTION_DIG end
	
	-- add build button
	pos[1] = pos[1] + 32 + 8
	self.guiElements.buildButton = GuiButton:create( pos, {32,32} )
	self.guiElements.buildButton.onClick = function( button ) actionBar.action = ACTION_BUILD end
end

function actionBar:unload()
	Assets.unloadTexture( self.background )
	
	-- unload gui elements
	for _,v in pairs(self.guiElements) do
		v:unload()
	end
end

function actionBar:update( dt )
	-- update gui elements
	for _,v in pairs(self.guiElements) do
		v:update( dt )
	end
end

function actionBar:render()
	-- render background
	Graphics.queueQuad( self.position, self.size, {0,0,1,1}, 1.0, self.background )
	
	-- render gui elements
	for _,v in pairs(self.guiElements) do
		v:render()
	end
end