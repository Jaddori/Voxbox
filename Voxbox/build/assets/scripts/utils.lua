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
	to[1] = from[1]
	to[2] = from[2]
	to[3] = from[3]
	
	to.chunk = from.chunk
	to.region = from.region
	to.length = from.length
end

function copyWorldBlock( to, from )
	to[1] = from[1]
	to[2] = from[2]
	to[3] = from[3]
end

function vectorToString( v )
	local str = v[1]
	for i=2, #v do
		str = str .. "," .. tostring(v[i])
	end
	
	return str
end

function printTable( t )
	for k,v in pairs(t) do
		Log.log( VERBOSITY_DEBUG, k .. ": " .. v )
	end
end

function distance( a, b )
	local c = { b[1]-a[1], b[2]-a[2], b[3]-a[3] }
	return math.sqrt( c[1]*c[1] + c[2]*c[2] + c[3]*c[3] )
end

function direction( a, b )
	local c = { b[1]-a[1], b[2]-a[2], b[3]-a[3] }
	local clen = math.sqrt( c[1]*c[1] + c[2]*c[2] + c[3]*c[3] )
	return { c[1] / clen, c[2] / clen, c[3] / clen }
end