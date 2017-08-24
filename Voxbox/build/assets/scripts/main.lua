require( "./assets/scripts/utils" )
require( "./assets/scripts/worker" )
require( "./assets/scripts/console" )
require( "./assets/scripts/info" )
require( "./assets/scripts/camera" )
require( "./assets/scripts/gui" )
require( "./assets/scripts/action_bar" )

local workers = {}

local unitSelectionStart = vec2()
local unitSelectionEnd = vec2()
local unitSelectionMin = vec2()
local unitSelectionMax = vec2()
local unitSelectionSize = vec2()
local haveUnitSelection = false
local selectedWorker = {}

local testLabel = GuiLabel:create( {300,128}, {128,128}, "Test Label" )
local testButton = GuiButton:create( {128,164}, {128,24}, "Test Button" )
local testTextbox = GuiTextbox:create( {128,196}, {128,24} )

local action =
{
	previewBlock = vec3(),
	havePreview = false,
	startSelection = vec3(),
	endSelection = vec3(),
	selectionBounds = {0,0,0,0,0,0},
	haveSelection = false,
}

function mainLoad()
	console:load()
	info:load()
	camera:load()
	actionBar:load()
	
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
	actionBar:update( dt )
	
	if not console.visible then
		camera:update( dt )
	end
	
	workers[1]:update( dt )
	
	-- check for unit selection
	if Input.buttonPressed( Buttons.Right ) then
		camera.mousePosition:copy( unitSelectionStart )
	end
	
	if Input.buttonDown( Buttons.Right ) then
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
	
	if Input.buttonReleased( Buttons.Right ) then
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
	
	-- perform action
	if actionBar.action == ACTION_DIG and selectedWorker then
		-- get preview block
		if World.hitBlock( camera.rayStart, camera.rayEnd, action.previewBlock ) then
			World.localToWorld( action.previewBlock, action.previewBlock )
			action.havePreview = true
		else
			action.havePreview = false
		end
		
		if action.havePreview then
			-- start selection
			if Input.buttonPressed( Buttons.Left ) then
				action.previewBlock:copy( action.startSelection )
				action.haveSelection = true
			end
			
			-- update selection
			if Input.buttonDown( Buttons.Left ) then
				action.previewBlock:copy( action.endSelection )
				makeCube( action.startSelection, action.endSelection, action.selectionBounds )
			end
		end
		
		if action.haveSelection then
			local selectionHeight = action.selectionBounds[5] - action.selectionBounds[2]
		
			-- size selection
			local mwheel = Input.getMouseWheel()
			if mwheel < 0 and selectionHeight > 0 then
				action.selectionBounds[5] = action.selectionBounds[5] - 1
			elseif mwheel > 0 then
				action.selectionBounds[5] = action.selectionBounds[5] + 1
			end
			
			-- execute action
			if Input.keyReleased( Keys.Space ) then
				selectedWorker:dig( action.selectionBounds )
				actionBar.action = ACTION_SELECT
			end
		end
	elseif actionBar.action == ACTION_BUILD and selectedWorker then
		-- get preview block
		if World.hitBlock( camera.rayStart, camera.rayEnd, action.previewBlock ) then
			World.localToWorld( action.previewBlock, action.previewBlock )
			
			action.previewBlock[2] = action.previewBlock[2] + 1
			action.havePreview = true
		else
			action.havePreview = false
		end
		
		if action.havePreview then
			-- start selection
			if Input.buttonPressed( Buttons.Left ) then
				action.previewBlock:copy( action.startSelection )
				action.haveSelection = true
			end
			
			-- update selection
			if Input.buttonDown( Buttons.Left ) then
				action.previewBlock:copy( action.endSelection )
				makeCube( action.startSelection, action.endSelection, action.selectionBounds )
			end
		end
		
		if action.haveSelection then
			local selectionHeight = action.selectionBounds[5] - action.selectionBounds[2]
		
			-- size selection
			local mwheel = Input.getMouseWheel()
			if mwheel < 0 and selectionHeight > 0 then
				action.selectionBounds[5] = action.selectionBounds[5] - 1
			elseif mwheel > 0 then
				action.selectionBounds[5] = action.selectionBounds[5] + 1
			end
			
			-- execution action
			if Input.keyReleased( Keys.Space ) then
				selectedWorker:build( action.selectionBounds )
				actionBar.action = ACTION_SELECT
			end
		end
	end
end

function mainRender()
	console:render()
	info:render()
	actionBar:render()
	
	workers[1]:render()
	
	-- render unit selection quad
	if haveUnitSelection then
		Graphics.queueQuad( unitSelectionMin, unitSelectionSize, {0,0,0,0}, 0.85 )
	end
	
	if actionBar.action == ACTION_DIG then
		-- render preview block
		if action.havePreview then
			Graphics.queueBlock( action.previewBlock, {1,0,0,0.65} )
		end
		
		-- render selection
		if action.haveSelection then
			for x=action.selectionBounds[1], action.selectionBounds[4] do
				for y=action.selectionBounds[2], action.selectionBounds[5] do
					for z=action.selectionBounds[3], action.selectionBounds[6] do
						Graphics.queueBlock( {x,y,z}, {1,0,0,0.5} )
					end
				end
			end
		end
	elseif actionBar.action == ACTION_BUILD then
		-- render preview block
		if action.havePreview then
			Graphics.queueBlock( action.previewBlock, {1,0,0,0.65} )
		end
		
		-- render selection
		if action.haveSelection then
			for x=action.selectionBounds[1], action.selectionBounds[4] do
				for y=action.selectionBounds[2], action.selectionBounds[5] do
					for z=action.selectionBounds[3], action.selectionBounds[6] do
						Graphics.queueBlock( {x,y,z}, {1,0,0,0.5} )
					end
				end
			end
		end
	end
end