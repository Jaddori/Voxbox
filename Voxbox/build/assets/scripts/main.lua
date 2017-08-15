require("./assets/scripts/worker")
require("./assets/scripts/console")

local workers = {}

function mainLoad()
	console.load()
	
	workers[0] = Worker:create()
	workers[0]:load()
end

function mainUnload()
	console.unload()
	
	workers[0]:unload()
end

function mainUpdate()
	console.update()
	
	workers[0]:update()
end

function mainRender()
	console.render()
	
	workers[0]:render()
end