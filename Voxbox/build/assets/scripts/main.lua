require( "./assets/scripts/utils" )
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
		
		if World.hitBlock( rayStart, rayEnd, blockLocation ) then
			--workers[1].position = blockLocation
			
			printBlockLocation( blockLocation )
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