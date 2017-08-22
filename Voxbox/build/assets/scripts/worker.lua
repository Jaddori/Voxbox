require( "./assets/scripts/utils" )

local WORKER_SPEED = 0.05

function workerLoad()
	Worker =
	{
		position = vec3(),
		size = {2,4},
		texture = Assets.loadTexture( "./assets/textures/worker.dds" ),
		uv = {0,0,0.25,0.5},
		alive = false,
		path = {},
		curPathNode = 0,
		direction = vec3(),
		digBlocks = {},
		curDigBlock = 0,
		buildBlocks = {},
		curBuildBlock = 0,
		selected = false,
	}
	
	Worker.setTarget = function( self, worldBlock )
		self.curPathNode = World.findPath( self.position, worldBlock, self.path )
	end
	
	Worker.dig = function( self, bounds )
		local curBlock = 1
		for x=bounds[1], bounds[4] do
			for y=bounds[2], bounds[5] do
				for z=bounds[3], bounds[6] do
					self.digBlocks[curBlock] = {x,y,z}
					curBlock = curBlock + 1
				end
			end
		end
		
		self.curDigBlock = curBlock-1
		
		-- remove any build blocks
		self.curBuildBlock = 0
	end
	
	Worker.build = function( self, bounds )
		local curBlock = 1
		for x=bounds[1], bounds[4] do
			for y=bounds[2], bounds[5] do
				for z=bounds[3], bounds[6] do
					self.buildBlocks[curBlock] = {x,y,z}
					curBlock = curBlock+1
				end
			end
		end
		
		self.curBuildBlock = curBlock-1
		
		-- remove any dig blocks
		self.curDigBlock = 0
	end
	
	Worker.create = function( self )
		local newWorker = {}
		setmetatable( newWorker, self )
		self.__index = self
		newWorker.alive = true
		return newWorker
	end

	Worker.destroy = function( self )
	end

	Worker.update = function( self, dt )
		-- update pathing
		if self.curPathNode > 0 then
			local target = self.path[self.curPathNode]
			
			Vec3.direction( self.position, target, self.direction )
			self.position = self.position + ( self.direction * WORKER_SPEED )
			
			if Vec3.distance( self.position, target ) < 0.1 then
				self.curPathNode = self.curPathNode - 1
			end
		end
		
		-- update digging
		if self.curDigBlock > 0 then
			-- are we close enough to the block?
			local target = self.digBlocks[self.curDigBlock]
			
			if self.position:distance( target ) < 0.1 then
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
			
			if self.position:distance( target ) < 0.1 then
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

	Worker.render = function( self )
		if self.alive then
			local renderPosition = { self.position[1], self.position[2]+self.size[2]*0.5, self.position[3] }
			Graphics.queueBillboard( renderPosition, self.size, self.uv, false, self.texture )
			
			if self.selected then
				DebugShapes.addSphere( self.position, 2.0, {0.0, 0.0, 1.0, 1.0} )
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
end

function workerUnload()
	Assets.unloadTexture( Worker.texture )
	Worker = nil
end