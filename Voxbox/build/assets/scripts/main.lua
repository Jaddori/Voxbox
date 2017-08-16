require( "./assets/scripts/worker" )
require( "./assets/scripts/console" )
require( "./assets/scripts/camera" )

local workers = {}
local rayStart = {}
local rayEnd = {}
local blockLocation = {}
local haveRay = false

function mainLoad()
	console:load()
	
	workerLoad()
	workers[1] = Worker:create()
end

function mainUnload()
	console:unload()
	
	workerUnload()
end

function mainUpdate()
	camera:update()
	console:update()
	
	workers[1]:update()
	
	if Input.buttonReleased( Buttons.Right ) then
		Camera.unproject( camera.mousePosition, 0.0, rayStart )
		Camera.unproject( camera.mousePosition, 1.0, rayEnd )
		
		if World.marchBlock( rayStart, rayEnd, blockLocation ) then
			workers[1].position = blockLocation
			
			local str = "Hit block: " .. tostring(blockLocation[1]) .. "," .. tostring(blockLocation[2]) .. "," .. tostring(blockLocation[3])
			Log.log( VERBOSITY_DEBUG, str )
		end
		
		haveRay = true
	end
end

function mainRender()
	console:render()
	
	workers[1]:render()
	
	if haveRay then
		DebugShapes.addLine( rayStart, rayEnd, {1,1,0,1} )
	end
end