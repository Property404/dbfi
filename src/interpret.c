
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpret.h"
#include "options.h"

void interpret(const char *code, int options)
{
	int tape_size = 5;	/* *Current* length of tape */
	char *tape = calloc(tape_size, sizeof(char));
	int pointer = 0;

	/* List of loop positions */
	int *loop_queue = malloc(0);
	int loops = 0;		/* Number of loops */

	/* iterative variables */
	int i;
	int j;
	int temp;

	/* options */
	SET_OPTIONS_VARIABLE(options);


	for (i = 0; code[i] != '\0'; i++) {
		switch (code[i]) {
			/* Vomit out cell contents */
		case '#':
			if HAS_OPTION
				(OPT_DEBUG) {
				printf("[");
				for (j = 0; j < 10; j++) {
					printf("%d|", tape[j]);
				}
				printf("]\n");
				break;
				}
			break;
			/* Sum up all pluses and add to cell */
		case '+':
			tape[pointer]++;
			for (; code[i + 1] == '+'; i++) {
				tape[pointer]++;
			}
			break;
		case '-':
			tape[pointer]--;
			for (; code[i + 1] == '-'; i++) {
				tape[pointer]--;
			}
			break;
		case ',':
			tape[pointer] = getchar();
			break;
		case '>':
			temp = tape_size;
			pointer++;
			while (code[i + 1] == '>') {
				pointer++;
				i++;
			}
			/* Push to end of tape */
			if (pointer >= tape_size) {
				char *buffer =
				    calloc((pointer + 1), sizeof(char));
				for (j = 0; j < tape_size; j++) {
					buffer[j] = tape[j];
				}
				free(tape);
				tape = buffer;
				tape_size += pointer - temp + 1;
			}
			break;
		case '<':
			pointer--;
			for (; code[i + 1] == '<'; i++) {
				pointer--;
			}
			if (pointer < 0) {
				fprintf(stderr,
					"error: negative pointer\n");
				exit(1);
			}
			break;
		case '.':
			printf("%c", tape[pointer]);
			break;
		case ']':
			/* Go to beginning of loop */
			i = loop_queue[loops - 1] - 1;
			/* pop */
			loops -= 1;
			break;
		case '[':
			if (tape[pointer] != 0) {
				int *buffer;
				int j;
				/* Push loop */
				loops++;
				buffer = malloc(loops * sizeof(int));
				for (j = 0; j < loops - 1; j++) {
					buffer[j] = loop_queue[j];
				}
				buffer[loops - 1] = i;
				free(loop_queue);
				loop_queue = buffer;
			} else {
				/* Go to end of loop */
				int x = 0;
				while (code[i + 1] != '\0') {

					if (code[i] == '[') {
						x++;
					} else if (code[i] == ']') {
						x--;
					}
					i++;
					if (x == 0) {
						break;
					}
				}
				i--;
			}
			break;
		}
	}
}
