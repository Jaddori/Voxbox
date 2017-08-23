require( "./assets/scripts/gui_base" )

GuiButton = GuiBase:create()
GuiButton.text = "[GuiButton]"

function GuiButton:create( position, size )
	position = position or vec2(0,0)
	size = size or vec2(128,24)
	
	local button = {}
	setmetatable( button, self )
	self.__index = self
	
	button.position = position
	button.size = size
	
	return button
end

function GuiButton:onClick()
	Log.log( VERBOSITY_DEBUG, "GuiButton clicked." )
end