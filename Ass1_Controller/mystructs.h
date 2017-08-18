/*
 * mystructs.h
 *
 *  Created on: Feb 16, 2017
 *      Author: Chandler Newman-Reed
 */

#ifndef MYSTRUCTS_H_
#define MYSTRUCTS_H_

#define NUM_STATES 9
typedef enum {
	START_STATE = 0,
	IDLE_STATE = 1,
	ENTRY_LEFT_STATE = 2,
	ENTRY_RIGHT_STATE = 3,
	EXIT_LEFT_STATE = 4,
	EXIT_RIGHT_STATE = 5,
	LOCK_LEFT_STATE = 6,
	LOCK_RIGHT_STATE = 7,
	STOP_STATE = 8
} State;

#define NUM_INPUTS 12
typedef enum {
	LEFT_SCAN = 0,
	LEFT_DOOR_OPEN = 1,
	RIGHT_DOOR_OPEN = 2,
	LEFT_DOOR_CLOSE = 3,
	RIGHT_DOOR_CLOSE = 4,
	GUARD_LEFT_UNLOCK = 5,
	GUARD_RIGHT_UNLOCK = 6,
	GUARD_LEFT_LOCK = 7,
	GUARD_RIGHT_LOCK = 8,
	RIGHT_SCAN  = 9,
	WEIGHT_SCALE = 10,
	STOP = 11
} Input;

#define NUM_OUTPUTS 11
typedef enum {
	START_MSG = 0,
	IDLE_MSG = 1,
	LOCK_LEFT = 2,
	LOCK_RIGHT = 3,
	UNLOCK_LEFT = 4,
	UNLOCK_RIGHT = 5,
	STOP_MSG = 6,
	LEFT_DOOR_OPENED = 7,
	RIGHT_DOOR_OPENED = 8,
	WEIGHED = 9,
	ID_SCAN = 10
} Output;

const char *outMessage[NUM_OUTPUTS] = {
		"Start Message",
		"Awaiting person",
		"Left door locked",
		"Right door locked",
		"Left door unlocked",
		"Right door unlocked",
		"Stop Display",
		"Person opened left door",
		"Person opened right door",
		"Person weighed",
		"Person scanned ID"
};

const char *inMessage[NUM_INPUTS] = {
		"ls",
		"lo",
		"ro",
		"lc",
		"rc",
		"glu",
		"gru",
		"gll",
		"grl",
		"rs",
		"ws",
		"stop"
};

typedef struct {
	char msg[5];
	int personid;
	int weight;
} Person;

typedef struct {
	int msg;
	Person person;
} Display;
#endif /* MYSTRUCTS_H_ */
