function printLocalBlock( b )
	Log.log( VERBOSITY_DEBUG, "Block location:" )
	Log.log( VERBOSITY_DEBUG, "\tBlock: " .. tostring(b[1]) .. "," .. tostring(b[2]) .. "," .. tostring(b[3]) )
	Log.log( VERBOSITY_DEBUG, "\tChunk: " .. tostring(b.chunk) )
	Log.log( VERBOSITY_DEBUG, "\tRegion: " .. tostring(b.region) )
	Log.log( VERBOSITY_DEBUG, "\tLength: " .. tostring(b.length) )
end

function printWorldBlock( b )
	Log.log( VERBOSITY_DEBUG, "World block:" )
	Log.log( VERBOSITY_DEBUG, "\tBlock: " .. tostring(b[1]) .. "," .. tostring(b[2]) .. "," .. tostring(b[3]) )
end

function copyLocalBlock( to, from )
	Vec3.copy( from, to )
	
	to.chunk = from.chunk
	to.region = from.region
	to.length = from.length
end

function copyWorldBlock( to, from )
	Vec3.copy( from, to )
end

function printTable( t )
	for k,v in pairs(t) do
		Log.log( VERBOSITY_DEBUG, k .. ": " .. v )
	end
end