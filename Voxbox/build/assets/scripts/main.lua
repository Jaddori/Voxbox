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

local unitSelectionStart = vec2()
local unitSelectionEnd = vec2()
local unitSelectionMin = vec2()
local unitSelectionMax = vec2()
local unitSelectionSize = vec2()
local haveUnitSelection = false
local selectedWorker = {}

function mainLoad()
	console:load()
	info:load()
	camera:load()
	
	workerLoad()
	workers[1] = Worker:create()
	workers[1].position = vec3( 5, 31+1, 25 )
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
	
	--[[if Input.buttonReleased( Buttons.Right ) then
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
	
	if haveSelection then
		if Input.buttonDown( Buttons.Middle ) then
			Camera.unproject( camera.mousePosition, 0.0, rayStart )
			Camera.unproject( camera.mousePosition, 1.0, rayEnd )
			
			if World.hitBlock( rayStart, rayEnd, localBlock ) then
				World.localToWorld( localBlock, worldBlock )
				
				copyWorldBlock( endSelection, worldBlock )
			end
		end
		
		if Input.keyReleased( Keys.Enter ) then
			local minX = math.min( startSelection[1], endSelection[1] )
			local minZ = math.min( startSelection[3], endSelection[3] )
			
			local maxX = math.max( startSelection[1], endSelection[1] )
			local maxZ = math.max( startSelection[3], endSelection[3] )
			
			Log.log( VERBOSITY_DEBUG, "Starting dig." )
			workers[1]:dig( {minX,minZ,maxX,maxZ}, startSelection[2] )
		end
	end--]]
	
	-- check for unit selection
	if Input.buttonPressed( Buttons.Left ) then
		camera.mousePosition:copy( unitSelectionStart )
	end
	
	if Input.buttonDown( Buttons.Left ) then
		camera.mousePosition:copy( unitSelectionEnd )
		
		unitSelectionMin[1] = math.min(unitSelectionStart[1], unitSelectionEnd[1])
		unitSelectionMin[2] = math.min(unitSelectionStart[2], unitSelectionEnd[2])
		
		unitSelectionMax[1] = math.max( unitSelectionStart[1], unitSelectionEnd[1] )
		unitSelectionMax[2] = math.max( unitSelectionStart[2], unitSelectionEnd[2] )
		
		unitSelectionSize = unitSelectionMax - unitSelectionMin
		
		haveUnitSelection = true
	else
		haveUnitSelection = false
	end
	
	if Input.buttonReleased( Buttons.Left ) then
		for i=1, #workers do
			local worldPosition = vec3()
			workers[i].position:copy( worldPosition )
			
			local windowPosition = {0,0}
			Camera.project( worldPosition, windowPosition )
			
			-- check if workers window position is inside the selection box
			if windowPosition[1] > unitSelectionMin[1] and
				windowPosition[1] < unitSelectionMax[1] and
				windowPosition[2] > unitSelectionMin[2] and
				windowPosition[2] < unitSelectionMax[2] then
				workers[i].selected = true
				selectedWorker = workers[i]
			else
				workers[i].selected = false
			end
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
	
	-- render unit selection quad
	if haveUnitSelection then
		Graphics.queueQuad( unitSelectionStart, unitSelectionSize, {0,0,0,0}, 0.85 )
	end
end