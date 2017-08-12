#pragma once

class Chunk;
struct CoreData
{
	Camera* perspectiveCamera;
	Input* input;
	Console* console;
	Chunk* chunks;
	DebugShapes* debugShapes;
};