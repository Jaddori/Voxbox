require( "./assets/scripts/gui_base" )

local GUI_TEXTBOX_CARET_FLASH_TIME = 0.5

GuiTextbox = GuiBase:create()
GuiTextbox.__index = GuiTextbox
setmetatable( GuiTextbox, GuiBase )

function GuiTextbox:create( position, size )
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
	
	local textbox = GuiBase:create()
	setmetatable( textbox, GuiTextbox )
	
	textbox.position = position
	textbox.size = size
	textbox.textInput = ""
	textbox.caretVisible = false
	textbox.caretElapsed = 0.0
	textbox:setText( textbox.textInput )
	
	return textbox
end

function GuiTextbox:onConfirm()
end

GPADDING = 0
function GuiTextbox:update( dt )
	GuiBase.update( self, dt )

	-- update caret flashing
	self.caretElapsed = self.caretElapsed + dt
	if self.caretElapsed >= GUI_TEXTBOX_CARET_FLASH_TIME then
		self.caretElapsed = 0.0
		self.caretVisible = not self.caretVisible
	end
	
	if self.focused then
		-- update text input
		local prevLen = self.textInput:len()
		self.textInput = self.textInput .. Input.textInput()
		
		-- check for backspace
		if Input.keyRepeated( Keys.Backspace ) and self.textInput:len() > 0 then
			self.textInput = self.textInput:sub(0, self.textInput:len()-1)
		end
		
		if self.textInput:len() ~= prevLen then
			self:setText( self.textInput )
		end
		
		-- check for enter
		if Input.keyReleased( Keys.Enter ) then
			self:onConfirm()
		end
	end
end

function GuiTextbox:render()
	if self.visible then
		Graphics.queueQuad( self.position, self.size, GUI_BASE_UV_PRESS, self.backgroundOpacity, self.background )
		
		local text = self.text
		if self.focused and self.caretVisible then
			text = text .. "_"
		end
		
		Graphics.queueText( self.font, text, self.textPosition, self.textColor )
	end
end