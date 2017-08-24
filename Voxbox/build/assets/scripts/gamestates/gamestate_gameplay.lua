require( "./assets/scripts/gamestates/gamestate_base" )
require( "./assets/scripts/gamestates/gamestate_pause" )
require( "./assets/scripts/action_bar" )
require( "./assets/scripts/worker" )
require( "./assets/scripts/actions/action_select" )
require( "./assets/scripts/actions/action_dig" )
require( "./assets/scripts/actions/action_build" )

Gameplay = {}

function Gameplay:load()
	actionBar:load()

	self.workers = {}
	self.workers[1] = Worker:create()
	self.workers[1].position = vec3( 5, 31+1, 25 )
	
	self.actions = {}
	self.actions[ACTION_SELECT] = SelectAction
	self.actions[ACTION_SELECT].workers = self.workers
	self.actions[ACTION_DIG] = DigAction
	self.actions[ACTION_BUILD] = BuildAction
end

function Gameplay:unload()
end

function Gameplay:update( dt )
	actionBar:update( dt )
	
	if not console.visible then
		camera:update( dt )
	end
	
	-- update current action
	self.actions[actionBar.action]:update( dt, self.actions[ACTION_SELECT].worker )
	
	-- update workers
	for i=1, #self.workers do
		self.workers[i]:update( dt )
	end
	
	if Input.keyReleased( "F" ) then
		Gamestate:push( Pause )
	end
end

function Gameplay:render()
	World.queueChunks()
	actionBar:render()
	
	self.actions[actionBar.action]:render()
	
	for i=1, #self.workers do
		self.workers[i]:render()
	end
end