#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>

#include "mystructs.h"

int main(int argc, char *argv[]) {
	int coid;
	pid_t serverpid = atoi(argv[1]);
	Person person;

	coid = ConnectAttach(ND_LOCAL_NODE, serverpid, 1, _NTO_SIDE_CHANNEL, 0);
	if (coid == -1) {
		fprintf (stderr, "Couldn't ConnectAttach\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}
	while(1){
		printf("Enter the event type (ls = left scan, rs = right scan, ws = weight scale, lo = left open, ro = right open, "
			   "lc = left closed, rc = right closed , gru = guard right unlock, grl = guard right lock, gll = guard left lock, "
			   "glu = guard left unlock, stop = stop program)\n");
		scanf("%s", &person.msg);

		if(strcmp(person.msg, "ls") == 0 || strcmp(person.msg, "rs") == 0){
			printf("Enter the person_id\n");
			scanf("%d", &person.personid);
		}

		if(strcmp(person.msg, "ws") == 0){
			printf("Enter the weight\n");
			scanf("%d", &person.weight);
		}

		if(MsgSend(coid, &person, sizeof(person), NULL, 0) == -1){
			fprintf (stderr, "Error during MsgSend\n");
			perror (NULL);
			exit (EXIT_FAILURE);
		}

		if(strcmp(person.msg, "stop") == 0){
			exit(EXIT_SUCCESS);
		}
	}

	return EXIT_SUCCESS;
}
