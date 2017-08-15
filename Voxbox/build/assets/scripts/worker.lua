function workerLoad()
	Worker =
	{
		position = {0,0,0},
		size = {2,4},
		texture = Assets.loadTexture( "./assets/textures/worker.dds" ),
		uv = {0,0,0.25,0.5},
		alive = false
	}
	
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
		end
	end
end

function workerUnload()
	Assets.unloadTexture( Worker.texture )
	Worker = nil
end