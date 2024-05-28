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
		float speed{ 100.f };
		float health = 10;
		int attackCooldown = 50;
		HealthSystem healthSystem;
		bool inputContextReversed;

		EsccapeCharacter() : id(0), speed(100.f), health(10), attackCooldown(50), healthSystem() {}
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