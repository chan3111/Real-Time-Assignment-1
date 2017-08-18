#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>

#include "mystructs.h"

int coid;
int smsg;
char path;

// States
void *idle_state(Person);
void *left_scan(Person);
void *right_scan(Person);
void *left_open(Person);
void *right_open(Person);
void *left_close(Person);
void *right_close(Person);
void *guard_left_lock(Person);
void *guard_right_lock(Person);
void *guard_left_unlock(Person);
void *guard_right_unlock(Person);
void *weighed(Person);

// Functions
void sendDisplay(int, Person);

typedef void *(*StateFunc)(Person);


int main(int argc, char *argv[]) {
	int rcvid, chid;
	pid_t displaypid = atoi(argv[1]);
	Person person;
	Display display;

	StateFunc states = idle_state;

	display.person = person;

	chid = ChannelCreate(0);
	if(chid == -1){
		perror("Failed to create a channel.\n");
		exit(EXIT_FAILURE);
	}

	coid = ConnectAttach(ND_LOCAL_NODE, displaypid, 1, _NTO_SIDE_CHANNEL, 0);
	if(coid == -1){
		fprintf(stderr, "Couldn't attach to client.\n");
		perror(NULL);
		exit(EXIT_FAILURE);
	}


	printf("Server PID is: %d\n", getpid());

	display.msg = IDLE_MSG;
	MsgSend(coid, &display, sizeof(display), NULL, 0);

	while(1){
		rcvid = MsgReceive(chid, &person, sizeof(person), NULL);
		states = (StateFunc)(*states)(person);
		MsgReply(rcvid, EOK, NULL, 0);
		if(strcmp(person.msg, inMessage[STOP]) == 0){
			printf("Stopping controller.\n");
			sendDisplay(STOP_MSG, person);
			exit(EXIT_SUCCESS);
		}
	}

	return EXIT_SUCCESS;
}


void *idle_state(Person person){
	if(strcmp(person.msg, inMessage[LEFT_SCAN]) == 0){
		path = 'N';
		sendDisplay(ID_SCAN, person);
		return left_scan;
	} else if(strcmp(person.msg, inMessage[RIGHT_SCAN]) == 0){
		path = 'X';
		sendDisplay(ID_SCAN, person);
		return right_scan;
	}
	return idle_state;
}


void* left_scan(Person person){
	if(strcmp(person.msg, inMessage[GUARD_LEFT_UNLOCK]) == 0){
		sendDisplay(UNLOCK_LEFT, person);
		return guard_left_unlock;
	}
	return left_scan;
}


void* right_scan(Person person){
	if(strcmp(person.msg, inMessage[GUARD_RIGHT_UNLOCK]) == 0){
		sendDisplay(UNLOCK_RIGHT, person);
		return guard_right_unlock;
	}
	return right_scan;
}


void* left_open(Person person){
	if(strcmp(person.msg, inMessage[WEIGHT_SCALE]) == 0){
		sendDisplay(WEIGHED, person);
		return weighed;
	} else if (strcmp(person.msg, inMessage[LEFT_DOOR_CLOSE]) == 0){
		return left_close;
	}
	return left_open;
}


void* right_open(Person person){
	if(strcmp(person.msg, inMessage[WEIGHT_SCALE]) == 0){
		sendDisplay(WEIGHED, person);
		return weighed;
	} else if(strcmp(person.msg, inMessage[RIGHT_DOOR_CLOSE]) == 0){
		return right_close;
	}
	return right_open;
}


void* guard_left_lock(Person person){
	if(strcmp(person.msg, inMessage[GUARD_RIGHT_UNLOCK]) == 0){
		sendDisplay(UNLOCK_RIGHT, person);
		return guard_right_unlock;
	}
	return guard_left_lock;
}


void* guard_right_lock(Person person){
	if(strcmp(person.msg, inMessage[GUARD_LEFT_UNLOCK]) == 0){
		sendDisplay(UNLOCK_LEFT, person);
		return guard_left_unlock;
	}
	return guard_right_lock;
}


void* guard_left_unlock(Person person){
	if(strcmp(person.msg, inMessage[LEFT_DOOR_OPEN]) == 0){
		sendDisplay(LEFT_DOOR_OPENED, person);
		return left_open;
	}
	return guard_left_unlock;
}


void* guard_right_unlock(Person person){
	if(strcmp(person.msg, inMessage[RIGHT_DOOR_OPEN]) == 0){
		sendDisplay(RIGHT_DOOR_OPENED, person);
		return right_open;
	}
	return guard_right_unlock;
}

void* left_close(Person person){
	if(strcmp(person.msg, inMessage[GUARD_LEFT_LOCK]) == 0){
		sendDisplay(LOCK_LEFT, person);
		if(path == 'X'){
			sendDisplay(IDLE_MSG, person);
			return idle_state;
		}
		return guard_left_lock;
	}
	return left_close;
}

void* right_close(Person person){
	if(strcmp(person.msg, inMessage[GUARD_RIGHT_LOCK]) == 0){
		sendDisplay(LOCK_RIGHT, person);
		if(path == 'N'){
			sendDisplay(IDLE_MSG, person);
			return idle_state;
		}
		return guard_right_lock;
	}
	return right_close;
}

void* weighed(Person person){
	if(strcmp(person.msg, inMessage[LEFT_DOOR_CLOSE]) == 0){
		return left_close;
	} else if (strcmp(person.msg, inMessage[RIGHT_DOOR_CLOSE]) == 0){
		return right_close;
	}
	return weighed;
}

void sendDisplay(int smsg, Person p){
	Display display;
	display.person = p;
	display.msg = smsg;
	if(MsgSend(coid, &display, sizeof(display), NULL, 0) == -1){
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}
}
