local INFO_WIDTH = 128
local INFO_HEIGHT = 256
local INFO_POLL_FREQUENCY = 1.0
local INFO_TEXT_COLOR = { 1, 1, 1, 1 }

info = {}

function info:load()
	info.font = Assets.loadFont( "./assets/fonts/verdana12.bin", "./assets/fonts/verdana12.dds" )
	info.bounds = { WINDOW_WIDTH-INFO_WIDTH, 0, INFO_WIDTH, INFO_HEIGHT }
	info.cores = 0
	info.threads = 0
	info.ram = 0
	info.vsync = true
	
	info.coreText = ""
	info.threadText = ""
	info.ramText = ""
	info.vsyncText = ""
	
	local yoffset = 0.0
	info.corePosition = { info.bounds[1], info.bounds[2] + yoffset }
	
	yoffset = yoffset + info.font.height
	info.threadPosition = { info.bounds[1], info.bounds[2] + yoffset }
	
	yoffset = yoffset + info.font.height
	info.ramPosition = { info.bounds[1], info.bounds[2] + yoffset }
	
	yoffset = yoffset + info.font.height
	info.vsyncPosition = { info.bounds[1], info.bounds[2] + yoffset }
	
	info.elapsedTime = 0
end

function info:unload()
end

function info:update()
	info.elapsedTime = info.elapsedTime + 0.01
	if info.elapsedTime > INFO_POLL_FREQUENCY then
		info.elapsedTime = 0
		
		SystemInfo.poll()
		info.cores = SystemInfo.getCores()
		info.threads = SystemInfo.getThreads()
		info.ram = SystemInfo.getRam()
		info.vsync = SystemInfo.getVsync()
		
		info.coreText = tostring( info.cores ) .. " available cores"
		info.threadText = tostring( info.threads ) .. " running threads"
		info.ramText = tostring( info.ram ) .. " b ram"
		
		if info.vsync then
			info.vsyncText = "VSYNC enabled"
		else
			info.vsyncText = "VSYNC disabled"
		end
	end
end

function info:render()
	Graphics.queueText( self.font, self.coreText, self.corePosition, INFO_TEXT_COLOR )
	Graphics.queueText( self.font, self.threadText, self.threadPosition, INFO_TEXT_COLOR )
	Graphics.queueText( self.font, self.ramText, self.ramPosition, INFO_TEXT_COLOR )
	Graphics.queueText( self.font, self.vsyncText, self.vsyncPosition, INFO_TEXT_COLOR )
end