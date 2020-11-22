#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include "Safeinput.h"
#define STRLEN 50


//---SUPPORT FUNCTIONS---//
INPUT_RESULT GetInput(char* prompt, char* buff, int maxSize)
{
	if (prompt != NULL && strlen(prompt) > 0)
	{
		printf("%s", prompt);
	}
	if (fgets(buff, maxSize, stdin) == NULL)
		return INPUT_RESULT_NO_INPUT;

	if (buff[strlen(buff) - 1] != '\n') {
		int extra = 0;
		char ch;
		while (((ch = getchar()) != '\n') && (ch != EOF))
			extra = 1;
		return (extra == 1) ? INPUT_RESULT_TOO_LONG : INPUT_RESULT_OK;
	}

	buff[strlen(buff) - 1] = '\0';
	return INPUT_RESULT_OK;
}
bool GetInputInt(char* prompt, int* value)
{
	char buff[255];
	if (GetInput(prompt, buff, sizeof(buff)) != INPUT_RESULT_OK)
		return false;
	long l = LONG_MIN;

	l = strtol(buff, NULL, 10);
	if (l == LONG_MIN) return false;
	*value = l;
	return true;
}
void time_stamp(char* now, time_t card_time) {
	struct tm* Right_now;
	Right_now = localtime(&card_time);
	strftime(now, STRLEN, "%Y-%m-%d", Right_now);
}
//---SUPPORT FUNCTIONS---//

//---STRUCTS---//
typedef struct {
	char cardID[STRLEN];
	bool access;
	time_t time;
} CARD;


typedef struct {
	CARD* allCards;
	int numberOfCards;
} SYSTEM_STATE;
//---STRUCTS---//

//---MAIN FUNCTIONS---//

//---SELECTION 1---//
void remoteOpenDoor() {
	printf("\nDoor is open, lamp is green.\n");
	Sleep(3000);
	printf("\nDoor is closed, lamp is red.\n");
	Sleep(1000);
	return;
}
//---SELECTION 2---//
void listCards(SYSTEM_STATE* state) {
	int i;
	char now[STRLEN];

	printf("Listing all cards\n");
	for (i = 0; i < state->numberOfCards; i++) {
		time_stamp(now, state->allCards[i].time);
		printf("\n%s\t%s\t%s\n", state->allCards[i].cardID, state->allCards[i].access ? "Access: 1" : "Access: 0", now);
	}
}
//---SELECTION 3---//
void accessHandler(SYSTEM_STATE* state) {
	int access = 0;
	char cardNumber[STRLEN];
	char newID[STRLEN];

	while (1) {
		int index;
		bool found = false;
		GetInput("\nCard ID:", newID, STRLEN);
		if (!found || state->numberOfCards == 0){
			break;
		};
		for (index = 0; index < state->numberOfCards; index++) {
			if (!strcmp(state->allCards[index].cardID, newID)) {
				found = true;
				GetInputInt("Enter new access: 1 for access, 0 for no access: ", &access);
				state->allCards[index].access = access;
				printf("\nNew access: %i \n", access);
				return;
			}
		}
	}
	if (state->numberOfCards == 0) {
		state->allCards = malloc(sizeof(CARD));
	}
	else {
		state->allCards = realloc(state->allCards, sizeof(CARD) * (state->numberOfCards + 1));
	}
	GetInputInt("1 for access, 0 for no access: ", &access);
	strcpy(state->allCards[state->numberOfCards].cardID, newID);
	state->allCards[state->numberOfCards].access = access;
	state->allCards[state->numberOfCards].time = time(NULL);
	state->numberOfCards++;
	printf("\nNew access set: %i\n", access);
}
//---SELECTION 9---//
void fakeScan(SYSTEM_STATE* state) {
	int i;
	char cardNumber[STRLEN];
	char now[STRLEN];
		GetInput("Scan a card\n", cardNumber, STRLEN);
		for (i = 0; i < state->numberOfCards; i++) {
			if (!strcmp(state->allCards[i].cardID, cardNumber)) {
				time_stamp(now, state->allCards[i].time);
				printf("\n%s\t%s\t%s\n", state->allCards[i].cardID, state->allCards[i].access ? "\nLamp: Green\n" : "\nLamp: Red\n", now);
				Sleep(1000);
			} else {
				printf("Can't find that card.");
			}
	}
}
//---MAIN FUNCTIONS---//

//---MAIN---//
void mainMenu(SYSTEM_STATE* state) {
	int selection;
	
	while (1) {
		printf("||----------------------------------||\n");
		printf("1.        Remote open door\n");
		printf("2.        List all cards\n");
		printf("3.        Add/Remove Access\n");
		printf("4.        Exit\n");
		printf("9.        Scan card\n");
		printf("||----------------------------------||\n");
		GetInputInt("Select Choice: ", &selection);
		if (selection == 1){
			remoteOpenDoor(state);
		}
		if (selection == 2){
			listCards(state);
		}
		if (selection == 3){
			accessHandler(state);
		}
		if (selection == 4){
			return;
		}
		if (selection == 9){
			fakeScan(state);
		}
	}
}

int main()
{
	SYSTEM_STATE state;
	state.allCards = 0;
	state.numberOfCards = 0;
	
	mainMenu(&state);
	return 0;
}
//---MAIN---//