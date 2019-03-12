#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <unistd.h>
#include <sys/dispatch.h>
#include "door_entry.h"

char serverRsp [50];
Person inputPerson;
Display controllerDisplay;
name_attach_t *attach;
int  rcvid;         // indicates who we should reply to
int  chid;          // the channel ID
int server_coid;


void waitState();
void scanState();
void leftDoorUnlockedState();
void openLeftDoorState();
void weightState();
void leftDoorClosedState();
void leftDoorLockedState();
void rightDoorUnlockedState();
void openRightDoorState();
void rightDoorClosedState();
void rightDoorlockedState();
void msgSendReply(Display Display, Person Person);
void (*statefunc)() = waitState;


void waitState()
{
	controllerDisplay.outMsg = WAIT_MSG;

	if (MsgSend (server_coid, &controllerDisplay, sizeof (controllerDisplay) + 1, &serverRsp, sizeof (serverRsp)) == -1)
	{
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}
	statefunc = scanState;
}
void scanState()
{
	if ( inputPerson.state == LEFT_SCAN || inputPerson.state == RIGHT_SCAN)
	{
		controllerDisplay.personID = inputPerson.personID;
		controllerDisplay.outMsg = SCANNED_MSG;

		msgSendReply(controllerDisplay, inputPerson);

		if (strcmp(inputPerson.direction, "inbound")==0)
			statefunc = leftDoorUnlockedState;
		else
			statefunc = rightDoorUnlockedState;
	}
	else
		MsgReply(rcvid, EOK, &inputPerson, sizeof (inputPerson));
}

void leftDoorUnlockedState()
{
	if ( inputPerson.state == GUARD_lEFT_UNLOCK)
	{
		controllerDisplay.outMsg = LEFT_DOOR_UNLOCKED_MSG;
		controllerDisplay.personWieght = inputPerson.weight;

		msgSendReply(controllerDisplay, inputPerson);

		statefunc = openLeftDoorState;
	}
	else
		MsgReply(rcvid, EOK, &inputPerson, sizeof (inputPerson));
}
void openLeftDoorState()
{
	if ( inputPerson.state == LEFT_DOOR_OPEN)
	{
		controllerDisplay.outMsg = OPEN_LEFT_DOOR_MSG;

		msgSendReply(controllerDisplay, inputPerson);

		if (strcmp(inputPerson.direction, "inbound")==0)
			statefunc = weightState;
		else
			statefunc = leftDoorClosedState;
	}
	else
		MsgReply(rcvid, EOK, &inputPerson, sizeof (inputPerson));
}
void weightState()
{
	if ( inputPerson.state == WEIGHT_SCAN)
	{
		controllerDisplay.outMsg = WEIGHT_MSG;
		controllerDisplay.personWieght = inputPerson.weight;

		msgSendReply(controllerDisplay, inputPerson);

		if (strcmp(inputPerson.direction, "inbound")==0)
			statefunc = leftDoorClosedState;
		else
			statefunc = rightDoorClosedState;
	}
	else
		MsgReply(rcvid, EOK, &inputPerson, sizeof (inputPerson));

}
void leftDoorClosedState()
{
	if ( inputPerson.state == LEFT_CLOSE)
	{
		controllerDisplay.outMsg = LEFT_DOOR_CLOSED_MSG;

		msgSendReply(controllerDisplay, inputPerson);

		statefunc = leftDoorLockedState;
	}
	else
		MsgReply(rcvid, EOK, &inputPerson, sizeof (inputPerson));
}
void leftDoorLockedState()
{
	if ( inputPerson.state == GUARD_lEFT_LOCK)
	{
		controllerDisplay.outMsg = LEFT_DOOR_LOCKED_MSG;

		msgSendReply(controllerDisplay, inputPerson);

		if (strcmp(inputPerson.direction, "inbound")==0)
			statefunc = rightDoorUnlockedState;
		else
			statefunc = waitState;
	}
	else
		MsgReply(rcvid, EOK, &inputPerson, sizeof (inputPerson));
}

void rightDoorUnlockedState()
{
	if ( inputPerson.state == GUARD_RIGHT_UNLOCK)
	{
		controllerDisplay.outMsg = RIGHT_DOOR_UNLOCK_MSG;

		msgSendReply(controllerDisplay, inputPerson);

		statefunc = openRightDoorState;
	}
	else
		MsgReply(rcvid, EOK, &inputPerson, sizeof (inputPerson));
}
void openRightDoorState()
{
	if ( inputPerson.state == RIGHT_DOOR_OPEN)
	{
		controllerDisplay.outMsg = OPEN_RIGHT_DOOR_MSG;

		msgSendReply(controllerDisplay, inputPerson);

		if (strcmp(inputPerson.direction, "inbound")==0)
			statefunc = rightDoorClosedState;
		else
			statefunc = weightState;
	}
	else
		MsgReply(rcvid, EOK, &inputPerson, sizeof (inputPerson));
}
void rightDoorClosedState()
{
	if ( inputPerson.state == RIGHT_CLOSE)
	{
		controllerDisplay.outMsg = RIGHT_DOOR_CLOSED_MSG;

		msgSendReply(controllerDisplay, inputPerson);

		statefunc = rightDoorlockedState;
	}
	else
		MsgReply(rcvid, EOK, &inputPerson, sizeof (inputPerson));
}
void rightDoorlockedState()
{
	if ( inputPerson.state == GUARD_RIGHT_LOCK)
	{
		controllerDisplay.outMsg = RIGHT_DOOR_LOCKED_MSG;

		msgSendReply(controllerDisplay, inputPerson);

		if (strcmp(inputPerson.direction, "inbound")==0)
			statefunc = waitState;
		else
			statefunc = leftDoorUnlockedState;
	}
	else
		MsgReply(rcvid, EOK, &inputPerson, sizeof (inputPerson));
}
void msgSendReply(Display controllerDisplay, Person inputPerson)
{
	if (MsgSend (server_coid, &controllerDisplay, sizeof (controllerDisplay) + 1, &serverRsp, sizeof (serverRsp)) == -1)
	{
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}
	MsgReply(rcvid, EOK, &inputPerson, sizeof (inputPerson));
}
int main(int argc, char* argv[]) {



	if ((server_coid = name_open(ATTACH_POINT_D, 0)) == -1) {
		perror("name_open failed.");
		return EXIT_FAILURE;
	}

	// checks if it was successful
	if ((attach = name_attach(NULL, ATTACH_POINT_C, 0)) == NULL)
	{
		return EXIT_FAILURE;
	}
	printf("Controller PID: %d\n", getpid());

	while (1)
	{
		if (statefunc != waitState)
			rcvid = MsgReceive (attach->chid, &inputPerson, sizeof (inputPerson),NULL);

		(*statefunc)();

		if(inMessage[inputPerson.state] == inMessage[EXIT_PRG])
		{
			controllerDisplay.outMsg = EXIT_DISPLAY;
			if (MsgSend (server_coid, &controllerDisplay, sizeof (controllerDisplay) + 1, &serverRsp, sizeof (serverRsp)) == -1)
			{
				fprintf (stderr, "Error during MsgSend\n");
				perror (NULL);
				exit (EXIT_FAILURE);
			}
			printf("%s\n", outMessage[EXIT_CONTROLLER]);

			break;
		}


	}
    name_close(server_coid);
	name_detach(attach, 0);
	return EXIT_SUCCESS;
}
