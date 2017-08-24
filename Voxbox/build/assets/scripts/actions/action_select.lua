SelectAction =
{
	startPoint = vec2(),
	endPoint = vec2(),
	min = vec2(),
	max = vec2(),
	size = vec2(),
	valid = false,
	worker = {},
	workers = {},
}

function SelectAction:update( dt )
	-- get a startPoint point
	if Input.buttonPressed( Buttons.Left ) then
		camera.mousePosition:copy( self.startPoint )
	end
	
	-- get an end point
	if Input.buttonDown( Buttons.Left ) then
		camera.mousePosition:copy( self.endPoint )
		
		self.min[1] = math.min( self.startPoint[1], self.endPoint[1] )
		self.min[2] = math.min( self.startPoint[2], self.endPoint[2] )
		
		self.max[1] = math.max( self.startPoint[1], self.endPoint[1] )
		self.max[2] = math.max( self.startPoint[2], self.endPoint[2] )
		
		self.size = self.max - self.min
		self.valid = true
	else
		self.valid = false
	end
	
	-- select unit
	if Input.buttonReleased( Buttons.Left ) then
		for i=1, #self.workers do
			local worldPosition = vec3()
			self.workers[i].position:copy( worldPosition )
			
			local windowPosition = {0,0}
			Camera.project( worldPosition, windowPosition )
			
			if inside( self.startPoint, self.size, windowPosition ) then
				self.worker = self.workers[i]
				self.workers[i].selected = true
			else
				self.workers[i].selected = false
			end
		end
	end
end

function SelectAction:render()
	if self.valid then
		Graphics.queueQuad( self.min, self.size, {0,0,0,0}, 0.85 )
	end
end