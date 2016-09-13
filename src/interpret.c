#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "interpret.h"
#include "timer.h"
#include "options.h"
#define BF_SHIFT '>'
#define BF_ADD '+'
#define BF_SKIP 's'
#define BF_GOTO 'g'
#define BF_PUT '.'
#define BF_GET ','
#define BF_DEBUG '~'
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
	int count = 0;		/* How many command objects have we put in the 'commands' list */
	int skip_max = 10;
	int *skip_queue = calloc(skip_max, sizeof(int));
	int skips = 0;		/* Where we are in the skip queue */
	for (int i = 0; code[i] != '\0'; i++) {
		if (code[i] == '[') {
			/* Record where a '[' would go to */
			commands[count].ctype = BF_SKIP;
			skip_queue[skips] = count;
			count++;
			skips++;
			/*Reallocate skip_queue */
			if (skips >= skip_max) {
				int *buffer =
				    calloc(skip_max, sizeof(int));
				for (int j = 0; j < skip_max; j++) {
					buffer[j] = skip_queue[j];
				}
				free(skip_queue);
				skip_queue =
				    calloc(skip_max + 1, sizeof(int));
				for (int j = 0; j < skip_max; j++) {
					skip_queue[j] = buffer[j];
				}
				free(buffer);
				skip_max++;
			}
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
			 *                      are just consecutive commands. I don't think this has a performance hit */
			commands[count].ctype = BF_PUT;
			commands[count].value = 1;
			count++;
		} else if (code[i] == ',') {
			/* See above */
			commands[count].ctype = BF_GET;
			commands[count].value = 1;
			count++;
		} else if (code[i] == '~') {
			commands[count].ctype = BF_DEBUG;
			count++;
		}

	}
	/* This object tells interpreter to stop running */
	commands[count].ctype = BF_END;
	free(skip_queue);
}

/* Interpret commands */
void run(const char *code, int options)
{
	/* Create list of commands from code */
	struct BFCommand *commands;
	commands = malloc(sizeof(struct BFCommand) * (strlen(code) + 2));
	compile(commands, code);

	int tape_size = 1;
	char *tape = calloc(tape_size, sizeof(char));
	int pointer = 0;

	/* Set up options */
	SET_OPTIONS_VARIABLE(options);

	/* Record time if -t */
	START_TIMER;

	/* Go through command objects */
	for (int i = 0; commands[i].ctype != BF_END; i++) {
		switch (commands[i].ctype) {
		case BF_SHIFT:
			pointer += commands[i].value;
			/* Check for pointer errors */
			if (pointer < 0) {
				fprintf(stderr,
					"runtime error: negative pointer\n");
				exit(1);
			}

			/* Reallocate tape */
			else if (pointer >= tape_size) {
				char *buffer =
				    calloc(tape_size +
					   (1 + pointer - tape_size),
					   sizeof(char));
				for (int j = 0; j < tape_size; j++) {
					buffer[j] = tape[j];
				}
				free(tape);
				tape =
				    calloc(tape_size +
					   (1 + pointer - tape_size),
					   sizeof(char));
				for (int j = 0; j < tape_size; j++) {
					tape[j] = buffer[j];
				}
				tape_size += 1 + pointer - tape_size;
				free(buffer);
			}
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
		case BF_DEBUG:
			if (HAS_OPTION(OPT_DEBUG)) {
				printf("{%d}", tape[pointer]);
			}
		}
		//printf("{%c:%i}\n",commands[i].ctype,commands[i].value);
	}

	if (HAS_OPTION(OPT_TIMER)) {
		STOP_TIMER;
		PRINT_TIMER;
	}
	free(commands);
	free(tape);
}
