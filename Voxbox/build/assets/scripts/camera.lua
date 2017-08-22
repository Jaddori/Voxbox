camera = 
{
	mousePosition = vec2(),
	mouseDelta = vec2(),
	rayStart = vec3(),
	rayEnd = vec3(),
	speed = 0.1,
	borderWidth = 128,			-- width of the virtual border along the screen edges
	height = 55,				-- height above the ground
	angle = -0.7,				-- angle of the camera facing the ground
	rotation = 0.7,
	debugMode = false
}

function camera:load()
	Camera.setPosition( {0,self.height,0} )
	Camera.setDirection( { 0, self.angle, self.rotation } )
end

function camera:update( dt )
	-- get mouse position and mouse delta
	Input.getMousePosition( self.mousePosition )
	Input.getMouseDelta( self.mouseDelta )
	
	-- project mouse cursor into 3D space
	Camera.unproject( self.mousePosition, 0.0, self.rayStart )
	Camera.unproject( self.mousePosition, 1.0, self.rayEnd )

	if self.debugMode then
		-- check for direction change
		if Input.buttonDown( Buttons.Left ) then
			if math.abs( self.mouseDelta[1] ) + math.abs( self.mouseDelta[2] ) > 0.0 then
				Camera.updateDirection( self.mouseDelta )
			end
		end
		
		-- check for movement
		local movement = {0,0,0}
		if Input.keyDown( "W" ) then movement[3] = movement[3] + 1 end
		if Input.keyDown( "S" ) then movement[3] = movement[3] - 1 end
		if Input.keyDown( "D" ) then movement[1] = movement[1] + 1 end
		if Input.keyDown( "A" ) then movement[1] = movement[1] - 1 end
		
		if math.abs( movement[1] ) + math.abs( movement[2] ) + math.abs( movement[3] ) > 0.0 then
			Camera.relativeMovement( movement )
		end
	else -- normal RTS camera
		local movement = vec3()
	
		-- check if the user is scrolling the camera to the left
		if self.mousePosition[1] > 0 and self.mousePosition[1] < self.borderWidth then
			movement[1] = movement[1] + self.speed
		elseif self.mousePosition[1] < WINDOW_WIDTH and self.mousePosition[1] > WINDOW_WIDTH - self.borderWidth then -- to the right
			movement[1] = movement[1] - self.speed
		end
		
		-- up
		if self.mousePosition[2] > 0 and self.mousePosition[2] < self.borderWidth then
			movement[3] = movement[3] + self.speed
		elseif self.mousePosition[2] < WINDOW_HEIGHT and self.mousePosition[2] > WINDOW_HEIGHT - self.borderWidth then -- down
			movement[3] = movement[3] - self.speed
		end
		
		if movement:length() > 0.0 then
			Camera.absoluteMovement( movement )
		end
	end
end