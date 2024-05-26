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
		float health = 10;
		HealthSystem healthSystem;

		EsccapeCharacter() : id(0), speed(1), health(10), healthSystem() {}
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