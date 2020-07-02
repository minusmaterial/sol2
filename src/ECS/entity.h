#pragma once
#include <stdio.h>
#include "typedefs.h"
#include "../graphics/render.h"
#include "../utility/array.h"

typedef struct{
	COMPTYPE ID;
	COMPTYPE type;
}entity;

typedef struct{
	array entities;
	COMPTYPE maxID;
}entitySystem;

int initEntitySystem(entitySystem * sys);
