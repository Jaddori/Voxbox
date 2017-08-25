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

function byteString( bytes, unit )
	local result = tostring( bytes )
	if unit == UNIT_BYTES then
		result = result .. " b"
	elseif unit == UNIT_KILOBYTES then
		result = result .. " kb"
	elseif unit == UNIT_MEGABYTES then
		result = result .. " Mb"
	else
		result = result .. " Gb"
	end
	
	return result
end

function makeRectangle( a, b, rect )
	rekt[1] = math.min( a[1], b[1] )
	rekt[2] = math.min( a[3], b[3] )
	rekt[3] = math.max( a[1], b[1] )
	rekt[4] = math.max( a[3], b[3] )
end

function makeCube( a, b, cube )
	cube[1] = math.min( a[1], b[1] )
	cube[2] = math.min( a[2], b[2] )
	cube[3] = math.min( a[3], b[3] )
	cube[4] = math.max( a[1], b[1] )
	cube[5] = math.max( a[2], b[2] )
	cube[6] = math.max( a[3], b[3] )
end

function inside( position, size, point )
	return
	(
		point[1] >= position[1] and
		point[1] <= position[1] + size[1] and
		point[2] >= position[2] and
		point[2] <= position[2] + size[2]
	)
end

function round( value, decimals )
	return math.floor( value * ( 10 ^ decimals ) ) * ( 10 ^ (-decimals) )
end