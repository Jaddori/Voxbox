Gamestate =
{
	current = 0,
	states = {},
}

function Gamestate:push( state )
	table.insert( Gamestate.states, state )
	Gamestate.current = Gamestate.current + 1
end

function Gamestate:pop()
	Gamestate.current = Gamestate.current - 1
end

function Gamestate:clear()
	Gamestate.current = 0
end

function Gamestate:update( dt )
	self.states[self.current]:update( dt )
end

function Gamestate:render()
	self.states[self.current]:render()
end