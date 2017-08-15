require( "./assets/scripts/worker" )
require( "./assets/scripts/console" )
require( "./assets/scripts/camera" )

local workers = {}
local rayStart = {}
local rayEnd = {}
local haveRay = false

function mainLoad()
	console.load()
	
	workerLoad()
	workers[0] = Worker:create()
end

function mainUnload()
	console.unload()
	
	workerUnload()
end

function mainUpdate()
	camera:update()
	console.update()
	
	workers[0]:update()
	
	if Input.buttonReleased( Buttons.Right ) then
		Camera.unproject( camera.mousePosition, 0.0, rayStart )
		Camera.unproject( camera.mousePosition, 1.0, rayEnd )
		
		haveRay = true
	end
end

function mainRender()
	console.render()
	
	workers[0]:render()
	
	if haveRay then
		DebugShapes.addLine( rayStart, rayEnd, {1,1,0,1} )
	end
end