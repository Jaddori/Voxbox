require( "./assets/scripts/gui_base" )

GuiLabel = GuiBase:create()
GuiLabel.text = "[GuiLabel]"

function GuiLabel:create( position )
	position = position or vec2(0,0)
	
	local label = {}
	setmetatable( label, self )
	self.__index = self
	
	label.position = position
	
	return label
end

function GuiLabel:onClick()
	Log.log( VERBOSITY_DEBUG, "GuiLabel clicked." )
end

function GuiLabel:render()
	if self.visible then
		Graphics.queueText( self.font, self.text, self.position, self.textColor )
	end
end