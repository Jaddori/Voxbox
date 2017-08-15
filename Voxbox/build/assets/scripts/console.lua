local CONSOLE_COLORS =
{
	{1,1,1,1},			-- information	(white)
	{1,0.45,0,1},		-- warning		(orange)
	{1,0,0,1},			-- error 		(red)
	{0.65,0.65,1,1}		-- debug		(blue)
}

local CONSOLE_WIDTH = WINDOW_WIDTH
local CONSOLE_HEIGHT = 256
local CONSOLE_OPACITY = 0.75
local CONSOLE_TEXT_OFFSET = 8
local CONSOLE_FLASH_TIME = 5
local CONSOLE_INPUT_OFFSET = 2
local CONSOLE_INPUT_HEIGHT = 24

console = {}

function console:load()
	self.visible = false
	self.font = Assets.loadFont( "./assets/fonts/verdana12.bin", "./assets/fonts/verdana12.dds" )
	self.position = {0,0}
	self.size = {CONSOLE_WIDTH,CONSOLE_HEIGHT}
	self.uv = {0,0,0,0}
	self.opacity = CONSOLE_OPACITY
	self.flashTime = 0.0
	self.threshold = 0
	self.messages = {}
	self.verbosities = {}
	self.prevMessageCount = 0
	
	self.input = {}
	self.input.position = {0,CONSOLE_HEIGHT+CONSOLE_INPUT_OFFSET}
	self.input.size = {CONSOLE_WIDTH,CONSOLE_INPUT_HEIGHT}
	self.input.text = ""
end

function console:unload()
end

function console:update()
	-- get messages from log
	self.threshold = Log.getThreshold()
	Log.getMessages( self.messages, self.verbosities, self.prevMessageCount )
	
	-- check if we need to flash new messages
	if self.visible then
		self.prevMessageCount = #self.messages
	else
		if #self.messages > self.prevMessageCount then
			for i=self.prevMessageCount+1, #self.messages do
				if self.verbosities[i] >= self.threshold then
					self.flashTime = CONSOLE_FLASH_TIME
					break
				end
			end
			
			self.prevMessageCount = #self.messages
		end
	end

	-- check if user toggled the console
	if Input.keyReleased( Keys.Console ) then
		self.visible = not self.visible
	end
	
	-- update the console if it is visible
	if self.visible then
		-- get text input
		self.input.text = self.input.text .. Input.textInput()
		
		-- get backspace
		if Input.keyRepeated( Keys.Backspace ) and self.input.text:len() > 0 then
			self.input.text = self.input.text:sub( 0, self.input.text:len()-1 )
		end
		
		-- get enter
		if Input.keyReleased( Keys.Enter ) then
			if self.input.text:len() > 0 then
				log( VERBOSITY_DEBUG, self.input.text )
		
				-- execute entered command
				local chunk, errorMessage = load( self.input.text )
				if errorMessage then
					log( VERBOSITY_ERROR, errorMessage )
				else
					local success, errorMessage = pcall(chunk)
					if not success then
						log( VERBOSITY_ERROR, errorMessage )
					end
				end
				
				self.input.text = ""
			end
		end
	end
	
	if self.flashTime > 0.0 then
		self.flashTime = self.flashTime - 0.01
	end
end

function console:render()
	if self.visible or self.flashTime > 0.0 then
		-- render message box
		Graphics.queueQuad( self.position, self.size, self.uv, self.opacity )
		
		-- render messages
		local yoffset = 256 - self.font.height
		
		for i=#self.messages, 1, -1 do
			if self.verbosities[i] >= self.threshold then
				Graphics.queueText( self.font, self.messages[i], {CONSOLE_TEXT_OFFSET, yoffset}, CONSOLE_COLORS[self.verbosities[i]+1] )
				yoffset = yoffset - self.font.height
				
				if yoffset <= 0.0 then break end
			end
		end
		
		-- render input box
		Graphics.queueQuad( self.input.position, self.input.size, self.uv, self.opacity )
		
		-- render text input
		if self.input.text:len() > 0 then
			Graphics.queueText( self.font, self.input.text, {CONSOLE_TEXT_OFFSET,self.input.position[2]}, CONSOLE_COLORS[1] )
		end
	end
end