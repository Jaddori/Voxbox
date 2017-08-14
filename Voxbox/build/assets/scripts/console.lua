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

console = {}

function console.load()
	console.visible = false
	console.font = Assets.loadFont( "./assets/fonts/verdana12.bin", "./assets/fonts/verdana12.dds" )
	console.position = {0,0}
	console.uv = {0,0,0,0}
	console.size = {CONSOLE_WIDTH,CONSOLE_HEIGHT}
	console.opacity = CONSOLE_OPACITY
end

function console.unload()
end

function console.update()
	if Input.keyReleased( Keys.Console ) then
		console.visible = not console.visible
	end
end

function console.render()
	if console.visible then
		queueQuad( console.position, console.size, console.uv, console.opacity )
		
		local threshold = Log.getThreshold()
		local messages, verbosities = Log.getMessages()
		local yoffset = 256 - console.font.height
		
		for i=#messages, 0, -1 do
			if verbosities[i] >= threshold then
				queueText( console.font, messages[i], {8, yoffset}, CONSOLE_COLORS[verbosities[i]+1] )
				yoffset = yoffset - console.font.height
				
				if yoffset <= 0.0 then break end
			end
		end
	end
end