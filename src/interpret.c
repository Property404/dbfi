#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "interpret.h"
#define BF_SHIFT '>'
#define BF_ADD '+'
#define BF_SKIP 's'
#define BF_GOTO 'g'
#define BF_PUT '.'
#define BF_GET ','
#define BF_END 'E'
struct BFCommand {
	char ctype;		//command type
	int value;
};
/* Convert code into list of commands 
 * The idea here is that this is faster to run
 * than interpreting the ['s and ]'s as you go along*/
static void compile(struct BFCommand *commands, const char *code)
{
	int count = 0;/* How many command objects have we put in the 'commands' list */
	int skip_queue[30000];
	int skips = 0;/* Where we are in the skip queue */
	for (int i = 0; code[i] != '\0'; i++) {
		if (code[i] == '[') {
			/* Record where a '[' would go to */
			commands[count].ctype = BF_SKIP;
			skip_queue[skips] = count;
			count++;
			skips++;
		} else if (code[i] == ']') {
			/* Record where a ']'would go back to */
			skips--;
			commands[count].ctype = BF_GOTO;
			commands[count].value = skip_queue[skips];
			commands[skip_queue[skips]].value = count;
			count++;
		} else if (code[i] == '<' || code[i] == '>') {
			/* Add up all <'s and >'s */
			commands[count].ctype = BF_SHIFT;
			commands[count].value = code[i] == '<' ? -1 : 1;
			while (code[i + 1] == '<' || code[i + 1] == '>') {
				commands[count].value +=
				    code[i + 1] == '<' ? -1 : 1;
				i++;
			}
			count++;
		} else if (code[i] == '+' || code[i] == '-') {
			commands[count].ctype = BF_ADD;
			commands[count].value = code[i] == '-' ? -1 : 1;
			while (code[i + 1] == '-' || code[i + 1] == '+') {
				commands[count].value +=
				    code[i + 1] == '-' ? -1 : 1;
				i++;
			}
			count++;
		} else if (code[i] == '.') {
			/* Note: Value doesn't do anything. Consecutive '.'
 * 			are just consecutive commands. I don't think this has a performance hit */
			commands[count].ctype = BF_PUT;
			commands[count].value = 1;
			count++;
		} else if (code[i] == ',') {
			/* See above */
			commands[count].ctype = BF_GET;
			commands[count].value = 1;
			count++;
		}

	}
	/* This object tells interpreter to stop running */
	commands[count].ctype = BF_END;
}
/* Interpret commands */
void run(const char *code, int options)
{
	/* Create list of commands from code */
	struct BFCommand *commands;
	commands = malloc(sizeof(struct BFCommand) * (strlen(code) + 2));
	compile(commands, code);

	/* TODO: Change tape to dynamic allocation */
	char tape[30000] = { 0 };
	int pointer = 150;

	/* Go through command objects */
	for (int i = 0; commands[i].ctype != BF_END; i++) {
		switch (commands[i].ctype) {
		case BF_SHIFT:
			pointer += commands[i].value;
			break;
		case BF_ADD:
			tape[pointer] += commands[i].value;
			break;
		case BF_PUT:
			putchar(tape[pointer]);
			break;
		case BF_GET:
			tape[pointer] = getchar();
			break;
		case BF_SKIP:
			if (tape[pointer] == 0) {
				i = commands[i].value;
			}
			break;
		case BF_GOTO:
			i = commands[i].value - 1;
			break;
		}
		//printf("{%c:%i}\n",commands[i].ctype,commands[i].value);
	}

	free(commands);
}
