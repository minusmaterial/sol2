#pragma once
#include <stdio.h>
#include <time.h>

#define SCHED_MEMORY 10

enum schedevent{
	render,
	phys_update,
	misc
};

typedef struct{
	double lastTimes[SCHED_MEMORY];
	double avgTime;
	double curTime;
	double physStep;
	double renderStep;
}scheduleSystem;

int initScheduler(scheduleSystem * sched);
int updateTime(scheduleSystem * sched);
