#pragma once

#include "core/core.h"
#include "core/system.h"
#include "character_controller.h"

#include "health_system.h"

using namespace dagger;

namespace esccape
{

	struct EsccapeCharacter
	{
		int id = 0;
		int speed{ 1 };
		//HealthSystem& healthSystem;
	};


	class EsccapeControllerSystem
		: public System
	{
		CharacterControllerFSM characterFSM;

	public:
		String SystemName() override {
			return "Character Controller System";
		}

		void Run() override;
	};
}