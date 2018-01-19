#pragma once
#include <cstdint>
#include "Scene.h"
#include "Window.h"
#include <atomic>

enum LIGHTS
{
	AMBIANT = 1,
	DIFFUSE = 2,
	SPECULAR = 3
};
enum AASELECTION
{
	NOAA,
	AA2X,
	AA4X,
	AA8X,
	AA16X
};


enum MESHMODE
{
	CUBE,
	SPHERE
};

struct Actions
{
	bool moveLeft;
	bool moveRight;
	bool moveUp;
	bool moveDown;
	bool zoomIn;
	bool zoomOut;
	bool xTurnClockwise;
	bool xTurnCounterClockwise;
	bool yTurnClockwise;
	bool yTurnCounterClockwise;
	bool zTurnClockwise;
	bool zTurnCounterClockwise;
	bool increaseLight;
	bool decreaseLight;
	bool addRed;
	bool addGreen;
	bool addBlue;
	bool addTransparency;
	bool changeAAValue;
	bool showHelp;
};

struct LightParams
{
	float ambiant;
	float diffuse;
	float specular;

	LightParams() { Reset(); }

	void Reset()
	{
		ambiant = 50;
		diffuse = 70;
		specular = 90;
	}
};

struct MeshParams
{
	float red;
	float green;
	float blue;
	float transparency;
	uint8_t cubeImageID = 0;
	uint8_t sphereImageID = 0;

	MeshParams() { Reset(); }

	void Reset()
	{
		red = 255;
		green = 0;
		blue = 0;
		transparency = 75;
	}
};

struct CameraParams
{
	float xOffset;
	float yOffset;
	float zoomOffset;
	float xRotationOffset;
	float yRotationOffset;
	float zRotationOffset;

	CameraParams() { Reset(); }

	void Reset()
	{
		xOffset = 0;
		yOffset = 0;
		zoomOffset = -6;
		xRotationOffset = 45;
		yRotationOffset = 45;
		zRotationOffset = 0;
	}
};

struct KeyHistory
{
	SDL_Keycode keys[10];
	uint8_t buffer = 0;
	SDL_Keycode code[10] = {
		SDLK_z,
		SDLK_e,
		SDLK_l,
		SDLK_d,
		SDLK_a,
		SDLK_s,
		SDLK_u,
		SDLK_c,
		SDLK_k,
		SDLK_s
	};

	KeyHistory() { Reset(); }

	void AddKey(const SDL_Keycode key)
	{
		keys[buffer++] = key;
		if (buffer == 10)
			buffer = 0;
	}

	void Reset()
	{
		for (uint8_t i = 0; i < 10; ++i)
			keys[i] = 63;

		buffer = 0;
	}

	bool IsKonamiCode()
	{
		bool gg = false;
		uint8_t goodKeys = 0;
		uint8_t indice = 0;

		for (uint8_t j = 0; j < 10; ++j)
		{
			goodKeys = 0;
			for (uint8_t i = 0; i < 10; ++i)
			{
				indice = i + j;
				while (indice > 9) indice -= 10;

				if (keys[indice] == code[i])
					++goodKeys;
			}
			if (goodKeys == 10) gg = true;
		}

		return gg;
	}
};

struct Zelda
{
	inline const static float xMaxRotations = 4;
	inline const static float yMaxRotations = 4;
	inline const static uint8_t translationSpeed = 3;
	inline const static uint8_t rotationSpeed = 180;
	inline const static float mat4dest_x = -1.73f;
	inline const static float mat4dest_y = -1.5f;
	inline const static float mat4dest_z = -10.f;
	inline const static float mat5dest_x = 1.73f;
	inline const static float mat5dest_y = -1.5f;
	inline const static float mat5dest_z = -10.f;
	inline const static float mat6dest_x = 0.f;
	inline const static float mat6dest_y = 1.5f;
	inline const static float mat6dest_z = -10.f;

	Zelda() { Reset(); }

	float mat4_x_rotation, mat4_y_rotation, mat4_x, mat4_y, mat4_z;
	float mat5_x_rotation, mat5_y_rotation, mat5_x, mat5_y, mat5_z;
	float mat6_x_rotation, mat6_y_rotation, mat6_x, mat6_y, mat6_z;

	void Reset()
	{
		const uint8_t translationOffset = 15;
		mat4_x_rotation = 0;
		mat4_y_rotation = 0;
		mat4_x = -1.73 - translationOffset;
		mat4_y = -1.5f - translationOffset;
		mat4_z = -10.f - translationOffset;

		mat5_x_rotation = 0;
		mat5_y_rotation = 0;
		mat5_x = 1.73f + translationOffset;
		mat5_y = -1.5f - translationOffset;
		mat5_z = -10.f - translationOffset;

		mat6_x_rotation = 0;
		mat6_y_rotation = 0;
		mat6_x = 0.f;
		mat6_y = 1.5f + translationOffset;
		mat6_z = -10.f - translationOffset;
	}
};

struct ApplicationInfos
{
	bool isRunning = true;
	bool showInterface = true;
	float lastTime = 0;
	float currentTime = 0;
	float deltaTime = 0;
	float secondCubeRotationOffset = 0;
	uint16_t fpsCounter = 0;
	uint16_t minFps = 65535;
	uint16_t maxFps = 0;
	uint16_t averageFps = 0;
	uint16_t fpsValues[10];
	uint8_t fpsValuesBuffer = 0;
	uint8_t selectedVersion = 1;
	uint8_t selectedLight = AMBIANT;
	LightParams lightParams;
	MeshParams meshParams;
	CameraParams cameraParams;
	uint8_t selectedAA = NOAA;
	uint16_t polygons = 0;
	uint8_t meshMode = CUBE;
	KeyHistory keyHistory;
	Zelda zelda;

	void Reset()
	{
		selectedLight = AMBIANT;
		selectedAA = NOAA;
		keyHistory.Reset();
		lightParams.Reset();
		meshParams.Reset();
		cameraParams.Reset();
	}
};

struct SharedContext
{
	Scene* scene = nullptr;
	Window* window = nullptr;

	ApplicationInfos appInfos;
	Actions actions;

	void RefreshScene() const
	{
		scene->ClearScene();
		scene->InitMeshes(appInfos.selectedVersion, appInfos.meshMode);
		scene->InitEntities(appInfos.selectedVersion, appInfos.meshMode);
		scene->InitLights();
	}
};