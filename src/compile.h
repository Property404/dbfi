#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BF_SHIFT 3
#define BF_ADD 68
#define BF_SKIP 9
#define BF_GOTO 12
#define BF_PUT 15
#define BF_GET 18
#define BF_END 21
struct BFCommand {
	int ctype;		//command type
	int value;
	int wherewasi;
};
/* Convert code into list of commands */
    void compile(struct BFCommand *commands, const char *code)
{
	commands = malloc(sizeof(struct BFCommand) * (strlen(code) + 2));
	int count = 0;
	int skip_queue[3000];
	int skips = 0;
	for (int i = 0; code[i] != '\0'; i++) {
		if (code[i]=='['){
			printf("[\n");
			commands[count].ctype = BF_SKIP;
			skip_queue[skips] = count;
			count++;skips++;
		}else if (code[i] == ']'){
			printf("]\n");
			skips--;
			commands[count].ctype = BF_GOTO;
			commands[count].value = skip_queue[skips];
			commands[skip_queue[skips]].value = count;
			count++;
		}
		else if (code[i] == '<' || code[i] == '>') {
			commands[count].ctype = BF_SHIFT;
			commands[count].value = code[i] == '<' ? -1 : 1;
			while (code[i + 1] == '<' || code[i + 1] == '>') {
				commands[count].value +=
				    code[i] == '<' ? -1 : 1;
				i++;
			}
			count++;
		} else if (code[i] == '+' || code[i] == '-') {
			printf("+\n");
			commands[count].ctype = BF_ADD;
			commands[count].value = code[i] == '-' ? -1 : 1;
			while (code[i + 1] == '-' || code[i + 1] == '+') {
				commands[count].value +=
				    code[i] == '-' ? -1 : 1;
				i++;
			}
			count++;
		} else if (code[i] == '.') {
			commands[count].ctype = BF_PUT;
			commands[count].value = 1;
			count++;
		}

	}
	commands[count].ctype = BF_END;
}

void run(const char* code, int options){
	struct BFCommand *commands;	
	printf("About to compile\n");
	compile(commands, code);
	printf("Compiled\n");/*
	for(int i=0;commands[i].ctype!=BF_END;i++){
		printf("{%i:%i}\n",commands[i].ctype,commands[i].value);
	}*/
} 
