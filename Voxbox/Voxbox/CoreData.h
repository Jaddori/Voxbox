#pragma once

class Camera;
class Input;
class Region;
class Graphics;
class DebugShapes;
class Assets;
struct CoreData
{
	Camera* perspectiveCamera;
	Input* input;
	Region* regions;
	Graphics* graphics;
	DebugShapes* debugShapes;
	Assets* assets;
};