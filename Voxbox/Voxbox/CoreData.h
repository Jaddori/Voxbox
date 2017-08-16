#pragma once

class Camera;
class Input;
class World;
class Graphics;
class DebugShapes;
class Assets;
struct CoreData
{
	Camera* perspectiveCamera;
	Input* input;
	World* world;
	Graphics* graphics;
	DebugShapes* debugShapes;
	Assets* assets;
};