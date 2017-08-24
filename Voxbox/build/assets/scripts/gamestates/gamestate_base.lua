Gamestate =
{
	current = 0,
	states = {},
}

function Gamestate:push( state )
	self.current = self.current + 1
	self.states[self.current] = state
end

function Gamestate:pop()
	self.current = self.current - 1
end

function Gamestate:clear()
	self.current = 0
end

function Gamestate:update( dt )
	self.states[self.current]:update( dt )
end

function Gamestate:render()
	self.states[self.current]:render()
end