#include "physics.h"

int initPhys(physicsSystem * sys){
	int err = 0;
	sys->posComps = arr_init(1, sizeof(comp_pos));
	sys->physComps = arr_init(1, sizeof(comp_physics));
	return err;
}

comp_pos initPosComp(COMPTYPE ID){
	comp_pos t;
	t.ID = ID;
	glm_mat4_identity(t.transform);
	return t;
};

comp_physics initPhysComp(COMPTYPE ID){
	comp_physics t;
	t.ID = ID;
	t.mass = 1;
	t.bsphere_radius = 1;
	t.h = initHull();
	return t;
};

hull initHull(){
	hull t;
	t.a= sphere;
};
