require("./assets/scripts/worker")
require("./assets/scripts/console")

w1 = Worker:create()
w2 = Worker:create()

print = function(str)
	log( VERBOSITY_DEBUG, str )
end

function load()
	print( "LUA LOAD" )
	console.load()
	
	w1:load()
	w2:load()
end

function unload()
	print( "LUA UNLOAD" )
	console.unload()
	
	w1:unload()
	w2:unload()
end

function update()
	console.update()
	
	w1:update()
	w2:update()
end

function render()
	console.render()
	
	w1:render()
	w2:render()
end