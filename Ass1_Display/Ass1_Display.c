#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/neutrino.h>

#include "mystructs.h"

int main(int argc, char *argv[]) {
	int rcvid;
	int chid;

	Display display;

	chid = ChannelCreate(0);
	if(chid == -1){
		perror("Failed to create a channel.");
		exit(EXIT_FAILURE);
	}
	printf("Server PID is: %d\n", getpid());

	while(1){
		rcvid = MsgReceive(chid, &display, sizeof(display), NULL);
		MsgReply(rcvid, EOK, NULL, 0);

		if(display.msg == ID_SCAN)
			printf("%s, ID = %d\n", outMessage[display.msg], display.person.personid);
		else if(display.msg == WEIGHED)
			printf("%s, Weight = %d\n", outMessage[display.msg], display.person.weight);
		else
			printf("%s\n", outMessage[display.msg]);

		if(display.msg == STOP_MSG){
			exit(EXIT_SUCCESS);
		}
	}
	return EXIT_SUCCESS;
}
