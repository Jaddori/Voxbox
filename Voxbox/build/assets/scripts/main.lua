require( "./assets/scripts/utils" )
require( "./assets/scripts/worker" )
require( "./assets/scripts/console" )
require( "./assets/scripts/info" )
require( "./assets/scripts/camera" )

local workers = {}
local rayStart = {}
local rayEnd = {}
local localBlock = {}
local worldBlock = {}
local haveRay = false

function mainLoad()
	console:load()
	info:load()
	
	workerLoad()
	workers[1] = Worker:create()
	workers[1].position = { 5, 31+1, 25 }
end

function mainUnload()
	console:unload()
	info:unload()
	
	workerUnload()
end

function mainUpdate()
	console:update()
	info:update()
	
	if not console.visible then
		camera:update()
	end
	
	workers[1]:update()
	
	if Input.buttonReleased( Buttons.Right ) then
		Camera.unproject( camera.mousePosition, 0.0, rayStart )
		Camera.unproject( camera.mousePosition, 1.0, rayEnd )
		
		if World.hitBlock( rayStart, rayEnd, localBlock ) then
			World.localToWorld( localBlock, worldBlock )
			
			workers[1]:setTarget( worldBlock )
			World.setBlock( localBlock, 10 )
		end
		
		haveRay = true
	end
end

function mainRender()
	console:render()
	info:render()
	
	workers[1]:render()
	
	if haveRay then
		DebugShapes.addLine( rayStart, rayEnd, {1,1,0,1} )
	end
end