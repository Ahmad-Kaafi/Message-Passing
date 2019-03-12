#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <unistd.h>
#include <sys/dispatch.h>
#include "../../assign1_controller/src/door_entry.h"

Display controllerDisplay;
int main(int argc, char* argv[]) {
	int     rcvid;         // indicates who we should reply to
	char serverRsp [50];
	name_attach_t *attach;
	// checks if it was successful
	if ((attach = name_attach(NULL, ATTACH_POINT_D, 0)) == NULL)
	{
		return EXIT_FAILURE;
	}
	printf("Display PID: %d\n", getpid());

	while(1)
	{
		rcvid = MsgReceive (attach->chid, &controllerDisplay, sizeof (controllerDisplay),NULL);

		if (controllerDisplay.outMsg == SCANNED_MSG)
			printf("%s %d\n", outMessage[controllerDisplay.outMsg], controllerDisplay.personID );

		else if (controllerDisplay.outMsg == WEIGHT_MSG)
			printf("%s %d\n", outMessage[WEIGHT_MSG], controllerDisplay.personWieght);

		else if (controllerDisplay.outMsg == EXIT_DISPLAY)
		{
			printf("%s\n", outMessage[EXIT_DISPLAY]);
			MsgReply (rcvid, EOK, &serverRsp, sizeof (serverRsp));
			break;
		}

		else
			printf("%s\n", outMessage[controllerDisplay.outMsg]);

		MsgReply (rcvid, EOK, &serverRsp, sizeof (serverRsp));

	}
	// destroy the channel when done
	name_detach(attach, 0);

	return EXIT_SUCCESS;
}
