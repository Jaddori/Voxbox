camera = 
{
	mousePosition = {0,0},
	mouseDelta = {0,0},
}

function camera:update( dt )
	-- get mouse position and mouse delta
	Input.getMousePosition( self.mousePosition )
	Input.getMouseDelta( self.mouseDelta )
	
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
		Camera.updatePosition( movement )
	end
end