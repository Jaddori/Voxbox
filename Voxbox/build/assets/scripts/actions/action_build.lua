BuildAction =
{
	previewBlock = vec3(),
	previewValid = false,
	selectionStart = vec3(),
	selectionEnd = vec3(),
	selectionBounds = {0,0,0,0,0,0},
	selectionValid = false,
}

function BuildAction:update( dt, selectedWorker )
	if not selectedWorker then return end
	
	-- get preview block
	if World.hitBlock( camera.rayStart, camera.rayEnd, self.previewBlock ) then
		World.localToWorld( self.previewBlock, self.previewBlock )
		
		self.previewBlock[2] = self.previewBlock[2] + 1
		self.previewValid = true
	else
		self.previewValid = false
	end
	
	-- start selection
	if Input.buttonPressed( Buttons.Left ) then
		if self.previewValid then
			self.previewBlock:copy( self.selectionStart )
			self.selectionValid = true
		else
			self.selectionValid = false
		end
	end
	
	-- update selection
	if Input.buttonDown( Buttons.Left ) then
		if self.previewValid then
			self.previewBlock:copy( self.selectionEnd )
			makeCube( self.selectionStart, self.selectionEnd, self.selectionBounds )
		end
	end
	
	if self.selectionValid then
		local selectionHeight = self.selectionBounds[5] - self.selectionBounds[2]
		
		-- size selection
		local mwheel = Input.getMouseWheel()
		if mwheel < 0 and selectionHeight > 0 then
			self.selectionBounds[5] = self.selectionBounds[5] - 1
		elseif mwheel > 0 then
			self.selectionBounds[5] = self.selectionBounds[5] + 1
		end
		
		-- execute action
		if Input.keyReleased( Keys.Space ) then
			selectedWorker:build( self.selectionBounds )
		end
	end
end

function BuildAction:render()
	-- render preview block
	if self.previewValid then
		Graphics.queueBlock( self.previewBlock, {1,0,0,0.65} )
	end
	
	-- render selection
	if self.selectionValid then
		for x=self.selectionBounds[1], self.selectionBounds[4] do
			for y=self.selectionBounds[2], self.selectionBounds[5] do
				for z=self.selectionBounds[3], self.selectionBounds[6] do
					Graphics.queueBlock( {x,y,z}, {1,0,0,0.5} )
				end
			end
		end
	end
end