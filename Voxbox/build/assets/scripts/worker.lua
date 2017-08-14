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
	print( "destroying worker " .. self.ID )
end

Worker.load = function( self )
	print( "loading worker " .. self.ID )
end

Worker.unload = function( self )
	print( "unloading worker " .. self.ID )
end

Worker.update = function( self )
end

Worker.render = function( self )
end