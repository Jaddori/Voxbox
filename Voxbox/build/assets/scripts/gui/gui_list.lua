require( "./assets/scripts/gui/gui_base" )

GUI_LIST_SLIDER_WIDTH = 8
GUI_LIST_SLIDER_HEIGHT = 24

GuiList = {}
GuiList.__index = GuiList

function GuiList:create( position, size )
	if position then position = vec2( position[1], position[2] )
	else position = vec2() end
	
	if size then size = vec2( size[1], size[2] )
	else size = vec2() end
	
	local list = GuiBase:create()
	setmetatable( list, GuiList )
	
	list.position = position
	list.size = size
	list.excessItems = 0
	list.itemOffset = 0
	list.items = {}
	
	local right = list.position[1] + list.size[1]
	
	list.slider = 
	{
		position = vec2( right-GUI_LIST_SLIDER_WIDTH, list.position[2] ),
		size = vec2( GUI_LIST_SLIDER_WIDTH, GUI_LIST_SLIDER_HEIGHT ),
		pressed = false,
		offset = vec2(),
	}
	
	return list
end

function GuiList:addItem( item )
	item.size[1] = self.size[1] - GUI_LIST_SLIDER_WIDTH
	table.insert( self.items, item )
	
	-- calculate out the number of items outside the bounds of the list
	self.excessItems = 0
	local yoffset = 0
	for _,v in pairs(self.items) do
		if yoffset >= self.size[2] then
			self.excessItems = self.excessItems + 1
		else
			yoffset = yoffset + v:getHeight()
		end
	end
	
	print( "Excess items: " .. tostring(self.excessItems) )
end

function GuiList:update( dt )
	local mpos = camera.mousePosition

	-- update the slider
	if Input.buttonPressed( Buttons.Left ) then
		if inside( self.slider.position, self.slider.size, mpos ) then
			self.slider.pressed = true
			self.slider.offset = mpos - self.slider.position
		end
	elseif Input.buttonReleased( Buttons.Left ) then
		self.slider.pressed = false
	end
	
	if self.slider.pressed then
		self.slider.position[2] = mpos[2] - self.slider.offset[2]
		
		-- clamp the sliders vertical position to the bounds of the list
		if self.slider.position[2] < self.position[2] then
			self.slider.position[2] = self.position[2]
		elseif self.slider.position[2] + self.slider.size[2] > self.position[2] + self.size[2] then
			self.slider.position[2] = self.position[2] + self.size[2] - self.slider.size[2]
		end
		
		-- calculate item offset from slider
		local sliderTop = self.slider.position[2] - self.position[2]
		local sliderFraction = sliderTop / ( self.size[2] - self.slider.size[2] )
		
		self.itemOffset = math.floor( ( self.excessItems * sliderFraction ) + 0.5 )
	end
	
	-- update items
	local yoffset = 0
	local pos = vec2( self.position[1], self.position[2] )
	for i=self.itemOffset+1, #self.items do
		pos[2] = self.position[2] + yoffset
		self.items[i]:update( dt, pos )
		yoffset = yoffset + self.items[i]:getHeight()
	end
end

function GuiList:render()
	if self.visible then
		-- render background 
		Graphics.queueQuad( self.position, self.size, GUI_BASE_UV_PRESS, 1.0, self.background )
		
		-- render items
		local yoffset = 0
		local pos = vec2( self.position[1], self.position[2] )
		for i=self.itemOffset+1, #self.items do
			pos[2] = self.position[2] + yoffset
			self.items[i]:render( pos )
			
			yoffset = yoffset + self.items[i]:getHeight()
			
			if yoffset >= self.size[2] then break end
		end
		
		-- render slider
		Graphics.queueQuad( self.slider.position, self.slider.size, {0,0,0,0}, 1.0 )
	end
end