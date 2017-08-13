#pragma once

class Camera;
class Input;
class Console;
class Chunk;
class Graphics;
class DebugShapes;
struct CoreData
{
	Camera* perspectiveCamera;
	Input* input;
	Console* console;
	Chunk* chunks;
	Graphics* graphics;
	DebugShapes* debugShapes;
};