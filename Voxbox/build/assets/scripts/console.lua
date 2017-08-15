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

function console.load()
	console.visible = false
	console.font = Assets.loadFont( "./assets/fonts/verdana12.bin", "./assets/fonts/verdana12.dds" )
	console.position = {0,0}
	console.size = {CONSOLE_WIDTH,CONSOLE_HEIGHT}
	console.uv = {0,0,0,0}
	console.opacity = CONSOLE_OPACITY
	console.flashTime = 0.0
	console.threshold = 0
	console.messages = {}
	console.verbosities = {}
	console.prevMessageCount = 0
	
	console.input = {}
	console.input.position = {0,CONSOLE_HEIGHT+CONSOLE_INPUT_OFFSET}
	console.input.size = {CONSOLE_WIDTH,CONSOLE_INPUT_HEIGHT}
	console.input.text = ""
end

function console.unload()
end

function console.update()
	-- get messages from log
	console.threshold = Log.getThreshold()
	console.messages, console.verbosities = Log.getMessages()
	
	-- check if we need to flash new messages
	if console.visible then
		console.prevMessageCount = #console.messages
	else
		if #console.messages > console.prevMessageCount then
			for i=console.prevMessageCount, #console.messages do
				if console.verbosities[i] == VERBOSITY_WARNING then
				end
				
				if console.verbosities[i] >= console.threshold then
					console.flashTime = CONSOLE_FLASH_TIME
					break
				end
			end
			
			console.prevMessageCount = #console.messages
		end
	end

	-- check if user toggled the console
	if Input.keyReleased( Keys.Console ) then
		console.visible = not console.visible
	end
	
	-- update the console if it is visible
	if console.visible then
		-- get text input
		console.input.text = console.input.text .. Input.textInput()
		
		-- get backspace
		if Input.keyRepeated( Keys.Backspace ) and console.input.text:len() > 0 then
			console.input.text = console.input.text:sub( 0, console.input.text:len()-1 )
		end
		
		-- get enter
		if Input.keyReleased( Keys.Enter ) then
			if console.input.text:len() > 0 then
				log( VERBOSITY_DEBUG, console.input.text )
		
				-- execute entered command
				local chunk, errorMessage = load( console.input.text )
				if errorMessage then
					log( VERBOSITY_ERROR, errorMessage )
				else
					local success, errorMessage = pcall(chunk)
					if not success then
						log( VERBOSITY_ERROR, errorMessage )
					end
				end
				
				console.input.text = ""
			end
		end
	end
	
	if console.flashTime > 0.0 then
		console.flashTime = console.flashTime - 0.01
	end
end

function console.render()
	if console.visible or console.flashTime > 0.0 then
		-- render message box
		Graphics.queueQuad( console.position, console.size, console.uv, console.opacity )
		
		-- render messages
		local yoffset = 256 - console.font.height
		
		for i=#console.messages, 0, -1 do
			if console.verbosities[i] >= console.threshold then
				Graphics.queueText( console.font, console.messages[i], {CONSOLE_TEXT_OFFSET, yoffset}, CONSOLE_COLORS[console.verbosities[i]+1] )
				yoffset = yoffset - console.font.height
				
				if yoffset <= 0.0 then break end
			end
		end
		
		-- render input box
		Graphics.queueQuad( console.input.position, console.input.size, console.uv, console.opacity )
		
		-- render text input
		if console.input.text:len() > 0 then
			Graphics.queueText( console.font, console.input.text, {CONSOLE_TEXT_OFFSET,console.input.position[2]}, CONSOLE_COLORS[1] )
		end
	end
end