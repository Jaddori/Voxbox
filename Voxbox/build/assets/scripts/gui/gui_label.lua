require( "./assets/scripts/gui/gui_base" )

GuiLabel = {}
GuiLabel.__index = GuiLabel
setmetatable( GuiLabel, GuiBase )

function GuiLabel:create( position, size, text )
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
	
	text = text or "[GuiLabel]"
	
	local label = GuiBase:create()
	setmetatable( label, GuiLabel )
	
	label.position = position
	label.size = size
	label.text = text
	label:alignText()
	
	return label
end

function GuiLabel:onClick()
	Log.log( VERBOSITY_DEBUG, "GuiLabel clicked." )
end

function GuiLabel:render( position )
	position = position or self.position
	local textPosition = position + self.textOffset

	if self.visible then
		Graphics.queueText( self.font, self.text, textPosition, self.textColor )
	end
end