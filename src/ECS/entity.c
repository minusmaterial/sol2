#include "entity.h"

int initEntitySystem(entitySystem * sys){
	int err = 0;
	sys->entities = arr_init(1, sizeof(entity));
	sys->maxID = 0;
	return err;
};

