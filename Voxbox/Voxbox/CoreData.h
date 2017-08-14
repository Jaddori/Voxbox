#pragma once

class Camera;
class Input;
class Chunk;
class Graphics;
class DebugShapes;
class Assets;
struct CoreData
{
	Camera* perspectiveCamera;
	Input* input;
	Chunk* chunks;
	Graphics* graphics;
	DebugShapes* debugShapes;
	Assets* assets;
};