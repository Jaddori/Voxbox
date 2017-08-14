require("./assets/scripts/worker")

w1 = Worker:create()
w2 = Worker:create()

function load()
	print( "LUA LOAD" )
	
	w1:load()
	w2:load()
end

function unload()
	print( "LUA UNLOAD" )
	
	w1:unload()
	w2:unload()
end

function update()
	print( "LUA UPDATE" )
	
	w1:update()
	w2:update()
end

function render()
	print( "LUA RENDER" )
	
	w1:render()
	w2:render()
end