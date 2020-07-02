#pragma once
#include "../ECS/typedefs.h"
#include <cglm/cglm.h>
#include "../utility/array.h"

typedef struct{
	array posComps;
	array physComps;
}physicsSystem;

typedef struct{
	COMPTYPE ID;
	mat4 transform;
}comp_pos;

typedef enum{
	cube,
	sphere,
	conv_shell
}hulltype;

typedef struct{
	hulltype a;
}hull;

typedef struct{
	COMPTYPE ID;
	double mass;
	double bsphere_radius;
	hull h;
}comp_physics;

double compute_bsphere_radius(hull h);
int initPhys(physicsSystem * sys);
comp_pos initPosComp();
comp_physics initPhysComp();
hull initHull();
