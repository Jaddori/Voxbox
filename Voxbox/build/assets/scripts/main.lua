require("./assets/scripts/worker")
require("./assets/scripts/console")

local workers = {}

function mainLoad()
	console.load()
	
	workerLoad()
	workers[0] = Worker:create()
end

function mainUnload()
	console.unload()
	
	workerUnload()
end

function mainUpdate()
	console.update()
	
	workers[0]:update()
end

function mainRender()
	console.render()
	
	workers[0]:render()
end