#pragma once
#include <stdio.h>
#include <stdint.h>
#include "../graphics/render.h"
#include "../input/input.h"
#include "../utility/array.h"
#include "../schedule/schedule.h"
#include "entity.h"

typedef struct{
	array comparrays;
	renderSystem Render;
	entitySystem Entity;
	physicsSystem Physics;
	scheduleSystem Schedule;
	inputSystem Input;
}systemCore;

int initSystems(systemCore * core);

int make_entity(systemCore * sys, COMPTYPE type);
int addEntityComponents(systemCore * core, COMPTYPE type, COMPTYPE ID);
