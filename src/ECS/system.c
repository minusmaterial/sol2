#include "system.h"
#include "../utility/maths.h"
#include "../utility/fileload.h"

int initSystems(systemCore * core){
	int err = 0;
	/*
	Just for reference:

	#define COMP_ENTITY 	((COMPTYPE)1)<<0
	#define COMP_RENDER 	((COMPTYPE)1)<<1
	#define COMP_POSITION 	((COMPTYPE)1)<<2
	#define COMP_PHYSICS 	((COMPTYPE)1)<<3

	This might not be up to date, make certain.
	 */
	core->comparrays = arr_init(1, sizeof(array*));
	initEntitySystem(&(core->Entity));

	arr_add(&(core->comparrays), (void*)&(core->Entity.entities));
	arr_add(&(core->comparrays), (void*)&(core->Render.renderComps));
	arr_add(&(core->comparrays), (void*)&(core->Physics.posComps));
	arr_add(&(core->comparrays), (void*)&(core->Physics.physComps));
    initEntitySystem(&(core->Entity));
	initRenderer(&(core->Render), 1920, 1080, "Sol2");
	initPhys(&(core->Physics));
	initInput(&(core->Input));
	
	return err;
};

int make_entity(systemCore * core, COMPTYPE type){
	int err = 0;
	entity e;
	e.type = type;
	core->Entity.maxID++;
	e.ID = core->Entity.maxID;
	if(core->Entity.maxID == 4294967295){
		printf("Probably an entity ID overflow!\n");
		exit(-1);
	}
	arr_add(&(core->Entity.entities), (void*)&e);
	addEntityComponents(core, e.type, e.ID);
	return err;
};
int addEntityComponents(systemCore * core, COMPTYPE type, COMPTYPE ID){
	for(int i = 0; i < MAX_NUM_SYSTEMS; i++){
		printf("heavens preserve me %d\n", i);
		if (!(type & (1<<i))) continue;
		printf("This entity has a component of type %d.\n", i);
		//core->comparrays[i] 
	}
	
}
