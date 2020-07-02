#pragma once
#include <stdint.h>

#define COMPTYPE uint32_t
#define MAX_NUM_SYSTEMS sizeof(COMPTYPE)*8
#define COMP_ENTITY 	((COMPTYPE)1)<<0
#define COMP_RENDER 	((COMPTYPE)1)<<1
#define COMP_POSITION 	((COMPTYPE)1)<<2
#define COMP_PHYSICS 	((COMPTYPE)1)<<3
