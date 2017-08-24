require( "./assets/scripts/utils" )

GUI_BASE_UV_IDLE = {0,0,1,0.25}
GUI_BASE_UV_HOVER = {0,0.25,1,0.5}
GUI_BASE_UV_PRESS = {0,0.5,1,0.75}
GUI_BASE_UV_DISABLED = {0,0.75,1,1}

GuiBase =
{
	position = vec2(),
	size = vec2(),
	
	font = Assets.loadFont( "./assets/fonts/verdana12.bin", "./assets/fonts/verdana12.dds" ),
	text = "[GuiBase]",
	textColor = {1,1,1,1},
	
	background = Assets.loadTexture( "./assets/textures/gui_base_background.dds" ),
	backgroundOpacity = 1.0,
	
	uv = GUI_BASE_UV_IDLE,
	
	visible = true,
	enabled = true,
	focusd = false,
	hovered = false,
	pressed = false,
}

function GuiBase:create()
	local newGuiBase = {}
	setmetatable( newGuiBase, self )
	self.__index = self
	return newGuiBase
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

function GuiBase:update( dt )
	if self.enabled then
		local mpos = camera.mousePosition
		local prevHovered = self.hovered
		local prevPressed = self.pressed
		
		self.hovered = inside( self.position, self.size, mpos )
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

function GuiBase:render()
	if self.visible then
		Graphics.queueQuad( self.position, self.size, self.uv, self.backgroundOpacity, self.background )
		Graphics.queueText( self.font, self.text, self.position, self.textColor )
	end
end