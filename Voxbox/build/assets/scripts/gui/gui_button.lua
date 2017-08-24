require( "./assets/scripts/gui/gui_base" )

GuiButton = {}
GuiButton.__index = GuiButton
setmetatable( GuiButton, GuiBase )

function GuiButton:create( position, size, text )
	if position then
		position = vec2( position[1], position[2] )
	else
		position = vec2()
	end
	
	if size then
		size = vec2( size[1], size[2] )
	else
		size = vec2()
	end
	
	text = text or ""
	
	local button = GuiBase:create()
	setmetatable( button, GuiButton )
	
	button.position = position
	button.size = size
	button.text = text
	button:alignText()
	
	return button
end

function GuiButton:onClick()
	Log.log( VERBOSITY_DEBUG, "GuiButton clicked." )
end