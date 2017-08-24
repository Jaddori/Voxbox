require( "./assets/scripts/utils" )

GUI_BASE_UV_IDLE = {0,0,1,0.25}
GUI_BASE_UV_HOVER = {0,0.25,1,0.5}
GUI_BASE_UV_PRESS = {0,0.5,1,0.75}
GUI_BASE_UV_DISABLED = {0,0.75,1,1}

GUI_TEXT_ALIGNMENT_NEAR = 0
GUI_TEXT_ALIGNMENT_CENTER = 1
GUI_TEXT_ALIGNMENT_FAR = 2

GuiBase = {}
GuiBase.__index = GuiBase

function GuiBase:create()
	local newGuiBase =
	{
		position = vec2(),
		size = vec2(),
		
		font = Assets.loadFont( "./assets/fonts/verdana12.bin", "./assets/fonts/verdana12.dds" ),
		text = "[GuiBase]",
		textColor = {1,1,1,1},
		textAlignmentX = GUI_TEXT_ALIGNMENT_NEAR,
		textAlignmentY = GUI_TEXT_ALIGNMENT_NEAR,
		textOffset = vec2(),
		textPadding = vec2(2,2),
		
		background = Assets.loadTexture( "./assets/textures/gui_base_background.dds" ),
		backgroundOpacity = 1.0,
		
		uv = GUI_BASE_UV_IDLE,
		
		visible = true,
		enabled = true,
		focusd = false,
		hovered = false,
		pressed = false,
	}
	
	setmetatable( newGuiBase, GuiBase )
	return newGuiBase
end

function GuiBase:unload()
	Assets.unloadFont( self.font )
	Assets.unloadTexture( self.background )
end

function GuiBase:onEnter()
end

function GuiBase:onLeave()
end

function GuiBase:onHover()
end

function GuiBase:onPress()
end

function GuiBase:onClick()
end

function GuiBase:alignText()
	local textBounds = self.font:measureText( self.text )
	
	--print(self.textAlignmentX)

	-- align text horizontally
	if self.textAlignmentX == GUI_TEXT_ALIGNMENT_NEAR then
		self.textOffset[1] = self.textPadding[1]
	elseif self.textAlignmentX == GUI_TEXT_ALIGNMENT_CENTER then
		self.textOffset[1] = ( self.size[1] * 0.5 ) - ( textBounds[1] * 0.5 )
	else -- GUI_TEXT_ALIGNMENT_FAR
		self.textOffset[1] = self.size[1] - textBounds[1] - self.textPadding[1]
	end
	
	-- align text vertically
	if self.textAlignmentY == GUI_TEXT_ALIGNMENT_NEAR then
		self.textOffset[2] = self.textPadding[2]
	elseif self.textAlignmentY == GUI_TEXT_ALIGNMENT_CENTER then
		self.textOffset[2] = ( self.size[2] * 0.5 ) - ( textBounds[2] * 0.5 )
	else -- GUI_TEXT_ALIGNMENT_FAR
		self.textOffset[2] = self.size[2] - textBounds[2] - self.textPadding[2]
	end
end

function GuiBase:setText( text )
	self.text = text
	self:alignText()
end

function GuiBase:setTextAlignment( xalign, yalign )
	self.textAlignmentX = xalign
	self.textAlignmentY = yalign
	
	self:alignText()
end

function GuiBase:setTextPadding( padding )
	self.textPadding[1] = padding[1]
	self.textPadding[2] = padding[2]
	
	self:alignText()
end

function GuiBase:getHeight()
	return self.size[2]
end

function GuiBase:update( dt, position )
	position = position or self.position

	if self.enabled then
		local mpos = {0,0}
		Input.getMousePosition( mpos )
		
		local prevHovered = self.hovered
		local prevPressed = self.pressed
		
		self.hovered = inside( position, self.size, mpos )
		self.pressed = self.hovered and Input.buttonDown( Buttons.Left )
		
		self.uv = GUI_BASE_UV_IDLE
		
		if self.hovered then
			if not prevHovered then
				self:onEnter()
			end
			
			self:onHover()
			self.uv = GUI_BASE_UV_HOVER
			
			if self.pressed then
				self:onPress()
				self.uv = GUI_BASE_UV_PRESS
			elseif prevPressed then
				self:onClick()
			end
		else
			if prevHovered then
				self:onLeave()
			end
		end
		
		-- check if we have focus
		if Input.buttonReleased( Buttons.Left ) then
			self.focused = self.hovered
		end
	else
		self.uv = GUI_BASE_UV_DISABLED
	end
end

function GuiBase:render( position )
	position = position or self.position
	local textPosition = position + self.textOffset

	if self.visible then
		Graphics.queueQuad( position, self.size, self.uv, self.backgroundOpacity, self.background )
		Graphics.queueText( self.font, self.text, textPosition, self.textColor )
	end
end