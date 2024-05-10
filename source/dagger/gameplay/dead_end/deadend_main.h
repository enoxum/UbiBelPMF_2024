#pragma once

#include "core/system.h"
#include "core/core.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace dead_end
{

	void setupWorld();

	class DeadEndGame
		: public Game
	{
		inline String GetIniFile() override { return "deadend.ini"; }

		void CoreSystemsSetup() override;
		void GameplaySystemsSetup() override;
		void WorldSetup() override;
	};
}