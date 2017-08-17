require( "./assets/scripts/utils" )
require( "./assets/scripts/worker" )
require( "./assets/scripts/console" )
require( "./assets/scripts/camera" )

local workers = {}
local rayStart = {}
local rayEnd = {}
local localBlock = {}
local worldBlock = {}
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
	console:update()
	
	if not console.visible then
		camera:update()
	end
	
	workers[1]:update()
	
	if Input.buttonReleased( Buttons.Right ) then
		Camera.unproject( camera.mousePosition, 0.0, rayStart )
		Camera.unproject( camera.mousePosition, 1.0, rayEnd )
		
		if World.hitBlock( rayStart, rayEnd, localBlock ) then
			printLocalBlock( localBlock )
			
			World.localToWorld( localBlock, worldBlock )
			printWorldBlock( worldBlock )
			
			copyWorldBlock( workers[1].position, worldBlock )
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
	
	if localBlock.length ~= nil then
		local rayDir = { rayEnd[1]-rayStart[1], rayEnd[2]-rayStart[2], rayEnd[3]-rayStart[3] }
		local rayLen = math.sqrt( rayDir[1]*rayDir[1] + rayDir[2]*rayDir[2] + rayDir[3]*rayDir[3] )
		rayDir[1] = rayDir[1] / rayLen
		rayDir[2] = rayDir[2] / rayLen
		rayDir[3] = rayDir[3] / rayLen
		
		local endPos = { rayStart[1] + rayDir[1] * localBlock.length,
							rayStart[2] + rayDir[2] * localBlock.length,
							rayStart[3] + rayDir[3] * localBlock.length };
		
		DebugShapes.addSphere( endPos, 1.0, {1,1,1,1} )
		
		local endPosTop = { endPos[1], endPos[2]+50, endPos[3] }
		DebugShapes.addLine( endPos, endPosTop, {1,1,1,1} )
	end
end