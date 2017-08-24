require( "./assets/scripts/utils" )

local WORKER_SPEED = 0.05
local WORKER_PATH_NODE_PROXIMITY = 0.1

Worker = {}
Worker.__index = Worker

function Worker:create()
	local worker =
	{
		position = vec3(),
		size = vec2(2,4),
		texture = Assets.loadTexture( "./assets/textures/worker.dds" ),
		uv = {0,0,0.25,0.5},
		alive = true,
		path = {},
		curPathNode = 0,
		direction = vec3(),
		digBlocks = {},
		curDigBlock = 0,
		buildBlocks = {},
		curBuildBlock = 0,
		selected = false,
	}
	
	setmetatable( worker, Worker )
	return worker
end

function Worker:destroy()
	Assets.unloadTexture( self.texture )
end

function Worker:setTarget( worldBlock )
end

function Worker:dig( bounds )
	local curBlock = 1
	for y=bounds[2], bounds[5] do
		for x=bounds[1], bounds[4] do
			for z=bounds[3], bounds[6] do
				self.digBlocks[curBlock] = {x,y,z}
				curBlock = curBlock + 1
			end
		end
	end
	
	self.curDigBlock = curBlock-1
	
	-- remove potential build blocks
	self.curBuildBlock = 0
end

function Worker:build( bounds )
	local curBlock = 1
	for y=bounds[2], bounds[5] do
		for x=bounds[1], bounds[4] do
			for z=bounds[3], bounds[6] do
				self.buildBlocks[curBlock] = {x,y,z}
				curBlock = curBlock + 1
			end
		end
	end
	
	self.curBuildBlock = curBlock-1
	
	-- remove potential build blocks
	self.curDigBlock = 0
end

function Worker:update( dt )
	-- update pathing
	if self.curPathNode > 0 then
		local target = self.path[self.curPathNode]
		
		Vec3.direction( self.position, target, self.direction )
		self.position = self.position + ( self.direction * WORKER_SPEED )
		
		if self.position:distance( target ) < WORKER_PATH_NODE_PROXIMITY then
			self.curPathNode = self.curPathNode - 1
		end
	end
	
	-- update digging
	if self.curDigBlock > 0 then
		local target = self.digBlocks[self.curDigBlock]
		
		if self.position:distance( target ) < WORKER_PATH_NODE_PROXIMITY then
			-- dig the block
			local localTarget = {}
			World.worldToLocal( target, localTarget )
			
			World.setBlock( localTarget, 0 )
			self.curDigBlock = self.curDigBlock - 1
		else
			-- move to block
			Vec3.direction( self.position, target, self.direction )
			self.position = self.position + ( self.direction * WORKER_SPEED )
		end
	end
	
	-- update building
	if self.curBuildBlock > 0 then
		local target = self.buildBlocks[self.curBuildBlock]
		
		if self.position:distance( target ) < WORKER_PATH_NODE_PROXIMITY then
			-- build the block
			local localTarget = {}
			World.worldToLocal( target, localTarget )
			
			World.setBlock( localTarget, 2 )
			self.curBuildBlock = self.curBuildBlock - 1
		else
			-- move to block
			self.position:direction( target, self.direction )
			self.position = self.position + ( self.direction * WORKER_SPEED )
		end
	end
end

function Worker:render()
	if self.alive then
		local renderPosition = vec3()
		self.position:copy( renderPosition )
		renderPosition[2] = renderPosition[2] + self.size[2]*0.5
		
		Graphics.queueBillboard( renderPosition, self.size, self.uv, false, self.texture )
		
		if self.selected then
			DebugShapes.addSphere( self.position, 2.0, {0,0,1,1} )
		end
		
		if #self.path > 0 then
			for i=#self.path, 1, -1 do
				local color = {0,0,0,1}
				if i > self.curPathNode then
					color = {1,0,0,1}
				end
				
				local pos = { self.path[i][1], self.path[i][2]+1.5, self.path[i][3] }
				DebugShapes.addSphere( self.path[i], 0.5, color )
			end
			
			DebugShapes.addSphere( self.path[1], 1.0, {1,1,1,1} )
		end
	end
end