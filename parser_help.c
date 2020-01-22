//COP4610
//Project 1 Starter Code
//example code for initial parsing

//*** if any problems are found with this code,
//*** please report them to the TA


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct
{
	char** tokens;
	int numTokens;
} instruction;

void addToken(instruction* instr_ptr, char* tok);
void printTokens(instruction* instr_ptr);
void clearInstruction(instruction* instr_ptr);
void addNull(instruction* instr_ptr);
char *GetEnv(const char* name);

int main() {
	char* token = NULL;
	char* temp = NULL;

	instruction instr;
	instr.tokens = NULL;
	instr.numTokens = 0;

	printf("Starting up terminal...\n");

	while (1) {

		printf("%s@%s:%s>",getenv("USER"),getenv("MACHINE"),getenv("PWD"));

		// loop reads character sequences separated by whitespace
		do {
			//scans for next token and allocates token var to size of scanned token
			scanf("%ms", &token);

			//simple exit to help exit program for testing. ** Needs to execute built in exit from requirements
			if (strcmp(token, "exit") == 0 || strcmp(token, "Exit") == 0 || strcmp(token, "EXIT") == 0)
			{
				printf("Goodbye!\nClosing Shell...\n");
				return 0;
			}

			temp = (char*)malloc((strlen(token) + 1) * sizeof(char));

			int i;
			int start = 0;
			for (i = 0; i < strlen(token); i++) {
				//pull out special characters and make them into a separate token in the instruction
				if (token[i] == '|' || token[i] == '>' || token[i] == '<' || token[i] == '&') {
					if (i-start > 0) {
						memcpy(temp, token + start, i - start);
						temp[i-start] = '\0';
						addToken(&instr, temp);
					}

					char specialChar[2];
					specialChar[0] = token[i];
					specialChar[1] = '\0';

					addToken(&instr,specialChar);

					start = i + 1;
				}
				if (token[i]=='$'){
					char env[10];
					strcpy(env, &token[1]);					//copy everything after $
					GetEnv(env);							//use getenv function to return appropriate output
				}
				if (token[i]=='.'&& token[i+1]=='.'){	   //cd..
					char buffer[100];
					chdir("..");
					if(strlen(buffer)==1)    				 //check if you just have'/'
					 	printf("You are at root.\n");
					else
						printf("New directory is: %s\n",getcwd(buffer,100));
				}
				if (token[i]=='.'){	   //cd.
					char buffer[100];
				     printf("Stayed in same directory: %s\n",getcwd(buffer,100));
				}

				if (token[i]=='~'){     //cd ~
					GetEnv("HOME");     //directs back to home directory
				}

			}

			if (start < strlen(token)) {
				memcpy(temp, token + start, strlen(token) - start);
				temp[i-start] = '\0';
				addToken(&instr, temp);
			}

			//free and reset variables
			free(token);
			free(temp);

			token = NULL;
			temp = NULL;
		} while ('\n' != getchar());    //until end of line is reached

		addNull(&instr);
		//printTokens(&instr);
		clearInstruction(&instr);
	}

	return 0;
}


char *GetEnv(const char* name)
{
   printf("%s : %s\n",name,getenv(name));
}

//reallocates instruction array to hold another token
//allocates for new token within instruction array
void addToken(instruction* instr_ptr, char* tok)
{
	//extend token array to accomodate an additional token
	if (instr_ptr->numTokens == 0)
		instr_ptr->tokens = (char**) malloc(sizeof(char*));
	else
		instr_ptr->tokens = (char**) realloc(instr_ptr->tokens, (instr_ptr->numTokens+1) * sizeof(char*));

	//allocate char array for new token in new slot
	instr_ptr->tokens[instr_ptr->numTokens] = (char *)malloc((strlen(tok)+1) * sizeof(char));
	strcpy(instr_ptr->tokens[instr_ptr->numTokens], tok);

	instr_ptr->numTokens++;
}

void addNull(instruction* instr_ptr)
{
	//extend token array to accomodate an additional token
	if (instr_ptr->numTokens == 0)
		instr_ptr->tokens = (char**)malloc(sizeof(char*));
	else
		instr_ptr->tokens = (char**)realloc(instr_ptr->tokens, (instr_ptr->numTokens+1) * sizeof(char*));

	instr_ptr->tokens[instr_ptr->numTokens] = (char*) NULL;
	instr_ptr->numTokens++;
}

// void printTokens(instruction* instr_ptr)
// {
// 	int i;
// 	printf("Tokens:\n");
// 	for (i = 0; i < instr_ptr->numTokens; i++) {
// 		if ((instr_ptr->tokens)[i] != NULL)
// 			printf("%s\n", (instr_ptr->tokens)[i]);
// 	}
// }

void clearInstruction(instruction* instr_ptr)
{
	int i;
	for (i = 0; i < instr_ptr->numTokens; i++)
		free(instr_ptr->tokens[i]);

	free(instr_ptr->tokens);

	instr_ptr->tokens = NULL;
	instr_ptr->numTokens = 0;
}
