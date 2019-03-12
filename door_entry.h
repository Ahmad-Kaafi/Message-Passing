/*
 * door_entry.h
 *
 *  Created on: Feb 20, 2019
 *      Author: ahmed
 */


#ifndef DOOR_ENTRY_H_
#define DOOR_ENTRY_H_

#define NUM_STATES 11	// where n = number of states in your finite state machine (FSM)
#define ATTACH_POINT_D "displayAP"
#define ATTACH_POINT_C "controllerAP"

typedef enum {
	//TODO :: to see the list of //TODOS, Window > Show View > Tasks
	//TODO :: assign an enum value, one for each state
	WAIT_STATE,
	SCANNED_STATE,
	LEFT_DOOR_UNLOCKED_STATE,
	OPEN_LEFT_DOOR_STATE,
	WEIGHT_STATE,
	LEFT_DOOR_CLOSED_STATE,
	LEFT_DOOR_LOCKED_STATE,
	RIGHT_DOOR_UNLOCK_STATE,
	OPEN_RIGHT_DOOR_STATE,
	RIGHT_DOOR_CLOSED_STATE,
	RIGHT_DOOR_LOCKED_STATE,
	EXIT
} State;

#define NUM_INPUTS 12	// where n equals the number of input commands that drive the FSM.
// From the assignment specification, notice the prompt. Each command, ls, rs, etc.
// Count those...
typedef enum {
	//TODO :: assign an enum value, one for each input command
	LEFT_SCAN,
	RIGHT_SCAN,
	GUARD_lEFT_UNLOCK,
	LEFT_DOOR_OPEN,
	WEIGHT_SCAN,
	LEFT_CLOSE,
	GUARD_lEFT_LOCK,
	GUARD_RIGHT_UNLOCK,
	RIGHT_DOOR_OPEN,
	RIGHT_CLOSE,
	GUARD_RIGHT_LOCK,
	EXIT_PRG
} Input;

const char *inMessage[NUM_INPUTS] = {
		//TODO :: each input command. For example, "ls"
		"ls",
		"rs",
		"glu",
		"lo",
		"ws",
		"lc",
		"gll",
		"gru",
		"ro",
		"rc",
		"grl",
		"exit"
};

#define NUM_OUTPUTS 13	// where n equals the number of output messages from the FSM.
typedef enum {
	//TODO :: assign an enum value, one for each output message from the FSM
	WAIT_MSG,
	SCANNED_MSG,
	LEFT_DOOR_UNLOCKED_MSG,
	OPEN_LEFT_DOOR_MSG,
	WEIGHT_MSG,
	LEFT_DOOR_CLOSED_MSG,
	LEFT_DOOR_LOCKED_MSG,
	RIGHT_DOOR_UNLOCK_MSG,
	OPEN_RIGHT_DOOR_MSG,
	RIGHT_DOOR_CLOSED_MSG,
	RIGHT_DOOR_LOCKED_MSG,
	EXIT_CONTROLLER,
	EXIT_DISPLAY
} Output;

const char *outMessage[NUM_OUTPUTS] = {
		//TODO :: each output message. For example, "Person opened left door"
		"Waiting for person...",
		"Person scanned ID, ID = ",
		"Left door unlocked by Guard.",
		"Person opened left door.",
		"Person weighed, Weight = ",
		"Left door closed.",
		"Left door locked by Guard.",
		"Right door unlocked by Guard.",
		"Person opened right door.",
		"Right door closed.",
		"Right door locked by Guard.",
		"Exiting Controller",
		"Exit Display"
};

// inputs client sends a Person struct to its server, the controller
typedef struct {
	//TODO ::   fields for person's ID (int), weight (int), direction (inbound or outbound),
	//			and some way to remember what state the Person is in. Suppose the Person in "Left Scan" state.
	//			You need a way to represent that.
	int personID;
	int weight;
	char direction [10];
	int state;
} Person;

// controller client sends a Display struct to its server, the display
typedef struct {
	//TODO ::   fields for the output message and the Person. For the output message, I used an int, which
	//			is the index into the outMessages array. That way I don't pass strings in my messages.
	//			The Person field is needed, as some output message require information from the Person.
	//			Specifically, those messages that display the Person's ID and weight.
	int outMsg;
	int personID;
	int personWieght;
} Display;


#endif /* DOOR_ENTRY_H_ */
