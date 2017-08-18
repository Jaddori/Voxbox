require( "./assets/scripts/utils" )

function workerLoad()
	Worker =
	{
		position = {0,0,0},
		size = {2,4},
		texture = Assets.loadTexture( "./assets/textures/worker.dds" ),
		uv = {0,0,0.25,0.5},
		alive = false,
		target = {0,0,0},
		reachedTarget = false
	}
	
	Worker.setTarget = function( self, worldBlock )
		copyWorldBlock( self.target, worldBlock )
		reachedTarget = false
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
	end

	Worker.render = function( self )
		if self.alive then
			Graphics.queueBillboard( self.position, self.size, self.uv, false, self.texture )
			DebugShapes.addSphere( self.position, 2.0, {0.0, 0.0, 1.0, 1.0} )
			
			if not self.reachedTarget then
				local pos = { self.position[1], self.position[2]+0.5, self.position[3] }
				local target = { self.target[1], self.target[2]+0.5, self.target[3] }
				DebugShapes.addLine( pos, target, {1,0,1,1} )
				DebugShapes.addSphere( target, 0.5, {1,0,1,1} )
			end
		end
	end
end

function workerUnload()
	Assets.unloadTexture( Worker.texture )
	Worker = nil
end