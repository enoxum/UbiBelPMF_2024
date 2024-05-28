#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace red_snake
{
	//void CreateSnake(float tileSize_, ColorRGBA color_, Vector3 pos_);
	void CreateFood(float tileSize_, ColorRGBA color_, Vector3 pos_);
	void SetUpWorld();
	class RedSnakeGame : public Game
	{
		inline String GetIniFile() override
		{
			return "redsnake.ini";
		};

		void CoreSystemsSetup() override;
		void GameplaySystemsSetup() override;
		void WorldSetup() override;

	};
}