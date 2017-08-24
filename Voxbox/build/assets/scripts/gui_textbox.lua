require( "./assets/scripts/gui_base" )

local GUI_TEXTBOX_CARET_FLASH_TIME = 0.5

GuiTextbox = GuiBase:create()
GuiTextbox.textInput = ""
GuiTextbox.caretVisible = false
GuiTextbox.caretElapsed = 0.0

function GuiTextbox:create( position, size )
	position = position or vec2(0,0)
	size = size or vec2(0,0)
	
	local textbox = {}
	setmetatable( textbox, self )
	self.__index = self
	
	textbox.position = position
	textbox.size = size
	
	return textbox
end

function GuiTextbox:onConfirm()
end

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
		self.textInput = self.textInput .. Input.textInput()
		
		-- check for backspace
		if Input.keyRepeated( Keys.Backspace ) and self.textInput:len() > 0 then
			self.textInput = self.textInput:sub(0, self.textInput:len()-1)
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
		
		local text = self.textInput
		if self.focused and self.caretVisible then
			text = text .. "_"
		end
		
		Graphics.queueText( self.font, text, self.position, self.textColor )
	end
end