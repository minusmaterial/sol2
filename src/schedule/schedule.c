#include "schedule.h"

int initScheduler(scheduleSystem * sched){
	int err = 0;
	for(int i = 0; i < SCHED_MEMORY; i++){
		(sched->lastTimes)[i] = 0;
	}
	sched->avgTime = 0;
	sched->curTime = 0;
	sched->physStep = 0.05;
	sched->renderStep = 0.016;
	return err;
}

int updateTime(scheduleSystem * sched){
	int err = 0;
	double temp = sched->curTime;
	//sched->curTime = 10*clock()/((float)CLOCKS_PER_SEC);
	sched->curTime = time(0);
	//printf("curtime: %f\n", sched->curTime);
	for(int i = 0 ; i < SCHED_MEMORY-1; i++){
		sched->lastTimes[i] = sched->lastTimes[i+1];
	};
	sched->lastTimes[SCHED_MEMORY-1] = sched->curTime;
	sched->avgTime = (sched->lastTimes[SCHED_MEMORY-1] - sched->lastTimes[0]) / SCHED_MEMORY;
	//printf("average cycle time over last %d cycles: %f\n", SCHED_MEMORY, sched->avgTime);
	return err;
}
