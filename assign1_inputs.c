#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>
#include <unistd.h>
#include <sys/dispatch.h>
#include "../../assign1_controller/src/door_entry.h"

Person inputPerson;

int main(int argc, char* argv[]) {
	int  server_coid;
	char input_cmd[10];

	if ((server_coid = name_open(ATTACH_POINT_C, 0)) == -1)
	{
		perror("name_open failed.");
		return EXIT_FAILURE;
	}
	inputPerson.state = LEFT_SCAN;
	while (1)
	{

		printf("Enter the event type:\n ls = left scan\n rs = right scan\n ws = weight scale\n"
				" lo = left open\n ro = right open\n lc = left closed\n rc = right closed\n"
				" gru = guard right unlock\n grl = guard right lock\n gll = guard left lock\n glu = guard left unlock\n");

		scanf("%s", &input_cmd);
		for(int i = 0; i < NUM_INPUTS; i++)
		{
			if (strcmp(input_cmd , inMessage[i])==0)
			{
				inputPerson.state = i;
				break;
			}
		}
		if (strcmp(input_cmd, inMessage[LEFT_SCAN]) == 0 )
		{

			fflush( stdout );
			printf("Enter your ID number:\n");
			scanf("%d", &inputPerson.personID);
			strcpy(inputPerson.direction , "inbound");
		}
		else if (strcmp(input_cmd, inMessage[RIGHT_SCAN]) == 0 )
		{
			fflush( stdout );
			printf("Enter your ID number:\n");
			scanf("%d", &inputPerson.personID);
			strcpy(inputPerson.direction , "outbound");
		}
		else if (strcmp(input_cmd , inMessage[WEIGHT_SCAN]) == 0)
		{
			fflush( stdout );
			printf("Enter your weight:\n");
			scanf("%d", &inputPerson.weight);
		}
		else if (strcmp(input_cmd , inMessage[EXIT_PRG]) == 0)
		{
			printf("Exiting Inputs\n");
			if (MsgSend (server_coid, &inputPerson, sizeof (inputPerson) + 1, &inputPerson, sizeof (inputPerson)) == -1)
			{
				fprintf (stderr, "Error during MsgSend\n");
				perror (NULL);
				exit (EXIT_FAILURE);
			}
			break;
		}

		if (MsgSend (server_coid, &inputPerson, sizeof (inputPerson) + 1, &inputPerson, sizeof (inputPerson)) == -1)
		{
			fprintf (stderr, "Error during MsgSend\n");
			perror (NULL);
			exit (EXIT_FAILURE);
		}
	}
	//Disconnect from the channel
	name_close(server_coid);
	return EXIT_SUCCESS;
}
