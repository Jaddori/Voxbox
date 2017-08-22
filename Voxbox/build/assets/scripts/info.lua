local INFO_WIDTH = 128
local INFO_HEIGHT = 256
local INFO_POLL_FREQUENCY = 1.0
local INFO_TEXT_COLOR = { 1, 1, 1, 1 }

info = {}

function info:load()
	self.font = Assets.loadFont( "./assets/fonts/verdana12.bin", "./assets/fonts/verdana12.dds" )
	self.bounds = { WINDOW_WIDTH-INFO_WIDTH, 0, INFO_WIDTH, INFO_HEIGHT }
	self.cores = 0
	self.threads = 0
	self.ram = 0
	self.luaRam = 0
	self.vsync = true
	self.cameraPosition = vec3()
	self.cameraDirection = vec3()
	
	self.coreText = ""
	self.threadText = ""
	self.ramText = ""
	self.luaRamText = ""
	self.vsyncText = ""
	self.cameraLabel = "Camera:"
	self.cameraPositionText = ""
	self.cameraDirectionText = ""
	
	local yoffset = 0.0
	self.corePosition = { self.bounds[1], self.bounds[2] + yoffset }
	
	yoffset = yoffset + self.font.height
	self.threadPosition = { self.bounds[1], self.bounds[2] + yoffset }
	
	yoffset = yoffset + self.font.height
	self.ramPosition = { self.bounds[1], self.bounds[2] + yoffset }
	
	yoffset = yoffset + self.font.height
	self.luaRamPosition = { self.bounds[1], self.bounds[2] + yoffset }
	
	yoffset = yoffset + self.font.height
	self.vsyncPosition = { self.bounds[1], self.bounds[2] + yoffset }
	
	yoffset = yoffset + self.font.height
	self.cameraLabelPosition = { self.bounds[1], self.bounds[2] + yoffset }
	
	yoffset = yoffset + self.font.height
	self.cameraPositionPosition = { self.bounds[1], self.bounds[2] + yoffset }
	
	yoffset = yoffset + self.font.height
	self.cameraDirectionPosition = { self.bounds[1], self.bounds[2] + yoffset }
	
	self.elapsedTime = INFO_POLL_FREQUENCY
end

function info:unload()
end

function info:update( dt )
	self.elapsedTime = self.elapsedTime + dt
	if self.elapsedTime > INFO_POLL_FREQUENCY then
		self.elapsedTime = 0
		
		SystemInfo.poll()
		self.cores = SystemInfo.getCores()
		self.threads = SystemInfo.getThreads()
		self.ram = SystemInfo.getRam()
		self.luaRam = collectgarbage("count") * UNIT_KILOBYTES
		self.vsync = SystemInfo.getVsync()
		
		self.coreText = tostring( self.cores ) .. " available cores"
		self.threadText = tostring( self.threads ) .. " running threads"
		
		local shrunkenBytes, newUnit = Core.shrinkBytes( self.ram )
		self.ramText = "RAM: " .. byteString( math.floor(shrunkenBytes), newUnit )
		
		shrunkenBytes, newUnit = Core.shrinkBytes( self.luaRam )
		self.luaRamText = "Lua RAM: " .. byteString( math.floor(shrunkenBytes), newUnit )
		
		if self.vsync then
			self.vsyncText = "VSYNC enabled"
		else
			self.vsyncText = "VSYNC disabled"
		end
		
		-- get camera information
		Camera.getPosition( self.cameraPosition )
		Camera.getDirection( self.cameraDirection )
		
		self.cameraPositionText = self.cameraPosition:tostring()
		self.cameraDirectionText = self.cameraDirection:tostring()
	end
end

function info:render()
	Graphics.queueText( self.font, self.coreText, self.corePosition, INFO_TEXT_COLOR )
	Graphics.queueText( self.font, self.threadText, self.threadPosition, INFO_TEXT_COLOR )
	Graphics.queueText( self.font, self.ramText, self.ramPosition, INFO_TEXT_COLOR )
	Graphics.queueText( self.font, self.luaRamText, self.luaRamPosition, INFO_TEXT_COLOR )
	Graphics.queueText( self.font, self.vsyncText, self.vsyncPosition, INFO_TEXT_COLOR )
	Graphics.queueText( self.font, self.cameraLabel, self.cameraLabelPosition, INFO_TEXT_COLOR )
	Graphics.queueText( self.font, self.cameraPositionText, self.cameraPositionPosition, INFO_TEXT_COLOR )
	Graphics.queueText( self.font, self.cameraDirectionText, self.cameraDirectionPosition, INFO_TEXT_COLOR )
end