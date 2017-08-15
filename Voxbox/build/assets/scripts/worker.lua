local workerID = 0
Worker = {}

Worker.create = function( self )
	local newWorker = {}
	newWorker.ID = workerID
	workerID = workerID + 1
	setmetatable( newWorker, self )
	self.__index = self
	return newWorker
end

Worker.destroy = function( self )
end

Worker.load = function( self )
	self.texture = Assets.loadTexture( "./assets/texture/worker.dds" )
	self.position = {0,0,0}
	self.size = {2,4}
	self.uv = {0,0,1,1}
	self.alive = true
end

Worker.unload = function( self )
	Assets.unloadTexture( self.texture )
	self.texture = nil
end

Worker.update = function( self )
end

Worker.render = function( self )
	if self.alive then
		Graphics.queueBillboard( self.position, self.size, self.uv, false, self.texture )
	end
end