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

local startSelection = {0,0,0}
local endSelection = {0,0,0}
local haveSelection = false

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

function mainUpdate( dt )
	console:update( dt )
	info:update( dt )
	
	if not console.visible then
		camera:update( dt )
	end
	
	workers[1]:update( dt )
	
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
	
	if Input.buttonPressed( Buttons.Middle ) then
		Camera.unproject( camera.mousePosition, 0.0, rayStart )
		Camera.unproject( camera.mousePosition, 1.0, rayEnd )
		
		if World.hitBlock( rayStart, rayEnd, localBlock ) then
			World.localToWorld( localBlock, worldBlock )
			
			copyWorldBlock( startSelection, worldBlock )
			haveSelection = true
		end
	end
	
	if haveSelection and Input.buttonDown( Buttons.Middle ) then
		Camera.unproject( camera.mousePosition, 0.0, rayStart )
		Camera.unproject( camera.mousePosition, 1.0, rayEnd )
		
		if World.hitBlock( rayStart, rayEnd, localBlock ) then
			World.localToWorld( localBlock, worldBlock )
			
			copyWorldBlock( endSelection, worldBlock )
		end
	end
end

function mainRender()
	console:render()
	info:render()
	
	workers[1]:render()
	
	if haveRay then
		DebugShapes.addLine( rayStart, rayEnd, {1,1,0,1} )
	end
	
	local y = startSelection[2]
	local minX = math.min( startSelection[1], endSelection[1] )
	local minZ = math.min( startSelection[3], endSelection[3] )
	
	local maxX = math.max( startSelection[1], endSelection[1] )
	local maxZ = math.max( startSelection[3], endSelection[3] )
	
	if haveSelection then
		for x=minX, maxX do
			for z=minZ, maxZ do
				Graphics.queueBlock( {x, y, z}, {1,0,0,0.5} )
			end
		end
	end
end