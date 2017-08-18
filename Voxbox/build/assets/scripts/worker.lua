require( "./assets/scripts/utils" )

function workerLoad()
	Worker =
	{
		position = {0,0,0},
		size = {2,4},
		texture = Assets.loadTexture( "./assets/textures/worker.dds" ),
		uv = {0,0,0.25,0.5},
		alive = false,
		path = {},
		curPathNode = 0,
	}
	
	Worker.setTarget = function( self, worldBlock )
		self.curPathNode = World.findPath( self.position, worldBlock, self.path )
		Log.log( VERBOSITY_DEBUG, "Cur path node: " .. tostring(self.curPathNode) )
		Log.log( VERBOSITY_DEBUG, "Path nodes: " .. tostring(#self.path) )
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

	Worker.update = function( self )
		if self.curPathNode > 0 then
			local target = self.path[self.curPathNode]
			
			local dir = direction( self.position, target )
			self.position[1] = self.position[1] + dir[1] * 0.05
			self.position[2] = self.position[2] + dir[2] * 0.05
			self.position[3] = self.position[3] + dir[3] * 0.05
			
			if distance( self.position, target ) < 0.1 then
				self.curPathNode = self.curPathNode - 1
			end
		end
	end

	Worker.render = function( self )
		if self.alive then
			Graphics.queueBillboard( self.position, self.size, self.uv, false, self.texture )
			DebugShapes.addSphere( self.position, 2.0, {0.0, 0.0, 1.0, 1.0} )
			
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